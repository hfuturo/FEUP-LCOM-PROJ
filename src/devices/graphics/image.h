#ifndef _LCOM_IMAGE_H_
#define _LCOM_IMAGE_H_
#include "pixel.h"
#include <stdint.h>

/**
 * @brief Struct to define an image without transparency
 *
 * data - pixels of the image
 * width - width of the image
 * height - height of the image
 */
typedef struct {
  pixel24 *data;
  uint32_t width, height, stride;
} Image24;

/**
 * @brief Struct to define an image with transparency
 *
 * @param data - pixels of the image
 * @param width - width of the image
 * @param height - height of the image
 */
typedef struct {
  pixel32 *data;
  uint32_t width, height, stride;
} Image32;

/**
 * @brief get the pixel at coordinates from an image
 *
 * @param img image from witch to get the pixel
 * @param x x-coordinate of the pixel
 * @param y y-coordinate of the pixel
 */
#define ImageAt(img, x, y) (img).data[(x) + (y) * (img).stride]

/**
 * @brief Load image from a file
 *
 * @param filePath Path of where the image is saved
 * @return Image struct of the desired content
 */
Image32(loadImageFromFile)(char *filePath);

/**
 * @brief Creats a sub image from an original image
 *
 * @param img The original image
 * @param x The x-coordinate of the beginning of the image
 * @param y The y-coordinate of the beginning of the image
 * @param width The width of the sub image
 * @param height The height of the sub image
 *
 * @return The sub image
 */
Image24(subImage24)(Image24 img, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

/**
 * @brief Frees image from memory
 *
 * @param img Image to be freed
 */
void(freeImage)(Image32 img);

#endif
