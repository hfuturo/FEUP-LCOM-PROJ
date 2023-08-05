#include <lcom/lcf.h>

#include "assert.h"
#include "devices/graphics/graphics.h"
#include "devices/rtc/rtc.h"
#include "file/directory.h"
#include "file/file.h"
#include "state_machine/state_machine.h"
#include "ui.h"
#include "utf-8/utf8_decode.h"
#include <dirent.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>

int MAXINDEXDIR = 1000000000;
char file_path_selected[PATH_MAX] = {0};
char file_name_selected[PATH_MAX] = {0};
bool is_directory = false;
int selected = 1;

char cwd[PATH_MAX] = {0};

/** @brief Icon of a folder */
static Image32 icon_pasta;

typedef struct ImageCache {
  Image32 img;
  char *dir;
  char *filename;
} ImageCache;

#define CACHE_CAP 100
ImageCache cached_images[CACHE_CAP];
size_t img_index = 0;
extern Image32 img;
extern enum State state;

int scroll = 0;
int list_files_x = 30;
int list_files_y = 30;
int list_files_last_x = 30;
int list_files_last_y = 30;
int list_files_width = 500;
int list_files_height = 500;
int list_files_padding = 10;
bool list_files_minimized = true;
bool list_files_maximized = false;
static bool list_files_was_maximized = false;

static int origin_x = -1;
static int origin_y = -1;
static bool list_files_dragging = false;
static bool list_files_scrolling = false;
static bool mouseSelect = false;

static Window last_list_window;
static Window last_main_window;
static uint16_t last_size;

uint16_t menu_x = 0;
uint16_t menu_y = 0;

void(load_ui)() {
  icon_pasta = loadImageFromFile("/home/lcom/labs/proj/src/assets/pastas.png");
  memset(cached_images, 0, sizeof(cached_images));
}

Image32(chache_put)(Image32 img, char *dir, char *path) {
  printf("caching image from file %s/%s\n", dir, path);
  img_index = (img_index + 1) % CACHE_CAP;
  freeImage(cached_images[img_index].img);
  free(cached_images[img_index].dir);
  free(cached_images[img_index].filename);
  cached_images[img_index].img = img;
  cached_images[img_index].dir = strdup(dir);
  cached_images[img_index].filename = strdup(path);
  return img;
}

Image32(cache_lookup)(char *dir, char *path) {
  for (int i = 0; i < CACHE_CAP; i++) {
    if (cached_images[i].filename != NULL && strcmp(path, cached_images[i].filename) == 0 && strcmp(dir, cached_images[i].dir) == 0) {
      return cached_images[i].img;
    }
  }
  return chache_put(loadImageFromFile(path), dir, path);
}

void(cache_invalidate)(char *dir, char *path) {
  for (int i = 0; i < CACHE_CAP; i++) {
    if (cached_images[i].filename != NULL && strcmp(path, cached_images[i].filename) == 0 && strcmp(dir, cached_images[i].dir) == 0) {
      printf("invalidating cached image of file %s/%s\n", dir, path);
      freeImage(cached_images[i].img);
      free(cached_images[i].dir);
      free(cached_images[i].filename);
      cached_images[i].filename = NULL;
      return;
    }
  }
}

void(release_mouse_file_list)() {
  list_files_dragging = false;
  list_files_scrolling = false;
}

void(set_scroll)(int new_value) {
  int wh = last_list_window.window.height;
  int scroll_height = 8 * last_size * MAXINDEXDIR;
  scroll = new_value;
  if (scroll < 0) {
    scroll = 0;
  }
  else if (scroll_height < wh) {
    scroll = 0;
  }
  else if (scroll > scroll_height - wh) {
    scroll = scroll_height - wh;
  }
  printf("scroll %d wh %d\n", scroll, wh);
}

void(calculate_scrollbar)(int *bar_height, int *bar_y, int *scroll_height, int *wh) {
  *wh = last_list_window.window.height;
  *scroll_height = 8 * last_size * MAXINDEXDIR;
  *bar_height = ((*wh) * (*wh)) / (*scroll_height);
  float a = scroll / ((float) (*scroll_height) - (*wh));
  *bar_y = a * ((*wh) - (*bar_height));
}

