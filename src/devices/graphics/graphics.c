#include "graphics.h"
#include <lcom/lcf.h>

#include "font/font8x8.h"
#include "utf-8/utf8_decode.h"

#include "../../utils.h"

// REAL_TIME current_time;
uint8_t *frame_buffer;
uint8_t *front_buffer;
uint8_t *back_buffer;
unsigned int v_he;
vbe_mode_info_t mode_info;
Image24 screen;
Image24 window;
WindowStack windows;

pixel24 black = {0, 0, 0};
pixel24 white = {255, 255, 255};
pixel24 green = {0, 255, 0};
pixel24 darkgreen = {0, 128, 0};
pixel24 grey = {211, 211, 211};
pixel24 red = {0, 0, 255};

void(window_to_screen)(int x, int y, int *out_x, int *out_y, Window win) {
  *out_x = x + win.offset_x;
  *out_y = y + win.offset_y;
}

void(screen_to_window)(int x, int y, int *out_x, int *out_y, Window win) {
  *out_x = x - win.offset_x;
  *out_y = y - win.offset_y;
}

Window(push_window)(int x, int y, int width, int height) {
  int offset_x = x;
  int offset_y = y;
  if (windows.count > 0) {
    offset_x += windows.items[windows.count - 1].offset_x;
    offset_y += windows.items[windows.count - 1].offset_y;
  }

  Window win = {
    .offset_x = offset_x,
    .offset_y = offset_y,
    .window = subImage24(window, x, y, width, height),
  };
  window = win.window;
  da_append(&windows, win);
  return win;
}

void(pop_window)() {
  if (windows.count > 1) {
    windows.count--;
  }
  window = windows.items[windows.count - 1].window;
}

bool(mouse_inside_window)(int mouse_x, int mouse_y, Window win) {
  screen_to_window(mouse_x, mouse_y, &mouse_x, &mouse_y, win);
  return !(mouse_x < 0 || mouse_y < 0 || mouse_x > (int) win.window.width || mouse_y > (int) win.window.height);
}

int(set_graphic_mode)(uint16_t mode) {

  reg86_t reg86;
  memset(&reg86, 0, sizeof(reg86_t));
  reg86.intno = 0x10;
  reg86.ax = 0x4F02;
  reg86.bx = mode | BIT(14);
  if (sys_int86(&reg86) != 0) {
    printf("Set graphic mode failed\n");
    return 1;
  }
  return 0;
}

int(set_frame_buffer)(uint16_t mode) {

  // retirar informação sobre o @mode
  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info))
    return 1;

  // cálculo do tamanho do frame buffer, em bytes
  unsigned int bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;
  v_he = mode_info.YResolution;

  // preenchimento dos endereços físicos
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr;                      // início físico do buffer
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size * 2; // fim físico do buffer

  // alocação física da memória necessária para o frame buffer
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  // alocação virtual da memória necessária para o frame buffer
  frame_buffer = vm_map_phys(SELF, (void *) physic_addresses.mr_base, frame_size * 2);
  if (frame_buffer == NULL) {
    printf("Virtual memory allocation error\n");
    return 1;
  }

  front_buffer = frame_buffer;
  back_buffer = frame_buffer + frame_size;

  screen.data = TYPE_PUN(pixel24 *, back_buffer);
  screen.width = mode_info.XResolution;
  screen.height = mode_info.YResolution;
  screen.stride = screen.width;

  windows.count = 0;
  window = screen;
  push_window(0, 0, window.width, window.height);
  printf("start %d\n", windows.count);

  return 0;
}

Bounds(normalize_rect)(int x, int y,
                       uint16_t width, uint16_t height) {
  Bounds b;
  b.xmin = x;
  b.ymin = y;
  b.xmax = x + width;
  if (b.xmax > (uint16_t) window.width)
    b.xmax = window.width;
  b.ymax = y + height;
  if (b.ymax > (uint16_t) window.height)
    b.ymax = window.height;

  Window win = windows.items[windows.count - 1];

  if (b.xmin + win.offset_x < 0) {
    b.xmin = -win.offset_x;
  }
  if (b.ymin + win.offset_y < 0) {
    b.ymin = -win.offset_y;
  }

  if (b.xmin < 0)
    b.xmin = 0;
  if (b.ymin < 0)
    b.ymin = 0;

  if (b.xmax + win.offset_x > (int) screen.width) {
    b.xmax = (int) screen.width - win.offset_x;
  }
  if (b.ymax + win.offset_y > (int) screen.height) {
    b.ymax = (int) screen.height - win.offset_y;
  }

  return b;
}

void(draw_rect)(int x, int y, uint16_t width, uint16_t height, pixel24 color) {
  Bounds b = normalize_rect(x, y, width, height);
  for (int i = b.xmin; i < b.xmax; i++) {
    for (int j = b.ymin; j < b.ymax; j++) {
      ImageAt(window, i, j) = color;
    }
  }
}

