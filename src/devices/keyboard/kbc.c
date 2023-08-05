#include "kbc.h"
#include "i8042.h"
#include <lcom/lcf.h>

uint8_t scancode[2];
uint8_t scancode_count = 0;
static int hook_id = -1;

int(kbc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }
  hook_id = *bit_no;
  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int(kbc_unsubscribe_int)() {
  if (hook_id < 0)
    return 1;
  return sys_irqrmpolicy(&hook_id);
}

/**Read/Write*/
int(kbc_read_scancode)() {
  if (kbc_read_byte(&scancode[scancode_count++])) {
    scancode_count = 0;
    return 1;
  }
  return 0;
}

int(kbc_read_byte)(uint8_t *byte) {
  uint8_t status;
  while (1) {
    if (util_sys_inb(KBC_STATUS_REG, &status)) {
      return 1;
    }
    if (status & KBC_FULL_OUT_BUFFER) {
      if (util_sys_inb(KBC_OUT_CMD, byte)) {
        return 1;
      }
      if ((status & KBC_PARITY_ERROR) || (status & KBC_TIMEOUT_ERROR)) {
        return 1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(KBC_WAIT));
  }
  return 1;
}

int(kbc_write_byte)(uint8_t byte) {
  uint8_t status;
  while (1) {
    if (util_sys_inb(KBC_STATUS_REG, &status)) {
      return 1;
    }
    if (!(status & KBC_FULL_IN_BUFFER)) {
      // write in 0x60 the modified byte to restore the keyboard
      if (sys_outb(KBC_OUT_CMD, byte)) {
        return 1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(KBC_WAIT));
  }
  return 1;
}
