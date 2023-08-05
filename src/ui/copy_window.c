#include "ui.h"

#include "devices/graphics/graphics.h"
#include "devices/rtc/rtc.h"
#include "file/file.h"
#include "state_machine/state_machine.h"

int copy_window_x = 400;
int copy_window_y = 400;
int copy_window_padding = 10;
bool copy_window_dragging = false;
bool copy_window_minimized = false;
static int origin_x = -1;
static int origin_y = -1;
static uint16_t last_size;
static Window last_window;

void(draw_copy_window)(uint16_t size) {
  if (copy_window_minimized || f_data.count == 0)
    return;

  last_size = size;
  int window_x, window_y;
  int copy_window_width = size * 110 + 2 * copy_window_padding;
  int copy_window_height = 7 * copy_window_padding + size * 36;
  screen_to_window(copy_window_x, copy_window_y, &window_x, &window_y, windows.items[windows.count - 1]);
  last_window = push_window(window_x, window_y, copy_window_width, copy_window_height);

  draw_rect(0, 0, copy_window_width, copy_window_width, darkMode ? black : white);
  draw_rect(0, copy_window_padding, copy_window_width, size * 10, darkMode ? white : black);
  draw_outline_rect(0, 0, copy_window_width, copy_window_height, green, copy_window_padding);
  draw_text("Current copy", 2 * copy_window_padding, 2 * copy_window_padding + size * 10, size, darkMode ? white : black);

  if (f_data.items[f_data.begin].file_size > 0) {
    draw_rect(2 * copy_window_padding, 3 * copy_window_padding + size * 18, copy_window_width - 4 * copy_window_padding, size * 10, darkgreen);
    draw_rect(2 * copy_window_padding, 3 * copy_window_padding + size * 18, ((copy_window_width - 40) * f_data.items[f_data.begin].current_position) / f_data.items[f_data.begin].file_size, size * 10, green);
  }
  else {
    draw_rect(2 * copy_window_padding, 3 * copy_window_padding + size * 18, copy_window_width - 4 * copy_window_padding, size * 10, green);
  }
  char buff[255];
  sprintf(buff, "In queue %d", f_data.count - 1);
  draw_text(buff, 2 * copy_window_padding, 4 * copy_window_padding + size * 28, size, darkMode ? white : black);

  pop_window();
}

void(release_mouse_copy_window)() {
  copy_window_dragging = false;
}

bool(click_mouse_copy_window)(int mouse_x, int mouse_y) {
  if (copy_window_minimized || f_data.count == 0)
    return false;

  copy_window_dragging = false;

  if (!mouse_inside_window(mouse_x, mouse_y, last_window)) {
    return false;
  }

  active_window = COPY_WINDOW_ACTIVE;

  if (mouse_y > copy_window_y + last_size * 10 + copy_window_padding) {
    return true;
  }

  origin_x = mouse_x;
  origin_y = mouse_y;
  copy_window_dragging = true;
  return true;
}

bool(right_click_mouse_copy_window)(int mouse_x, int mouse_y) {
  if (copy_window_minimized || f_data.count == 0)
    return false;

  if (!mouse_inside_window(mouse_x, mouse_y, last_window)) {
    return false;
  }

  active_window = COPY_WINDOW_ACTIVE;

  return true;
}

void(move_mouse_copy_window)(int mouse_x, int mouse_y) {
  if (copy_window_minimized || f_data.count == 0)
    return;

  if (!copy_window_dragging)
    return;

  copy_window_x += mouse_x - origin_x;
  copy_window_y += mouse_y - origin_y;

  origin_x = mouse_x;
  origin_y = mouse_y;
}