void(move_mouse_file_list)(int mouse_x, int mouse_y) {
  if (!list_files_scrolling && !list_files_dragging)
    return;

  if (list_files_scrolling) {
    screen_to_window(mouse_x, mouse_y, &mouse_x, &mouse_y, last_list_window);

    int bar_height, bar_y, scroll_height, wh;
    calculate_scrollbar(&bar_height, &bar_y, &scroll_height, &wh);

    int dy = mouse_y - origin_y;
    int diff = (dy * (scroll_height - wh)) / (wh - bar_height);
    int last_scroll = scroll;
    set_scroll(scroll + diff);
    if (last_scroll + diff != scroll) {
      origin_y += scroll - last_scroll;
    }
    else {
      origin_y = mouse_y;
    }
  }
  else if (list_files_dragging) {
    list_files_x += mouse_x - origin_x;
    list_files_y += mouse_y - origin_y;

    origin_x = mouse_x;
    origin_y = mouse_y;
  }
}

bool(click_mouse_file_list)(int mouse_x, int mouse_y) {
  if (list_files_minimized) {
    mouseSelect = false;
    return false;
  }

  if (!mouse_inside_window(mouse_x, mouse_y, last_main_window)) {
    mouseSelect = false;
    return false;
  }

  active_window = LIST_FILES_ACTIVE;

  int main_x, main_y, list_x, list_y;
  screen_to_window(mouse_x, mouse_y, &main_x, &main_y, last_main_window);
  screen_to_window(mouse_x, mouse_y, &list_x, &list_y, last_list_window);
  printf("click at %d %d last_list_window x=%d y=%d\n", main_x, main_y, last_list_window.offset_x, last_list_window.offset_y);

  int bar_height, bar_y, scroll_height, wh;
  calculate_scrollbar(&bar_height, &bar_y, &scroll_height, &wh);

  if (main_y < 10 * last_size) {
    origin_x = mouse_x;
    origin_y = mouse_y;
    list_files_dragging = !list_files_maximized;
    mouseSelect = false;
    return true;
  }

  if (
    list_x > (int) last_list_window.window.width - 10 * last_size &&
    list_y > bar_y &&
    list_y < bar_y + bar_height /**/
  ) {
    printf("start scroll at %d\n", list_y);

    origin_y = list_y;
    list_files_scrolling = true;
    mouseSelect = false;
    return true;
  }

  switch (state) {
    case RENAME:
      commit_rename();
      break;
    case NEW_FILE:
      commit_new_file();
      break;
    case NEW_DIRECTORY:
      commit_new_dir();
      break;
    default:
      break;
  }

  if (mouseSelect && menuPost(menuFile, menu_x, menu_y, list_x, list_y, last_size)) {
    mouseSelect = false;
    return true;
  }

  mouseSelect = false;
  int new_selected = (scroll + list_y) / (8 * last_size);
  if (selected == new_selected)
    open_selected();
  else
    update_selected(new_selected);
  return true;
}

bool(right_click_mouse_file_list)(int mouse_x, int mouse_y) {
  mouseSelect = false;
  if (list_files_minimized)
    return false;
  if (!mouse_inside_window(mouse_x, mouse_y, last_main_window))
    return false;

  active_window = LIST_FILES_ACTIVE;

  switch (state) {
    case RENAME:
      commit_rename();
      break;
    case NEW_FILE:
      commit_new_file();
      break;
    case NEW_DIRECTORY:
      commit_new_dir();
      break;
    default:
      break;
  }

  if (!mouse_inside_window(mouse_x, mouse_y, last_list_window)) {
    return true;
  }

  int list_x, list_y;
  screen_to_window(mouse_x, mouse_y, &list_x, &list_y, last_list_window);
  printf("right click at %d %d last_list_window x=%d y=%d\n", list_x, list_y, last_list_window.offset_x, last_list_window.offset_y);

  int bar_height, bar_y, scroll_height, wh;
  calculate_scrollbar(&bar_height, &bar_y, &scroll_height, &wh);

  if (
    list_x > (int) last_list_window.window.width - 10 * last_size &&
    list_y > bar_y &&
    list_y < bar_y + bar_height /**/
  ) {
    return true;
  }

  update_selected((scroll + list_y) / (8 * last_size));

  menu_x = list_x;
  menu_y = list_y;
  mouseSelect = true;

  return true;
}

