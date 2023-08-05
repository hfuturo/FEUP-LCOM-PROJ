#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @brief Mouse IRQ Line */
#define MOUSE_IRQ 12
/** @brief Address of the status register of the KBC */
#define KBC_ST_REG 0x64
/** @brief Address to send commands to the KBC */
#define KBC_CMD_REG 0x64
/** @brief Address of the output buffer */
#define KBC_OUT_BUF 0x60
/** @brief Write KBC Command Byte */
#define KBC_WRITE_CMD 0x60
/** @brief State of the output buffer */
#define KBC_OBF 0x01 //bit(0)
/** @brief State of the input buffer */
#define KBC_IBF 0x02 //bit(1)
/** @brief Bit set if there is a parity error */
#define KBC_PARITY_ERROR BIT(7)
/** @brief Bit set if there is a time out error */
#define KBC_TIMEOUT_ERROR BIT(6)
/** @brief Response delay */
#define DELAY_US    20000

/** @brief Bit set if data coming from the Mouse*/
#define MOUSE_DATA BIT(5)
/** @brief Bit use to synchroniza mouse packet data*/
#define FIRST_BYTE BIT(3)
/** @brief Bit set if mouse left button is press*/
#define MOUSE_LB            BIT(0)
/** @brief Bit set if mouse right button is press*/
#define MOUSE_RB            BIT(1)
/** @brief Bit set if mouse middle button is press*/
#define MOUSE_MB            BIT(2)
/** @brief Bit set if delta x is negative*/
#define MOUSE_X_SIGNAL      BIT(4)
/** @brief Bit set if delta y is negative*/
#define MOUSE_Y_SIGNAL      BIT(5)
/** @brief Bit set if occur overflow in x move*/
#define MOUSE_X_OVERFLOW    BIT(6)
/** @brief Bit set if occur overflow in y move*/
#define MOUSE_Y_OVERFLOW    BIT(7)
/** @brief Command to request forwarding of byte (command) to the mouse without any interpretation*/
#define WRITE_BYTE_MOUSE    0xD4
/** @brief Command to Disable Data Reporting*/
#define Mouse_Disable_Reporting 0xF5
/** @brief Command to Enable Data Reporting*/
#define Mouse_Enable_Reporting  0xF4
/** @brief  Everything OK acknowledgment byte from mouse controller*/
#define MOUSE_ACK 0xFA

#endif 
