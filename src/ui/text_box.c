#include <lcom/lcf.h>
#include <stdint.h>

#include "../devices/graphics/graphics.h"
#include "../utils.h"
#include "assert.h"
#include "ui.h"

#include "utf-8/utf8_decode.h"

static String_Builder tmp;
static int cursor = 0;

void(update_text_box)(char *item) {
  int len = strlen(item);
  if (len == 0)
    return;

  tmp.count = 0;
  if (cursor > text.count)
    cursor = text.count;

  utf8_decode_init(item, len);
  int ch;
  while ((ch = utf8_decode_next()) != UTF8_END) {
    if (ch < 0)
      continue;
    da_append(&text, 0);
    da_append(&tmp, ch);
  }

  memmove(
    &text.items[cursor + tmp.count],
    &text.items[cursor],
    (text.count - cursor - tmp.count) * sizeof(int));

  memcpy(&text.items[cursor], tmp.items, tmp.count * sizeof(int));

  cursor += tmp.count;
  text.items[text.count] = 0;
}

void(move_left_text_box)() {
  cursor--;
  if (cursor < 0)
    cursor = 0;
}
void(move_right_text_box)() {
  cursor++;
  if (cursor > text.count)
    cursor = text.count;
}

void(backspace_text_box)() {
  if (cursor > text.count) {
    cursor = text.count;
  }
  if (cursor == 0)
    return;

  memmove(
    &text.items[cursor - 1],
    &text.items[cursor],
    (text.count - cursor) * sizeof(int));
  cursor -= 1;
  text.count -= 1;
  text.items[text.count] = 0;
}

void(delete_text_box)() {
  if (cursor >= text.count)
    return;
  memmove(
    &text.items[cursor],
    &text.items[cursor + 1],
    (text.count - cursor - 1) * sizeof(int));
  text.count -= 1;
  text.items[text.count] = 0;
}

void(clean_text_box)(){
  text.capacity = 0;
  text.count = 0;
  text.items = 0;
  tmp.capacity = 0;
  tmp.count = 0;
  tmp.items = 0;
  cursor = 0;
}

void(draw_text_box)(uint16_t x, uint16_t y, pixel24 backgroundColor, pixel24 foregroundColor, uint16_t size) {

  if (cursor < text.count) {
    int tmp = text.items[cursor];
    text.items[cursor] = '\0';
    draw_parsed_text_with_background(text.items, x, y, size, foregroundColor, backgroundColor);
    text.items[cursor] = tmp;
    draw_parsed_text_with_background(text.items + cursor, cursor * 8 * size + size + x, y, size, foregroundColor, backgroundColor);
  }
  else {
    draw_parsed_text_with_background(text.items, x, y, size, foregroundColor, backgroundColor);
  }
  draw_rect(x + cursor * 8 * size, y, size, 8 * size, red);
}

char* (getText)(){
  return utf32_to_utf8_malloced_string(text.items, text.count);
}
