#include "ui.h"

#include "devices/graphics/graphics.h"
#include "file/file.h"
#include "state_machine/state_machine.h"

Image32 folder_image;
int folder_x;
int folder_y;
int folder_width;
int folder_height;
static uint16_t last_size;

void(set_folder_icon)(uint16_t size, Image32 *img) {
  last_size = size;
  folder_image = loadImageFromFile("/home/lcom/labs/proj/src/assets/pastas.png");
  set_size_folder();
}

void(set_size_folder)() {
  folder_x = 8 * last_size;
  folder_y = 8 * last_size;
  folder_width = 4 * 8 * last_size;
  folder_height = 4 * 8 * last_size;
}

bool(check_mouse_in_folder)(int mouse_x, int mouse_y) {
  if ((mouse_x >= folder_x + last_size * 4) && (mouse_x <= folder_x + folder_width + last_size * 4) &&
      (mouse_y >= folder_y) && (mouse_y <= folder_y + folder_height)) {
    return true;
  }
  return false;
}

void(draw_folder_icon)(uint16_t size) {
  if (size != last_size) {
    last_size = size;
    set_size_folder();
  }
  draw_image(folder_x + last_size * 4, folder_y, folder_width, folder_height, folder_image);
  draw_text("FILES", folder_x, folder_y + folder_height + last_size, last_size, white);
}
