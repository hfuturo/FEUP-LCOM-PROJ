#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>

extern uint8_t byte_index;
extern bool error;

extern int mouse_x;
extern int mouse_y;

/*
enum mouse_ev_t { LB_PRESSED,
                  LB_RELEASED,
                  RB_PRESSED,
                  RB_RELEASED,
                  BUTTON_EV,
                  MOUSE_MOV };

struct mouse_ev {
  enum mouse_ev_t type;
  int16_t delta_x, delta_y;
};
*/

/**
 * @brief Subscribes mouse interrupts
 *
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes mouse interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupted handler 
*/
void(mouse_ih)();

/**
 * @brief Write a command to mouse 
 * 
 * @param port address of buffer/port here command will be write
 * @param cmd Mouse command 
*/
int(mouse_write_cmd)(uint8_t port, uint8_t cmd);

/**
 * @brief Read a byte to mouse from KBC output buffer 
 * 
 * @param port address of buffer/port from where data is read 
 * @param byte address where byte read will be stored
 * 
*/
int(mouse_read_byte)(uint8_t port, uint8_t *byte);

/**
 * @brief Update position of mouse in screen
 * 
 * @param res_x screen size resolution x 
 * @param res_y screen size resolution y
*/
void(mouse_update)(uint16_t res_x, uint16_t res_y);

/**
 * @brief Fills and Synchronizes an array of three bytes read
 *
 * @return Return 0 upon success and non-zero otherwise
 * 
*/
int(kbc_read_packet)();

/**
 * @brief Fills the packet structure from the three-byte read array
*/
void(mouse_construct_packet)();

/**
 * @brief Enable/Disable Data Reporting in Steam mode 
 * 
 * @param cmd Mouse command to Enable or Disable Data Reporting
 * @return Return 0 upon success and non-zero otherwise
*/
int(mouse_data_reporting)(uint8_t cmd);

// int(compare_buttons)();

/**
 * @brief Processe mouse buttons changes
 * 
 * @return struct mouse_ev with mouse movements and changes in buttons state
*/
struct mouse_ev(mouse_event)();

#endif
