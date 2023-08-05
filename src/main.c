#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>

#include "devices/graphics/graphics.h"
#include "devices/keyboard/i8042.h"
#include "devices/keyboard/kbc.h"
#include "devices/keyboard/scancodes.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "devices/timer/timer.h"
#include "file/directory.h"
#include "file/file.h"
#include "state_machine/state_machine.h"
#include "ui/menu.h"
#include "ui/ui.h"
#include "utils.h"

Image32 background_image;
Image32 sun_image;
Image32 moon_image;
Image32 folder_image;

void(update_screen)(uint16_t size, Image32 cursor) {
  draw_rect(0, 0, screen.width, screen.height, darkMode ? black : white);
  draw_image(0, 0, screen.width, screen.height, background_image);
  draw_image(screen.width - 40 * size, 8 * size, 4 * 8 * size, 4 * 8 * size, darkMode ? moon_image : sun_image);
  if (darkMode) {
    draw_text("NIGHT", screen.width - 44 * size, 41 * size, size, white);
  }
  else {
    draw_text("DAY", screen.width - 36 * size, 41 * size, size, white);
  }

  draw_folder_icon(size);
  draw_background_info(size * 2);
  process_draw(size);
  draw_image(mouse_x, mouse_y, size * 10, size * 10, cursor);
  flip_page();
}

int(main)(int argc, char *argv[]) {

  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();

  return 0;
}

bool has_changed = false;

int(proj_main_loop)(int argc, char *argv[]) {
  int result = 0;
  printf("%d\n", chdir("/home/lcom/labs/proj/src"));
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
  }

  load_ui();

  sun_image = loadImageFromFile("/home/lcom/labs/proj/src/assets/sun.png");
  moon_image = loadImageFromFile("/home/lcom/labs/proj/src/assets/moon.png");
  background_image = loadImageFromFile("/home/lcom/labs/proj/src/assets/background.jpg");
  Image32 cursor = loadImageFromFile("/home/lcom/labs/proj/src/assets/cursor.png");

  if (set_frame_buffer(0x118))
    return_defer(1);

  if (set_graphic_mode(0x118))
    return_defer(1);

  if (mouse_data_reporting(0xF4))
    return 1;

  int ipc_status;
  message msg;
  uint8_t keyboard_hook_id = 5;
  uint8_t keyboard_mask = BIT(keyboard_hook_id);
  uint8_t timer_hook_id = 4;
  uint8_t timer_mask = BIT(timer_hook_id);
  uint8_t mouse_hook_id = 3;
  uint8_t mouse_mask = BIT(mouse_hook_id);
  uint8_t rtc_hook_id = 1;
  uint8_t rtc_mask = BIT(rtc_hook_id);

  if (kbc_subscribe_int(&keyboard_hook_id))
    return_defer(1);

  if (timer_subscribe_int(&timer_hook_id))
    return_defer(1);

  if (mouse_subscribe_int(&mouse_hook_id))
    return_defer(1);

  if (rtc_subscribe_int(&rtc_hook_id))
    return_defer(1);

  rtc_init();

  int run = 1;
  uint16_t size = 2;

  makeMenus();

  struct mouse_ev event;

  set_folder_icon(size, &folder_image);
  update_screen(size, cursor);

  while (run) {

    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("driver_receive failed");
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: {
          if (msg.m_notify.interrupts & rtc_mask) {
            has_changed = true;
            rtc_ih();
            rtc_clear_interrupts();
          }

          if (msg.m_notify.interrupts & timer_mask) {
            if (f_data.count > 0) {
              copy_file_worker();
            }
            if (has_changed) {
              has_changed = false;
              update_screen(size, cursor);
            }
          }
          if (msg.m_notify.interrupts & keyboard_mask) {
            if (kbc_read_scancode()) {
              return_defer(1);
            }
            int temp = process_keyboard_state(&size);
            if (temp == 1)
              return_defer(1);
            if (temp == 2)
              run = 0;
          }
          if (msg.m_notify.interrupts & mouse_mask) {
            mouse_ih();
            if (byte_index == 3) {
              if (!error) {
                mouse_construct_packet();
                mouse_update(screen.width, screen.height);
                event = mouse_event();
                process_mouse_state(mouse_x, mouse_y, event, size);
                has_changed = true;
              }
              byte_index = 0;
              error = false;
            }
          }
        } break;
        default:
          break;
      }
    }
  }

defer:
  freeImage(cursor);

  printf("result=%d\n", result);
  result |= rtc_unsubcsribe_int();
  printf("result=%d\n", result);
  result |= vg_exit();
  printf("result=%d\n", result);
  result |= kbc_unsubscribe_int();
  printf("result=%d\n", result);
  result |= mouse_unsubscribe_int();
  printf("result=%d\n", result);
  result |= timer_unsubscribe_int();
  printf("result=%d\n", result);
  result |= mouse_data_reporting(0xF5);
  printf("result=%d\n", result);
  return result;
}