void(draw_image)(int x, int y, uint16_t width, uint16_t height, Image32 img) {
  if (img.data == NULL)
    return;

  float img_aspect_ratio = img.width / (float) img.height;
  float dest_aspect_ratio = width / (float) height;
  if (img_aspect_ratio > dest_aspect_ratio) {
    int new_height = (img.height * width) / img.width;
    y += (height - new_height) / 2;
    height = new_height;
  }
  else {
    int new_width = (img.width * height) / img.height;
    x += (width - new_width) / 2;
    width = new_width;
  }

  Bounds b = normalize_rect(x, y, width, height);
  for (int i = b.xmin; i < b.xmax; i++) {
    for (int j = b.ymin; j < b.ymax; j++) {

      pixel32 image = ImageAt(img, (i - x) * img.width / width, (j - y) * img.height / height);
      pixel24 current = ImageAt(window, i, j);
      uint8_t alpha = image.a;

      pixel24 new_color;
      new_color.r = ((alpha * image.r) + ((255 - alpha) * current.r)) / 255;
      new_color.g = ((alpha * image.g) + ((255 - alpha) * current.g)) / 255;
      new_color.b = ((alpha * image.b) + ((255 - alpha) * current.b)) / 255;
      ImageAt(window, i, j) = new_color;
    }
  }
}

void(draw_outline_rect)(int x, int y, uint16_t width, uint16_t height, pixel24 color, uint16_t thickness) {
  Bounds b = normalize_rect(x, y, width, height);

  for (int i = x; i < x + width; i++) {
    if (i < b.xmin || i >= b.xmax)
      continue;
    for (int j = 1; j <= thickness; j++) {

      if (y + j - 1 >= b.ymin && y + j - 1 < b.ymax)
        ImageAt(window, i, y + j - 1) = color;
      if (y + height - j >= b.ymin && y + height - j < b.ymax)
        ImageAt(window, i, y + height - j) = color;
    }
  }
  for (int i = y; i < y + height; i++) {
    if (i < b.ymin || i >= b.ymax)
      continue;

    for (int j = 1; j <= thickness; j++) {
      if (x + j - 1 >= b.xmin && x + j - 1 < b.xmax)
        ImageAt(window, x + j - 1, i) = color;
      if (x + width - j >= b.xmin && x + width - j < b.xmax)
        ImageAt(window, x + width - j, i) = color;
    }
  }
}

char invalidChar[] = {~0x1E, ~0x33, ~0x30, ~0x18, ~0x0C, ~0x00, ~0x0C, ~0x00};

char *(loadBitmap) (int codepoint) {

  if (codepoint <= 0x007F)
    return font8x8_basic[codepoint];
  if (codepoint <= 0x009F)
    return font8x8_control[codepoint - 0x0080];
  if (codepoint <= 0x00FF)
    return font8x8_ext_latin[codepoint - 0x00A0];

  if (codepoint < 0x0390)
    return invalidChar;

  if (codepoint <= 0x03C9)
    return font8x8_greek[codepoint - 0x0390];

  if (codepoint < 0x2500)
    return invalidChar;

  if (codepoint <= 0x257F)
    return font8x8_box[codepoint - 0x2500];
  if (codepoint <= 0x259F)
    return font8x8_block[codepoint - 0x2580];

  if (codepoint < 0x3040)
    return invalidChar;

  if (codepoint <= 0x309F)
    return font8x8_hiragana[codepoint - 0x3040];

  return invalidChar;
}

void(draw_text_with_background)(char *text, int x, int y, uint16_t size, pixel24 foregroudColor, pixel24 backgroundColor) {
  if (text == NULL)
    return;
  utf8_decode_init(text, strlen(text));
  int codepoint;
  while ((codepoint = utf8_decode_next()) != UTF8_END) {
    if (codepoint != UTF8_ERROR) {
      char *bitmap = loadBitmap(codepoint);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          int set = bitmap[j] & 1 << i;
          if (set) {
            draw_rect(x + i * size, y + j * size, size, size, foregroudColor);
          }
          else {
            draw_rect(x + i * size, y + j * size, size, size, backgroundColor);
          }
        }
      }
    }
    x += size * 8;
  }
}

void(draw_parsed_text_with_background)(int *text, int x, int y, uint16_t size, pixel24 foregroudColor, pixel24 backgroundColor) {
  if (text == NULL)
    return;
  for (int i = 0; text[i] != 0; i++) {
    char *bitmap = loadBitmap(text[i]);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        int set = bitmap[j] & 1 << i;
        if (set) {
          draw_rect(x + i * size, y + j * size, size, size, foregroudColor);
        }
        else {
          draw_rect(x + i * size, y + j * size, size, size, backgroundColor);
        }
      }
    }
    x += size * 8;
  }
}

void(draw_text)(char *text, int x, int y, uint16_t size, pixel24 color) {
  if (text == NULL)
    return;
  utf8_decode_init(text, strlen(text));
  int codepoint;
  while ((codepoint = utf8_decode_next()) != UTF8_END) {
    if (codepoint != UTF8_ERROR) {
      char *bitmap = loadBitmap(codepoint);
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          int set = bitmap[j] & 1 << i;
          if (set) {
            draw_rect(x + i * size, y + j * size, size, size, color);
          }
        }
      }
    }
    x += size * 8;
  }
}

int(flip_page)() {
  uint16_t page;
  uint8_t *tmp = front_buffer;
  front_buffer = back_buffer;
  back_buffer = tmp;
  if (front_buffer > back_buffer) {
    page = v_he;
  }
  else {
    page = 0;
  }

  screen.data = TYPE_PUN(pixel24 *, back_buffer);
  windows.items[0].window.data = TYPE_PUN(pixel24 *, back_buffer);
  window = screen;
  windows.count = 1;

  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ax = 0x4F07;
  r86.bx = 0x0000;
  r86.cx = 0;
  r86.dx = page;

  if (sys_int86(&r86) != 0) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }
  return 0;
}
