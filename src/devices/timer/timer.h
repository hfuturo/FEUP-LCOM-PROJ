#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

/**
 * @brief Subscribes and enables Timer 0 interrupts
 * 
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and 1 otherwise
*/
int(timer_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes Timer 0 interrupts
 * 
 * @return Return 0 upon success and 1 otherwise
*/
int(timer_unsubscribe_int)();

#endif
