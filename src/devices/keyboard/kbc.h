#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <stdint.h>

/** @brief + Break Code */
#define KBC_BREAK_PLUS 0x9a
/** @brief - Break Code */
#define KBC_BREAK_MINUS 0xb5
/** @brief TAB Break Code*/
#define KBC_BREAK_TAB 0x8f
/** @brief R Break Code*/
#define KBC_RENAME_FILE 0x93
/** @brief E Break Code*/
#define KBC_DELETE_FILE 0x92
/** @brief F Break Code*/
#define KBC_CREATE_FILE 0xa1
/** @brief D Break Code*/
#define KBC_CREATE_DIRECTORY 0xa0
/** @brief ENTER Make Code*/
#define KBC_MAKE_ENTER 0x1c
/** @brief Arrow Up Make Code*/
#define KBC_MAKE_UP 0x48
/** @brief Arrow Down Make Code*/
#define KBC_MAKE_DOWN 0x50
/** @brief Arrow Left Make Code*/
#define KBC_MAKE_LEFT 0x4b
/** @brief Arrow Right Make Code*/
#define KBC_MAKE_RIGHT 0x4d
/** @brief DELETE Make Code*/
#define KBC_MAKE_DELETE 0x0e
/** @brief BACKSPACE Make Code*/
#define KBC_MAKE_BACKSPACE 0x0e
/** @brief C Make Code*/
#define KBC_CONTROL_C 0x2e
/** @brief V Make Code*/
#define KBC_CONTROL_V 0x2f
/** @brief X Make Code*/
#define KBC_CONTROL_X 0x2d
/** @brief M Make Code*/
#define KBC_MAXIMIZE_WINDOW 0x32
/** @brief J Make Code*/
#define KBC_MINIMIZE_WINDOW 0x24
/** @brief CAPS Make Code*/
#define KBC_MAKE_CAPS 0x3a
/** @brief CAPS Break Code*/
#define KBC_BREAK_CAPS 0xba
/** @brief SHIFT Make Code*/
#define KBC_MAKE_SHIFT 0x2a
/** @brief SHIFT Break Code*/
#define KBC_BREAK_SHIFT 0xaa
/** @brief CONTROL Make Code*/
#define KBC_MAKE_CONTROL 0x1d
/** @brief CONTROL Break Code*/
#define KBC_BREAK_CONTROL 0x9d
/** @brief ESC Make Code*/
#define KBC_MAKE_ESC 0x01
/** @brief ESC Break Code*/
#define KBC_BREAK_ESC 0x81

/** @brief Array of bytes that form the scancode that is being read */
extern uint8_t scancode[2];

/** @brief Number of bytes read to one scancode */
extern uint8_t scancode_count;

/**
 * @brief Subscribes keyboard interrupts
 *
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_unsubscribe_int)();
/**
 * @brief Read scandode from keyboard
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_scancode)();
/**
 * @brief Read byte from keyboard
 *
 * @param byte address where byte read will be stored
 * @return Return 0 upon success and non-zero otherwise
 */
int(kbc_read_byte)(uint8_t *byte);

#endif
