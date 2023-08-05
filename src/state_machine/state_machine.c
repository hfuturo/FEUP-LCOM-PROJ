#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>

#include "devices/graphics/graphics.h"
#include "devices/keyboard/i8042.h"
#include "devices/keyboard/kbc.h"
#include "devices/keyboard/scancodes.h"
#include "devices/mouse/mouse.h"
#include "devices/rtc/rtc.h"
#include "file/directory.h"
#include "file/file.h"
#include "state_machine/state_machine.h"
#include "ui/menu.h"
#include "ui/ui.h"
#include "utils.h"

enum State last_state;
enum State state = NORMAL_KBC;
enum Control_V_Operation control_v_operation = NOTHING;
enum ActiveWindow active_window = COPY_WINDOW_ACTIVE;
Image32 img;
bool control = false, shift = false, caps = false;

void(commit_new_file)() {
  has_changed = true;
  state = last_state;
  char *new_name = getText();
  if (!check_name_in_list(new_name)) {
    if (create_file(new_name)) {
      free(new_name);
      return;
    }
  }
  free(new_name);
}

void(commit_new_dir)() {
  has_changed = true;
  state = last_state;
  char *new_name = getText();
  if (!check_name_in_list(new_name)) {
    if (create_directory(new_name)) {
      free(new_name);
      clean_text_box();
      return;
    }
  }
  free(new_name);
}

void(commit_rename)() {
  has_changed = true;
  char *new_name = getText();
  state = last_state;
  if (!check_name_in_list(new_name)) {
    if (rename_in_list(new_name)) {
      free(new_name);
      return;
    }
  }
  free(new_name);
}

int(process_keyboard_state)(uint16_t *size) {
  if (scancode[0] == KBC_TWO_BYTES && scancode_count == 1) {
    return 0;
  }
  scancode_count = 0;
  switch (state) {
    case NORMAL_KBC:
      if (process_kbc_mouse_common(size)) {
        return 2;
      }
      if (scancode[0] == KBC_TWO_BYTES && !list_files_minimized) {
        switch (scancode[1]) {
          case KBC_MAKE_UP:
            update_selected(selected - 1);
            break;
          case KBC_MAKE_DOWN:
            update_selected(selected + 1);
            break;
        }
      }
      if (scancode[0] == KBC_BREAK_TAB) {
        state = NORMAL_MOUSE;
      }
      break;
    case NORMAL_MOUSE:
      if (process_kbc_mouse_common(size)) {
        return 2;
      }
      if (scancode[0] == KBC_TWO_BYTES) {
        switch (scancode[1]) {
          case KBC_MAKE_DOWN:
            mouse_y += 10;
            has_changed = true;
            break;
          case KBC_MAKE_UP:
            mouse_y -= 10;
            has_changed = true;
            break;
          case KBC_MAKE_LEFT:
            mouse_x -= 10;
            has_changed = true;
            break;
          case KBC_MAKE_RIGHT:
            mouse_x += 10;
            has_changed = true;
            break;
          default:
            break;
        }
        move_mouse_file_list(mouse_x, mouse_y);
        move_mouse_copy_window(mouse_x, mouse_y);
      }
      if (scancode[0] == KBC_BREAK_TAB) {
        state = NORMAL_KBC;
      }
      break;
    case RENAME:
      process_keyboard_common();
      process_keyboard_write();
      if (scancode[0] == KBC_MAKE_ENTER && !list_files_minimized) {
        commit_rename();
      }
      break;
    case NEW_FILE:
      process_keyboard_common();
      process_keyboard_write();
      if (scancode[0] == KBC_MAKE_ENTER && !list_files_minimized) {
        commit_new_file();
      }
      break;
    case NEW_DIRECTORY:
      process_keyboard_common();
      process_keyboard_write();
      if (scancode[0] == KBC_MAKE_ENTER && !list_files_minimized) {
        commit_new_dir();
      }
      break;
    case OPEN_IMAGE:
      process_keyboard_common();
      if (scancode[0] == KBC_BREAK_ESC) {
        state = last_state;
        has_changed = true;
      }
      break;
    default:
      break;
  }
  return 0;
}

