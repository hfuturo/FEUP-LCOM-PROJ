#ifndef _LCOM_MENU_H_
#define _LCOM_MENU_H_

#include "devices/graphics/graphics.h"
#include "ui.h"
#include <stdint.h>
#include <stdio.h>

struct menu;
struct menu_entry;
/** @brief Main menu */
typedef struct menu Menu;
/** @brief Entry of the menu */
typedef struct menu_entry MenuEntry;

/**
 * @brief Menu of presented when lb pressed in mouse
 * @param items pointer to array of menu entries
 * @param count number of menu entries
 * @param capacity capacity of array
 */
struct menu {
  MenuEntry **items;
  int count;
  int capacity;
};

/**
 * @brief Menu entry
 * @param desc descriptive text of the entry
 * @param func pointer to a function executed when the user clicks in this option
 */
struct menu_entry {
  char *desc;
  void (*func)();
};

/**
 * @brief Menu constructor
 */
Menu *newMenu();
/**
 * @brief Add new entry to a menu
 *
 * @param m pointer to a menu
 * @param desc descriptive text of the new entry
 * @param f pointer to the function that will be called when clicked on action
 */
void menuAddFunction(Menu *m, char *desc, void (*f)(void));
/**
 * @brief Checks what was the option chosen if it was chosen any
 *
 * @param m pointer to a menu
 * @param x x-position of upper-left corner of the menu
 * @param y y-position of upper-left corner of the menu
 * @param mouse_x x-position of upper-left corner of the mouse
 * @param mouse_y y-position of upper-left corner of the mouse
 * @param size size of the text
 */
bool(menuPost)(Menu *m, int x, int y, int mouse_x, int mouse_y, uint16_t size);
/**
 * @brief Draw Menu
 *
 * @param m menu to be drawn
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param backgroundColor background color of the menu
 * @param foregroundColor foreground color of the menu
 * @param size size of the text
 */
void(draw_menu)(Menu *m, int x, int y, pixel24 backgroundColor, pixel24 foregroundColor, uint16_t size);

#endif
