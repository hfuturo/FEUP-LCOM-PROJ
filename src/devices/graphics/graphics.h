#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_
#include "image.h"
#include "lcom/vbe.h"
#include "pixel.h"
#include <stdbool.h>
#include <stdint.h>
/**
 * @brief Limits of the screen
 */
typedef struct {
  int xmin;
  int ymin;
  int xmax;
  int ymax;
} Bounds;

/**
 * @brief An area of the screen
 *
 */
typedef struct {
  int offset_x, offset_y;
  Image24 window;
} Window;

/**
 * @brief Stack of windows
 *
 */
typedef struct {
  Window *items;
  int count;
  int capacity;
} WindowStack;

/**
 * @brief Current window and all it's parents
 */
extern WindowStack windows;

/**
 * @brief Color black
 */
extern pixel24 black;
/**
 * @brief Color white
 */
extern pixel24 white;
/**
 * @brief Color green
 */
extern pixel24 green;
/**
 * @brief Color darkgreen
 */
extern pixel24 darkgreen;
/**
 * @brief Color grey
 */
extern pixel24 grey;
/**
 * @brief Color red
 */
extern pixel24 red;

/**
 * @brief current drawing window
 */
extern Image24 window;
/**
 * @brief back buffer as an image representing the entire screen
 */
extern Image24 screen;

/**
 * @brief Map video memory
 *
 * @param mode Vbe mode
 * @return Return 0 upon success and 1 otherwise
 */
int(set_frame_buffer)(uint16_t mode);
/**
 * @brief Initialize graphics mode
 * @param mode Graphic mode choosen
 * @return Return 0 upon success and 1 otherwise
 */
int(set_graphic_mode)(uint16_t mode);
/**
 * @brief Draw retangle fill with color
 *
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param width Width of retangle
 * @param height Height of retangle
 * @param color Color of retangle
 */
void(draw_rect)(int, int, uint16_t, uint16_t, pixel24);
/**
 * @brief Draw image
 *
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param width Width of image
 * @param height Height of image
 * @param img image to be draw
 */
void(draw_image)(int x, int y, uint16_t width, uint16_t height, Image32 img);
/**
 * @brief Draw retangle outline with color
 *
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param width Width of retangle
 * @param height Height of retangle
 * @param color Color of retangle's outline
 * @param thickness Size of the outline's thickness
 */
void(draw_outline_rect)(int x, int y, uint16_t width, uint16_t height, pixel24 color, uint16_t thickness);
/**
 * @brief Draw text
 *
 * @param text Text to be draw
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param size Size of the text
 * @param color Text's color
 */
void(draw_text)(char *text, int x, int y, uint16_t size, pixel24 color);
/**
 * @brief Draw text with a diferent background color in utf8
 *
 * @param text Text to be draw
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param size Size of the text
 * @param foregroudColor Text's color
 * @param backgroundColor Background color
 */
void(draw_text_with_background)(char *text, int x, int y, uint16_t size, pixel24 foregroudColor, pixel24 backgroundColor);
/**
 * @brief Draw text with a diferent background in utf32
 *
 * @param text Text to be draw
 * @param x x-position of upper-left corner
 * @param y y-position of upper-left corner
 * @param size Size of the text
 * @param foregroudColor Text's color
 * @param backgroundColor Background color
 */
void(draw_parsed_text_with_background)(int *text, int x, int y, uint16_t size, pixel24 foregroudColor, pixel24 backgroundColor);

/**
 * @brief Switch displaying buffer
 */
int(flip_page)();

/**
 * @brief Convert window coordinates to screen coordinates
 *
 * @param x x-coordinate of the point in the window
 * @param y y-coordinate of the point in the window
 * @param out_x x-coordinate of the point in the screen
 * @param out_y y-coordinate of the point in the screen
 * @param win coresponding window
 */
void(window_to_screen)(int x, int y, int *out_x, int *out_y, Window win);
/**
 * @brief Convert screnn coordinates to window coordinates
 *
 * @param x x-coordinate of the point in the scree
 * @param y y-coordinate of the point in the scree
 * @param out_x x-coordinate of the point in the window
 * @param out_y y-coordinate of the point in the window
 * @param win coresponding window
 */
void(screen_to_window)(int x, int y, int *out_x, int *out_y, Window win);
/**
 * @brief Push a new window into the windows stack and make it the current window
 *
 * @param x x-offset from the parent window
 * @param y y-offset from the parent window
 * @param width width of the new window
 * @param height height of the new window
 *
 * @return the newly created Window
 */
Window(push_window)(int x, int y, int width, int height);
/**
 * @brief pop current window from the windows stack changing the current window to it's parent
 */
void(pop_window)();
/**
 * @brief check if the mouse is inside a given window
 *
 * @param mouse_x x-coordinate of the mouse in screen coordinates
 * @param mouse_y y-coordinate of the mouse in screen coordinates
 * @param win window to test
 *
 * @return if the mouse is inside the given window
 */
bool(mouse_inside_window)(int mouse_x, int mouse_y, Window win);

#endif
