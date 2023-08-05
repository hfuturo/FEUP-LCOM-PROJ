#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

typedef union {
  struct info {
    uint8_t bcd : 1;
    uint8_t mode : 3;
    uint8_t initMode : 2;
    uint8_t timer : 2;
  } info;
  uint8_t data;
} TimerConfig;

int counter = 0;
static int hook_id = -1;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < TIMER_MIN_FREQ)
    return 1;
  TimerConfig t;
  if (timer_get_conf(timer, &t.data))
    return 1;
  uint8_t selects[] = {TIMER_SEL0, TIMER_SEL1, TIMER_SEL2};
  uint8_t cmd = 0;
  cmd |= selects[timer];
  cmd |= TIMER_LSB_MSB;
  cmd |= t.info.bcd;
  cmd |= t.info.mode << 1;
  if (sys_outb(TIMER_CTRL, cmd))
    return 1;

  uint16_t counter_init = (uint16_t) (TIMER_FREQ / freq);

  uint8_t lsb = 0, msb = 0;
  if (util_get_LSB(counter_init, &lsb))
    return 1;
  if (util_get_MSB(counter_init, &msb))
    return 1;

  if (sys_outb(TIMER_0 + timer, lsb))
    return 1;
  if (sys_outb(TIMER_0 + timer, msb))
    return 1;

  return 0;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  /* To be implemented by the students */
  hook_id = *bit_no;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int(timer_unsubscribe_int)() {
  if (hook_id < 0)
    return 1;
  return sys_irqrmpolicy(&hook_id);
}

void(timer_int_handler)() {
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL)
    return 1;
  uint32_t readCommand = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
  if (sys_outb(TIMER_CTRL, readCommand))
    return 1;
  if (timer > 2)
    return 1;
  return util_sys_inb(TIMER_0 + timer, st);
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val conf;
  TimerConfig t = *(TimerConfig *) &st;
  switch (field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial:
      conf.in_mode = t.info.initMode;
      break;
    case tsf_mode:
      conf.count_mode = t.info.mode;
      uint8_t firstTwo = conf.count_mode & (BIT(0) | BIT(1));
      if (firstTwo == 2 || firstTwo == 3)
        conf.count_mode &= ~BIT(2);
      break;
    case tsf_base:
      conf.bcd = t.info.bcd;
      break;
    default: return 1;
  }
  return timer_print_config(timer, field, conf);
}
