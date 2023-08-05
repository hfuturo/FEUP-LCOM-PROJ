#include <lcom/lcf.h>

#include "image.h"
#define STBI_NO_FAILURE_STRINGS
#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image32 loadImageFromFile(char *filePath) {
  int width, height, comp;
  stbi_uc *data = stbi_load(filePath, &width, &height, &comp, 4);
  Image32 img = {
    .data = (pixel32 *) data,
    .width = width,
    .height = height,
    .stride = width,
  };
  if (data == NULL) {
    return img;
  }
  return img;
}

Image32(subImage32)(Image32 img, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
  Image32 i = {
    .data = &ImageAt(img, x, y),
    .width = width,
    .height = height,
    .stride = img.stride,
  };
  return i;
}

Image24(subImage24)(Image24 img, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
  Image24 i = {
    .data = &ImageAt(img, x, y),
    .width = width,
    .height = height,
    .stride = img.stride,
  };
  return i;
}

void(freeImage)(Image32 img) {
  stbi_image_free(img.data);
}
