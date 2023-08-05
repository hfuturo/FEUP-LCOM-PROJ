#include "menu.h"
#include "../utils.h"
#include "file/directory.h"
#include "file/file.h"
#include "utf-8/utf8_decode.h"
#include <lcom/lcf.h>

Menu *newMenu() {
  Menu *m = malloc(sizeof(Menu)); // missing error checking
  m->count = 0;
  m->capacity = 0;
  m->items = NULL;
  return m;
}

void menuAddFunction(Menu *m, char *desc, void (*func)()) {
  MenuEntry *me = malloc(sizeof(MenuEntry));
  me->desc = desc;
  me->func = func;
  da_append(m, me);
}

bool(menuPost)(Menu *m, int x, int y, int mouse_x, int mouse_y, uint16_t size) {
  uint16_t size_x = 66 * size + 20;
  uint16_t size_y = m->count * 8 * size + 20;
  if (x + size_x > (int) window.width) {
    x = x - size_x;
  }
  if (y + size_y > (int) window.height) {
    y = y - size_y;
  }
  if (mouse_x < x || mouse_x > x + size_x) {
    return false;
  }
  if (mouse_y < y || mouse_y > y + size_y) {
    return false;
  }
  int option = (mouse_y - y - 10) / (8 * size);
  m->items[option]->func();
  return true;
}

void(draw_menu)(Menu *m, int x, int y, pixel24 backgroundColor, pixel24 foregroundColor, uint16_t size) {
  uint16_t size_x = 66 * size + 20;
  uint16_t size_y = m->count * 8 * size + 20;
  // int px = x;
  if (x + size_x > (int) window.width) {
    x = x - size_x;
  }
  if (y + size_y > (int) window.height) {
    y = y - size_y;
  }

  // printf("%d %d %d\n", px, x, window.width);
  push_window(x, y, size_x, size_y);
  draw_rect(0, 0, size_x, size_y, backgroundColor);
  draw_outline_rect(0, 0, size_x, size_y, foregroundColor, size * 2);
  for (int i = 0; i < m->count; i++) {
    draw_text(m->items[i]->desc, 10, 10 + i * 8 * size, size, foregroundColor);
  }
  pop_window();
}
