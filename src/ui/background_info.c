
#include "devices/graphics/graphics.h"
#include "devices/rtc/rtc.h"
#include <stdio.h>
#include <string.h>

void(draw_background_info)(uint16_t size) {
  char hours[255], day[255];
  sprintf(hours, "%02d:%02d:%02d", current_time.hour, current_time.minute, current_time.second);
  sprintf(day, "%02d/%02d/20%02d", current_time.day, current_time.month, current_time.year);
  draw_text(hours, window.width / 2 - 4 * 8 * size, window.height / 2 - 13 * size, size, white);
  draw_text(current_time.weekday, window.width / 2 - strlen(current_time.weekday) / 2 * 8 * size, window.height / 2 - 4 * size, size, white);
  draw_text(day, window.width / 2 - 5 * 8 * size, window.height / 2 + 5 * size, size, white);
}