int(process_kbc_mouse_common)(uint16_t *size) {
  if (control) {
    process_keyboard_common();
    switch (scancode[0]) {
      case KBC_BREAK_PLUS:
        has_changed = true;
        (*size)++;
        break;
      case KBC_BREAK_MINUS:
        if (*size > 1) {
          (*size)--;
          has_changed = true;
        }
      case KBC_CONTROL_C: {
        if (!list_files_minimized)
          process_control_c();
        break;
      }
      case KBC_CONTROL_V:
        if (!list_files_minimized)
          process_control_v();
        break;
      case KBC_CONTROL_X:
        if (!list_files_minimized)
          process_control_x();
        break;
      case KBC_MAXIMIZE_WINDOW:
        toogle_maximized();
        has_changed = true;
        break;
      case KBC_MINIMIZE_WINDOW:
        copy_window_minimized = !copy_window_minimized;
        has_changed = true;
        break;
      case KBC_DELETE_FILE:
        if (!list_files_minimized)
          process_delete();
        break;
      case KBC_CREATE_FILE:
        if (!list_files_minimized)
          process_new_file();
        break;
      case KBC_RENAME_FILE:
        if (!list_files_minimized)
          process_rename();
        break;
      case KBC_CREATE_DIRECTORY:
        if (!list_files_minimized)
          process_new_dir();
        break;
    }
  }
  if (scancode[0] != KBC_TWO_BYTES) {
    switch (scancode[0]) {
      case KBC_MAKE_ENTER:
        if (!list_files_minimized)
          open_selected();
        break;
      case KBC_MAKE_CONTROL:
        control = true;
        break;
      case KBC_BREAK_ESC:
        return 1;
        break;
    }
  }
  return 0;
}

int(process_keyboard_write)() {
  switch (scancode[0]) {
    case KBC_MAKE_SHIFT:
      shift = true;
      break;
    case KBC_BREAK_SHIFT:
      shift = false;
      break;
    case KBC_BREAK_CAPS:
      caps = !caps;
      break;
    case KBC_MAKE_BACKSPACE:
      if (!list_files_minimized) {
        backspace_text_box();
        has_changed = true;
      }
      break;
    case KBC_BREAK_ESC:
      if (!list_files_minimized) {
        state = last_state;
        has_changed = true;
        clean_text_box();
      }
      return 1;
  }
  if (scancode[0] == KBC_TWO_BYTES) {
    if (!list_files_minimized) {
      if (scancode[1] == 0x4b) {
        move_left_text_box();
        has_changed = true;
      }
      else if (scancode[1] == 0x4d) {
        move_right_text_box();
        has_changed = true;
      }
    }
  }
  if (!list_files_minimized) {
    if (!(scancode[0] & KBC_BREAK_CODE) && scancode[0] != KBC_TWO_BYTES) {
      if (shift != caps) {
        update_text_box(scancodesShift[scancode[0] & ~KBC_BREAK_CODE]);
      }
      else {
        update_text_box(scancodes[scancode[0] & ~KBC_BREAK_CODE]);
      }
      has_changed = true;
    }
  }
  return 0;
}

void process_delete() {
  update_selected(selected);
  if (delete_selected()) {
    printf("Erro in delete_selected()\n");
  }
}

void process_control_x() {
  update_selected(selected);
  control_v_operation = MOVING;
  if (save_selected_content()) {
    printf("Erro in save_selected_content()\n");
  };
}

void process_control_c() {
  update_selected(selected);
  control_v_operation = COPYING;
  if (save_selected_content()) {
    printf("Erro in save_selected_content()\n");
  };
}

void process_control_v() {
  update_selected(selected);
  if (control_v_operation == MOVING) {
    if (move_selected_content()) {
      printf("Erro in move_selected_content()\n");
    }
  }
  if (control_v_operation == COPYING) {
    if (copy_selected_content()) {
      printf("Erro in copy_selected_content()\n");
    }
  }
}

void process_open() {
  update_selected(selected);
  open_selected();
}

void process_new_file() {
  update_selected(selected);
  last_state = state;
  state = NEW_FILE;
  clean_text_box();
}

void process_new_dir() {
  update_selected(selected);
  last_state = state;
  state = NEW_DIRECTORY;
  clean_text_box();
}

void process_rename() {
  if (selected < 2)
    return;
  update_selected(selected);
  last_state = state;
  state = RENAME;
  clean_text_box();
  bool tmp;
  char *filename = get_selected_info(&tmp);
  update_text_box(filename);
}

