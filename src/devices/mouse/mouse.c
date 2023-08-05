#include <lcom/lcf.h>

#include "i8042_mouse.h"
#include "mouse.h"
#include <stdint.h>

static int hook_id_mouse;
static struct packet mouse_packet;
static struct packet last_packet;
uint8_t byte_index = 0;
static uint8_t mouse_bytes[3];
static uint8_t current_byte;
bool error;
int mouse_x = 200;
int mouse_y = 200;


int(mouse_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return 1;
  hook_id_mouse = *bit_no;
  sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse);
  return 0;
}

int(mouse_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id_mouse);
}

void(mouse_ih)() {
  kbc_read_packet();
}

int(kbc_read_packet)() {
  if (mouse_read_byte(KBC_OUT_BUF, &current_byte)) {
    return 1;
  }
  if (byte_index == 0 && (current_byte & FIRST_BYTE)) {
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
  else if (byte_index > 0) {
    mouse_bytes[byte_index] = current_byte;
    byte_index++;
  }
  return 0;
}

int(mouse_read_byte)(uint8_t port, uint8_t *byte) {
  error = false;
  uint8_t status;
  while (1) {
    if (util_sys_inb(KBC_ST_REG, &status)) {
      return 1;
    }
    if (status & KBC_OBF) {
      if (util_sys_inb(port, byte))
        return 1;
      if ((status & MOUSE_DATA) == 0)
        return 1;
      if ((status & KBC_PARITY_ERROR) || (status & KBC_TIMEOUT_ERROR)) {
        error = true;
        return 1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 0;
}

void(mouse_construct_packet)() {
  last_packet = mouse_packet;
  for (int i = 0; i < 3; i++) {
    mouse_packet.bytes[i] = mouse_bytes[i];
  }

  mouse_packet.lb = mouse_bytes[0] & MOUSE_LB;
  mouse_packet.mb = mouse_bytes[0] & MOUSE_MB;
  mouse_packet.rb = mouse_bytes[0] & MOUSE_RB;
  mouse_packet.x_ov = mouse_bytes[0] & MOUSE_X_OVERFLOW;
  mouse_packet.y_ov = mouse_bytes[0] & MOUSE_Y_OVERFLOW;
  mouse_packet.delta_x = (mouse_bytes[0] & MOUSE_X_SIGNAL) ? (mouse_bytes[1] | 0xFF00) : mouse_bytes[1];
  mouse_packet.delta_y = (mouse_bytes[0] & MOUSE_Y_SIGNAL) ? (mouse_bytes[2] | 0xFF00) : mouse_bytes[2];
}

int(mouse_write_cmd)(uint8_t port, uint8_t cmd) {
  uint8_t time = 10;
  uint8_t status;
  while (time) {
    if (util_sys_inb(KBC_ST_REG, &status)) {
      return 1;
    }
    if ((status & KBC_IBF) == 0) {
      if (sys_outb(port, cmd))
        return 1;
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    time--;
  }
  return 1;
}

int(mouse_data_reporting)(uint8_t cmd) {
  uint8_t attemps = 10;
  uint8_t mouse_response = 0;
  do {
    attemps--;
    if (mouse_write_cmd(KBC_CMD_REG, WRITE_BYTE_MOUSE))
      return 1;
    if (mouse_write_cmd(KBC_WRITE_CMD, cmd))
      return 1;
    tickdelay(micros_to_ticks(20000));
    if (util_sys_inb(KBC_WRITE_CMD, &mouse_response)) {
      return 1;
    }
  } while (mouse_response != MOUSE_ACK && attemps);
  if (attemps == 0) {
    return 1;
  }

  return 0;
}

void(mouse_update)(uint16_t res_x, uint16_t res_y) {
  if (mouse_packet.x_ov || mouse_packet.y_ov) {
    return;
  }
  mouse_x += mouse_packet.delta_x;
  mouse_y -= mouse_packet.delta_y;
  if (mouse_x < 0)
    mouse_x = 0;
  if (mouse_x > res_x - 8)
    mouse_x = res_x - 8;
  if (mouse_y < 0)
    mouse_y = 0;
  if (mouse_y > res_y - 8)
    mouse_y = res_y - 8;
}



struct mouse_ev(mouse_event)(){
  struct mouse_ev event;
  event.type = BUTTON_EV;
  // midle button never press
  if (last_packet.mb == false && mouse_packet.mb == false) {
    if (last_packet.rb == false && mouse_packet.rb == false) {
      if (last_packet.lb == true && mouse_packet.lb == true) {
        // continua primido o botão esquerdo
        event.type = MOUSE_MOV;
      }
      else {
        if (mouse_packet.lb == true) {
          // botão esquerdo presso;
          event.type = LB_PRESSED;
        }
        if (last_packet.lb == true) {
          // botão esquerdo solto;
          event.type = LB_RELEASED;
        }
      }
    }
    if (last_packet.lb == false && mouse_packet.lb == false) {
      if (last_packet.rb == true && mouse_packet.rb == true) {
        // continua primido o botão direito
        event.type = MOUSE_MOV;
      }
      else {
        if (mouse_packet.rb == true) {
          // botão direito presso;
          event.type = RB_PRESSED;
        }
        if (last_packet.rb == true) {
          // botão direito solto;
          event.type = RB_RELEASED;
        }
      }
    }
  }
  event.delta_x = mouse_packet.delta_x;
  event.delta_y = mouse_packet.delta_y;
  return event;
}

/*
struct mouse_ev(mouse_event)() {
  int compare = compare_buttons();
  struct mouse_ev event;
  switch (compare) {
    case 1: {
      event.type = MOUSE_MOV;
      break;
    }
    case 2: {
      event.type = LB_PRESSED;
      break;
    }
    case 3: {
      event.type = LB_RELEASED;
      break;
    }
    case 4: {
      event.type = RB_PRESSED;
      break;
    }
    case 5: {
      event.type = RB_RELEASED;
      break;
    }
    default: {
      event.type = BUTTON_EV;
    }
  }
  event.delta_x = mouse_packet.delta_x;
  event.delta_y = mouse_packet.delta_y;
  return event;
}
*/
