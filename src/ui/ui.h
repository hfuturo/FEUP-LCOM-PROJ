#ifndef _LCOM_UI_H_
#define _LCOM_UI_H_

#include "assert.h"
#include "devices/graphics/graphics.h"
#include <lcom/lcf.h>
#include <minix/sysutil.h>

// from file_list.c
/** @brief Max index in a directory (updated with list_files function) */
extern int MAXINDEXDIR;
/** @brief Path of the item selected previously (used to move or copy a file/directory) */
extern char file_path_selected[PATH_MAX];
/** @brief Name of the item selected previously (used to move or copy a file/directory) */
extern char file_name_selected[PATH_MAX];
/** @brief True if the item selected previously is a directory and false otherwise (used to move or copy a file/directory) */
extern bool is_directory;
/** @brief Index of line selected from list of contents of current directory */
extern int selected;

extern int scroll;
/** @brief True if files list window is minimized and false otherwise */
extern bool list_files_minimized;
/** @brief True if files list window is maximized and false otherwise */
extern bool list_files_maximized;
/** @brief True if copy window is minimized and false otherwise */
extern bool copy_window_minimized;

// from folder_icon.c
/** @brief Image used for the folder icon */
Image32 folder_image;
/** @brief Position of the x component of the icon */
int folder_x;
/** @brief Position of the y component of the icon */
int folder_y;
/** @brief Size of the width the icon */
int folder_width;
/** @brief Size of the height component of the icon */
int folder_height;

// from text_box.c
/**
 * @brief Struct to save data of the text from a text box
 */
typedef struct {
  int *items;
  int count;
  int capacity;
} String_Builder;

/** @brief Text box content */
String_Builder text;

// implemented on text_box.c

/**
 * @brief Draw text box
 *
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param backgroundColor Color given to the background of the text box
 * @param foregroundColor Size given to the foreground of the text box
 * @param size Size given to the text while drawing
 */
void(draw_text_box)(uint16_t x, uint16_t y, pixel24 backgroundColor, pixel24 foregroundColor, uint16_t size);
/**
 * @brief Update text in text box with the new char added
 *
 * @param item New char to be added to the text of the text box
 */
void(update_text_box)(char *item);
/**
 * @brief Move cursor of the text box to the left
 */
void(move_left_text_box)();
/**
 * @brief Move cursor of the text box to the right
 */
void(move_right_text_box)();
/**
 * @brief Delete the char that is at the right of the cursor
 */
void(delete_text_box)();
/**
 * @brief Delete the char that is at the left of the cursor
 */
void(backspace_text_box)();
/**
 * @brief Reset the values of the variables used in the text box
 */
void(clean_text_box)();
/**
 * @brief Get text that user wrote while creating/renaming file/folder
 *
 * @return Return the word written by user
 */
char *(getText) ();
/**
 * @brief Open selected file/folder
 *
 * @return Return 0 upon success and 1 otherwise
 */

// implemented on file_list.c

int(open_selected)();
/**
 * @brief Delete selected file/folder from current directory
 *
 * @return Return 0 upon success and 1 otherwise
 */
int(delete_selected)();
/**
 * @brief Rename selected file/folder from current directory
 *
 * @param new_name Name used to replace old one
 * @return Return 0 upon success and 1 otherwise
 */
int(rename_in_list)(char *new_name);
/**
 * @brief Check If the name writen by user while creating/renaming file/folder already exists in current directory
 *
 * @param new_name Name writen by the user
 * @return Return 0 if it does not exist and 1 otherwise
 */
int(check_name_in_list)(char *new_name);
/**
 * @brief Load images that are the icons used while listing
 */
void(load_ui)();
/**
 * @brief Save path and name of the selected file/folder
 *
 * @return Return 0 upon success and 1 otherwise
 */
bool(save_selected_content)();
/**
 * @brief Move the content that was selected to current directory
 *
 * @return Return 0 upon success and 1 otherwise
 */
bool(move_selected_content)();
/**
 * @brief Copy the content that was selected to current directory
 *
 * @return Return 0 upon success and 1 otherwise
 */
bool(copy_selected_content)();
/**
 * @brief File list callback for when the mouse is released;
 */
void(release_mouse_file_list)();
/**
 * @brief File list callback for when the mouse moves
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 */
void(move_mouse_file_list)(int mouse_x, int mouse_y);
/**
 * @brief File list callback for when the mouse is left clicked
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 *
 * @return if the event shoud not pass to the window bellow
 */
bool(click_mouse_file_list)(int mouse_x, int mouse_y);
/**
 * @brief File list callback for when the mouse is right clicked
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 *
 * @return if the event shoud not pass to the window bellow
 */
bool(right_click_mouse_file_list)(int mouse_x, int mouse_y);

/**
 * @brief Toggle if the file list is maximized or not
 */
void(toogle_maximized)();

/**
 * @brief Set selected file and update the scroll acordingly
 */
void(update_selected)(int value);

/**
 * @brief List all the files and directories from the current directory
 *
 * @param color Color used to write the text
 * @param size Size of the text
 * @param rename True if state is in rename and false otherwise
 * @return Return 0 upon success and 1 otherwise
 */
int(list_files)(uint16_t size, bool rename, bool new);
/**
 * @brief Get selected content's name and check if it is a directory or a file
 *
 * @param is_dir Pointer that is set to true if content is a directory and false otherwise
 * @return Name of the content selected
 */
char *(get_selected_info) (bool *is_dir);

// implemented on copy_window.c

/**
 * @brief Draw window with the information of the copy status
 *
 * @param size Size of the text to be drawn
 */
void(draw_copy_window)(uint16_t size);
/**
 * @brief Copy window callback for when the mouse is released
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 */
void(release_mouse_copy_window)();
/**
 * @brief Copy window callback for when the mouse is moved
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 */
void(move_mouse_copy_window)(int mouse_x, int mouse_y);
/**
 * @brief Copy window callback for when the mouse is left clicked
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 *
 * @return if the event shoud not pass to the window bellow
 */
bool(click_mouse_copy_window)(int mouse_x, int mouse_y);
/**
 * @brief Copy window callback for when the mouse is right clicked
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 *
 * @return if the event shoud not pass to the window bellow
 */
bool(right_click_mouse_copy_window)(int mouse_x, int mouse_y);

// implemented on folder_icon.c

/**
 * @brief Inicialize folder icon from home screen with an image and size
 *
 * @param size Size of the icon and text
 * @param img Image of the icon
 */
void(set_folder_icon)(uint16_t size, Image32 *img);
/**
 * @brief Check if mouse is hover folder icon
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 * @return True if mouse is hover folder icon and false otherwise
 */
bool(check_mouse_in_folder)(int mouse_x, int mouse_y);
/**
 * @brief Draw folder icon on the home screen
 *
 * @param size Size of the icon
 */
void(draw_folder_icon)(uint16_t size);
/**
 * @brief Call if size has changed, updating variables related with icon's size
 */
void(set_size_folder)();

// implemented on background_info.c

/** @brief Draws the current date, day of the week and time in the background image
 *
 * @param size The size of the text
 */
void(draw_background_info)(uint16_t size);

#endif