Menu *menuFile;

void makeMenus() {
  menuFile = newMenu();
  menuAddFunction(menuFile, "Open", process_open);
  menuAddFunction(menuFile, "Copy", process_control_c);
  menuAddFunction(menuFile, "Cut", process_control_x);
  menuAddFunction(menuFile, "Paste", process_control_v);
  menuAddFunction(menuFile, "Rename", process_rename);
  menuAddFunction(menuFile, "Delete", process_delete);
  menuAddFunction(menuFile, "New File", process_new_file);
  menuAddFunction(menuFile, "New Dir", process_new_dir);
}

int(process_mouse_state)(int mouse_x, int mouse_y, struct mouse_ev event, uint16_t size) {
  if (state == OPEN_IMAGE)
    return 0;
  if (event.type == LB_PRESSED) {
    if (active_window == LIST_FILES_ACTIVE && !list_files_maximized) {
      if (!click_mouse_file_list(mouse_x, mouse_y))
        if (!click_mouse_copy_window(mouse_x, mouse_y)) {
          if (check_mouse_in_folder(mouse_x, mouse_y)) {
            list_files_minimized = false;
            copy_window_minimized = false;
          }
          else {
            list_files_minimized = true;
            copy_window_minimized = true;
          }
        }
    }
    else {
      if (!click_mouse_copy_window(mouse_x, mouse_y))
        if (!click_mouse_file_list(mouse_x, mouse_y)) {
          if (check_mouse_in_folder(mouse_x, mouse_y)) {
            list_files_minimized = false;
            copy_window_minimized = false;
          }
          else {
            list_files_minimized = true;
            copy_window_minimized = true;
          }
        }
    }
  }
  if (event.type == LB_RELEASED) {

    release_mouse_file_list();
    release_mouse_copy_window();
  }
  if (event.type == MOUSE_MOV) {
    move_mouse_file_list(mouse_x, mouse_y);
    move_mouse_copy_window(mouse_x, mouse_y);
  }
  if (event.type == RB_PRESSED) {
    if (active_window == LIST_FILES_ACTIVE && !list_files_maximized) {
      if (!right_click_mouse_file_list(mouse_x, mouse_y))
        if (!right_click_mouse_copy_window(mouse_x, mouse_y)) {
          list_files_minimized = true;
          copy_window_minimized = true;
        }
    }
    else {
      if (!right_click_mouse_copy_window(mouse_x, mouse_y))
        if (!right_click_mouse_file_list(mouse_x, mouse_y)) {
          list_files_minimized = true;
          copy_window_minimized = true;
        }
    }
  }
  return 0;
}

int(process_keyboard_common)() {
  switch (scancode[0]) {
    case KBC_BREAK_CONTROL:
      control = false;
      break;
    default:
      return 1;
  }
  return 0;
}

int(process_draw)(uint16_t size) {
  switch (state) {
    case NEW_FILE:
    case NEW_DIRECTORY: {
      if (active_window == LIST_FILES_ACTIVE && !list_files_maximized) {
        draw_copy_window(size);
        list_files(size, false, true);
      }
      else {
        list_files(size, false, true);
        draw_copy_window(size);
      }
      break;
    }
    case RENAME: {
      if (active_window == LIST_FILES_ACTIVE && !list_files_maximized) {
        draw_copy_window(size);
        list_files(size, true, false);
      }
      else {
        list_files(size, true, false);
        draw_copy_window(size);
      }
      break;
    }
    case NORMAL_KBC:
    case NORMAL_MOUSE: {
      if (active_window == LIST_FILES_ACTIVE && !list_files_maximized) {
        draw_copy_window(size);
        list_files(size, false, false);
      }
      else {
        list_files(size, false, false);
        draw_copy_window(size);
      }
      break;
    }
    case OPEN_IMAGE: {
      int square_size = 16 * size;
      for (uint32_t i = 0; i < (screen.width / square_size) + 1; i++) {
        for (uint32_t j = 0; j < (screen.height / square_size) + 1; j++) {
          draw_rect(i * square_size, j * square_size, square_size, square_size, ((i + j) & 1) ? white : grey);
        }
      }
      draw_image(0, 0, screen.width, screen.height, img);
      break;
    }
  }
  return 0;
}
