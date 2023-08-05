#ifndef _LCOM_PIXEL_H_
#define _LCOM_PIXEL_H_
#include <stdint.h>

#pragma pack(1)

/** @brief Pixel structure of 24 bits
 *
 *  @param b Byte representing the blue color
 *  @param g Byte representing the green color
 *  @param r Byte representing the red color
 */
typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} pixel24;

/** @brief Pixel structure of 32 bits
 *
 *  @param r Byte representing the red color
 *  @param b Byte representing the blue color
 *  @param g Byte representing the green color
 *  @param a Byte representing the alpha component (transparency)
 */
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} pixel32;

#pragma options align = reset

#endif