void(toogle_maximized)() {
  list_files_minimized = false;
  if (!list_files_maximized) {
    list_files_last_x = list_files_x;
    list_files_last_y = list_files_y;
    list_files_width = screen.width;
    list_files_height = screen.height;
    list_files_x = 0;
    list_files_y = 0;
    list_files_maximized = true;
  }
  else {
    list_files_x = list_files_last_x;
    list_files_y = list_files_last_y;
    list_files_width = 500;
    list_files_height = 500;
    list_files_maximized = false;
  }
}

void(update_selected)(int value) {
  has_changed = true;
  int wh = last_list_window.window.height;
  mouseSelect = false;
  selected = value;
  if (selected < 0)
    selected = 0;
  if (selected >= MAXINDEXDIR)
    selected = MAXINDEXDIR - 1;
  if (selected * 8 * last_size < scroll) {
    set_scroll(selected * 8 * last_size);
  }
  else if ((selected + 1) * 8 * last_size - wh > scroll) {
    set_scroll((selected + 1) * 8 * last_size - wh);
  }
  else {
    set_scroll(scroll);
  }
}

int(list_files)(uint16_t size, bool rename, bool new) {
  if (list_files_minimized)
    return 0;

  if (cwd[0] == '\0')
    getcwd(cwd, sizeof(cwd));

  draw_outline_rect(list_files_x - list_files_padding, list_files_y - list_files_padding, list_files_width + list_files_padding * 2, list_files_height + list_files_padding * 2, green, list_files_padding);

  push_window(list_files_x, list_files_y, list_files_width, list_files_height);
  last_main_window = windows.items[windows.count - 1];
  draw_rect(0, 0, window.width, window.height, darkMode ? black : white);

  draw_rect(0, 0, window.width, 10 * size, darkMode ? white : black);
  push_window(0, 0, window.width - 8 * 10 * size, 10 * size);
  draw_text(cwd, 0, size, size, darkMode ? black : white);
  pop_window();

  char time_string[10];
  sprintf(time_string, "%02d:%02d:%02d", current_time.hour, current_time.minute, current_time.second);

  draw_text(time_string, window.width - 8 * 8 * size, size, size, darkMode ? black : white);
  if (new) {
    draw_text(">", 0, size * 10, size, darkMode ? white : black);
    draw_text_box(10 * size, size * 10, darkMode ? black : white, darkMode ? white : black, size);
    push_window(0, 18 * size, window.width, window.height - 18 * size);
  }
  else {
    push_window(0, 10 * size, window.width, window.height - 10 * size);
  }
  last_list_window = windows.items[windows.count - 1];
  last_size = size;

  if (list_files_maximized != list_files_was_maximized) {
    update_selected(selected);
  }
  list_files_was_maximized = list_files_maximized;

  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (!d) {
    printf("Error opening directory.\n");
    return 1;
  }

  int y = 0;
  int index = 0;

  while ((dir = readdir(d)) != NULL) {
    int wy = y - scroll;
    if (index == selected && rename) {
      draw_text(">", 0, wy, size, darkMode ? white : black);
    }
    else if (dir->d_type & DT_DIR)
      draw_image(0, wy, size * 8, size * 8, icon_pasta);
    else if (
      strcmp(&dir->d_name[dir->d_namlen - 4], ".png") == 0 ||
      strcmp(&dir->d_name[dir->d_namlen - 4], ".bmp") == 0 ||
      strcmp(&dir->d_name[dir->d_namlen - 4], ".jpg") == 0 ||
      strcmp(&dir->d_name[dir->d_namlen - 5], ".jpeg") == 0 /**/
    ) {
      Image32 img = cache_lookup(cwd, dir->d_name);
      draw_image(0, wy, size * 8, size * 8, img);
    }

    if (index == selected) {
      if (rename) {
        draw_text_box(size * 10, wy, darkMode ? black : white, darkMode ? white : black, size);
      }
      else {
        draw_text_with_background(dir->d_name, size * 10, wy, size, darkgreen, darkMode ? white : black);
      }
    }
    else {
      draw_text(dir->d_name, size * 10, wy, size, darkMode ? white : black);
    }

    y += size * 8;
    index++;
  }

  MAXINDEXDIR = index;

  int bar_height, bar_y, scroll_height, wh;
  calculate_scrollbar(&bar_height, &bar_y, &scroll_height, &wh);

  draw_rect(window.width - 10 * size, bar_y, 10 * size, bar_height, darkgreen);

  if (mouseSelect) {
    draw_menu(menuFile, menu_x, menu_y, darkMode ? black : white, darkMode ? white : black, size);
  }

  pop_window();
  pop_window();

  if (closedir(d) == -1) {
    printf("Error closing directory.\n");
    return 1;
  }

  return 0;
}

