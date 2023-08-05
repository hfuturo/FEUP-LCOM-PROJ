#include <lcom/lcf.h>

#include "rtc.h"

static int hook_id = -1;
bool darkMode = false;
REAL_TIME current_time;

void(rtc_init)() {
  rtc_clear_interrupts();
  rtc_update_interrupts();
  background_theme_handler();
  rtc_get_real_time(&current_time);
}

int(rtc_subscribe_int)(uint8_t *bit_no) {
  if (!bit_no)
    return 1;
  hook_id = *bit_no;
  return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &hook_id);
}

int(rtc_unsubcsribe_int)() {
  if (hook_id < 1)
    return 1;
  return sys_irqrmpolicy(&hook_id);
}

void(rtc_ih)() {
  int cause;
  uint8_t regC;

  sys_outb(RTC_ADDR_REG, REGISTER_C);
  cause = util_sys_inb(RTC_DATA_REG, &regC);

  if ((regC & RTC_AF) && cause == 0) {
    background_theme_handler();
  }

  if ((regC & RTC_UF) && cause == 0) {
    rtc_get_real_time(&current_time);
  }
}

int(rtc_set_alarm)(uint8_t hour, uint8_t minute, uint8_t second) {
  uint8_t regB;

  if (read_RTC_output(REGISTER_B, &regB))
    return 1;

  if (write_RTC_command(REGISTER_B, regB | RTC_AIE))
    return 1;

  if (~regB & RTC_DM) {
    if (hour != DONT_CARE_VALUE)
      hour = binary_to_bcd(hour);
    if (minute != DONT_CARE_VALUE)
      minute = binary_to_bcd(minute);
    if (second != DONT_CARE_VALUE)
      second = binary_to_bcd(second);
  }

  if (write_RTC_command(HOUR_ALARM, hour))
    return 1;
  if (write_RTC_command(MINUTE_ALARM, minute))
    return 1;
  if (write_RTC_command(SECOND_ALARM, second))
    return 1;

  return 0;
}

void(rtc_clear_interrupts)() {
  uint8_t trash;
  read_RTC_output(REGISTER_C, &trash);
}

int(rtc_get_real_time)(REAL_TIME *real_time) {
  if (!real_time)
    return 1;

  uint8_t output;
  bool is_binary = rtc_binary();

  if (!rtc_valid_time()) {
    printf("Internal clock is not working.\n");
    return 1;
  }

  while (rtc_updating()) tickdelay(micros_to_ticks(1));

  if (read_RTC_output(SECOND, &output))
    return 1;
  real_time->second = is_binary ? output : bcd_to_binary(output);

  if (read_RTC_output(MINUTE, &output))
    return 1;
  real_time->minute = is_binary ? output : bcd_to_binary(output);

  if (read_RTC_output(HOUR, &output))
    return 1;
  real_time->hour = is_binary ? output : bcd_to_binary(output);

  if (read_RTC_output(DAY_OF_MONTH, &output))
    return 1;
  real_time->day = is_binary ? output : bcd_to_binary(output);

  if (read_RTC_output(MONTH, &output))
    return 1;
  real_time->month = is_binary ? output : bcd_to_binary(output);

  if (read_RTC_output(YEAR, &output))
    return 1;
  real_time->year = is_binary ? output : bcd_to_binary(output);

  if (read_RTC_output(DAY_OF_WEAK, &output))
    return 1;
  uint8_t day_of_weak = is_binary ? output : bcd_to_binary(output);

  switch (day_of_weak) {
    case 0: strcpy(real_time->weekday, "Sunday"); break;
    case 1: strcpy(real_time->weekday, "Monday"); break;
    case 2: strcpy(real_time->weekday, "Tuesday"); break;
    case 3: strcpy(real_time->weekday, "Wednesday"); break;
    case 4: strcpy(real_time->weekday, "Thursday"); break;
    case 5: strcpy(real_time->weekday, "Friday"); break;
    case 6: strcpy(real_time->weekday, "Saturday"); break;
    default: break;
  }

  return 0;
}

int(read_RTC_output)(uint8_t reg, uint8_t *output) {
  if (!output)
    return 1;
  if (sys_outb(RTC_ADDR_REG, reg))
    return 1;
  if (util_sys_inb(RTC_DATA_REG, output))
    return 1;
  return 0;
}

int(write_RTC_command)(uint8_t reg, uint8_t command) {
  if (sys_outb(RTC_ADDR_REG, reg))
    return 1;
  if (sys_outb(RTC_DATA_REG, command))
    return 1;
  return 0;
}

int(rtc_updating)() {
  uint8_t output;
  if (read_RTC_output(REGISTER_A, &output))
    return 1;
  return output & RTC_UIP;
}

int(rtc_valid_time)() {
  uint8_t output;
  if (read_RTC_output(REGISTER_D, &output))
    return 1;
  return output & RTC_VRT;
}

int(rtc_binary)() {
  uint8_t output;
  if (read_RTC_output(REGISTER_B, &output))
    return 1;
  return output & RTC_DM;
}

uint8_t(bcd_to_binary)(uint8_t number) {
  return (number & 0xF) + (((number >> 4) & 0xF) * 10);
}

uint8_t(binary_to_bcd)(uint8_t number) {
  return ((number / 10) << 4) | (number % 10);
}

void(background_theme_handler)() {
  REAL_TIME real_time;
  if (rtc_get_real_time(&real_time))
    return;

  // darkMode = real_time.second >= 20 || real_time.second < 8;
  darkMode = real_time.hour >= 20 || real_time.hour < 8;

  rtc_change_alarms();
}

void(rtc_change_alarms)() {
  // darkMode ? rtc_set_alarm(DONT_CARE_VALUE, DONT_CARE_VALUE, 8) : rtc_set_alarm(DONT_CARE_VALUE, DONT_CARE_VALUE, 20);
  darkMode ? rtc_set_alarm(8, 0, 0) : rtc_set_alarm(20, 0, 0);
}

int(rtc_update_interrupts)() {
  uint8_t regB;
  if (read_RTC_output(REGISTER_B, &regB))
    return 1;
  if (write_RTC_command(REGISTER_B, regB | RTC_UIE))
    return 1;
  return 0;
}