int(open_selected)() {
  bool is_dir;
  has_changed = true;
  char *file_name = get_selected_info(&is_dir);
  if (is_dir) {
    update_selected(0);
    chdir(file_name);
    getcwd(cwd, sizeof(cwd));
  }
  int len = strlen(file_name);
  if (strcmp(&file_name[len - 4], ".png") == 0 ||
      strcmp(&file_name[len - 4], ".bmp") == 0 ||
      strcmp(&file_name[len - 4], ".jpg") == 0 ||
      strcmp(&file_name[len - 5], ".jpeg") == 0) {
    img = cache_lookup(cwd, file_name);
    last_state = state;
    state = OPEN_IMAGE;
  }

  return 0;
}

int(delete_selected)() {
  bool is_dir;
  char *file_name = get_selected_info(&is_dir);

  if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0)
    return 0;

  if (is_dir) {
    if (delete_directory(file_name))
      return 1;
  }
  else {
    if (delete_file(file_name))
      return 1;
  }

  MAXINDEXDIR--;
  update_selected(selected);

  return 0;
}

int(check_name_in_list)(char *new_name) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (!d) {
    printf("Error opening directory.\n");
    return 1;
  }
  while ((dir = readdir(d)) != NULL) {
    if (strcmp(new_name, dir->d_name) == 0) {
      printf("Error name already exists in directory.\n");
      return 1;
    }
  }
  if (closedir(d) == -1) {
    printf("Error closing directory.\n");
    return 1;
  }
  return 0;
}

int(rename_in_list)(char *new_name) {
  bool is_dir;
  char *file_name = get_selected_info(&is_dir);
  if (is_dir) {
    if (rename_directory(file_name, new_name))
      return 1;
  }
  else {
    if (rename_file(file_name, new_name))
      return 1;
  }

  return 0;
}

bool(save_selected_content)() {
  char *file_name = get_selected_info(&is_directory);
  strcpy(file_name_selected, file_name);
  realpath(file_name_selected, file_path_selected);
  return 0;
}

bool(move_selected_content)() {
  if (file_path_selected[0] == '\0') {
    return 1;
  }

  char new_path[PATH_MAX];
  sprintf(new_path, "%s/%s", cwd, file_name_selected);
  if (is_directory) {
    if (move_directory(file_path_selected, new_path)) {
      return 1;
    }
  }
  else {
    if (move_file(file_path_selected, new_path)) {
      return 1;
    }
  }
  return 0;
}

char temp_copy_name[PATH_MAX] = {0};

bool(copy_selected_content)() {
  if (file_path_selected[0] == '\0') {
    return 1;
  }

  strcpy(temp_copy_name, file_name_selected);

  int i = 1;
  while (check_name_in_list(temp_copy_name)) {
    sprintf(temp_copy_name, "%s.Copy(%d)", file_name_selected, i);
    i++;
  }

  char new_path[PATH_MAX];
  sprintf(new_path, "%s/%s", cwd, temp_copy_name);

  active_window = COPY_WINDOW_ACTIVE;

  if (is_directory) {
    if (copy_directory(file_path_selected, new_path)) {
      printf("Error while copying file.\n");
      return 1;
    }
  }
  else {
    if (copy_file(file_path_selected, new_path)) {
      printf("Error while copying file.\n");
      return 1;
    }
  }
  return 0;
}

char tmp_name[PATH_MAX] = {0};

char *(get_selected_info) (bool *is_dir) {
  DIR *d;
  struct dirent *dir;
  d = opendir(".");
  if (!d) {
    printf("Error opening directory.\n");
    return NULL;
  }
  int index = 0;
  while ((dir = readdir(d)) != NULL) {
    if (index == selected) {
      (*is_dir) = dir->d_type == DT_DIR;
      strcpy(tmp_name, dir->d_name);
      break;
    }
    index++;
  }
  if (closedir(d) == -1) {
    printf("Error closing directory.\n");
    return NULL;
  }

  return tmp_name;
}
