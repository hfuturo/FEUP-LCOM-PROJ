#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/** @brief KBC IRQ Line */
#define IRQ_KEYBOARD 1
/** @brief Bit that is set if it's a break code */
#define KBC_BREAK_CODE BIT(7)
/** @brief First byte of a two byte scancode */
#define KBC_TWO_BYTES 0xE0
/** @brief Enable interrupts from keyboard */
#define KBC_ENABLE_INT BIT(0)
/** @brief Bit set if there is a time out error */
#define KBC_TIMEOUT_ERROR BIT(6)
/** @brief Bit set if there is a parity error */
#define KBC_PARITY_ERROR BIT(7)
/** @brief State of the output buffer */
#define KBC_FULL_OUT_BUFFER BIT(0)
/** @brief State of the input buffer */
#define KBC_FULL_IN_BUFFER BIT(1)
/** @brief Address of the status register of the KBC */
#define KBC_STATUS_REG 0x64
/** @brief Address to send commands to the KBC */
#define KBC_IN_CMD 0x64
/** @brief Address to write command arguments to the KBC */
#define KBC_OUT_CMD 0x60
/** @brief Read KBC Command Byte */
#define KBC_READ_CMD 0x20
/** @brief Write KBC Command Byte */
#define KBC_WRITE_CMD 0x60
/** @brief Response delay of the KBC */
#define KBC_WAIT 20000
/** @brief Maximum number of attempts before timeout */
#define KBC_MAX_ATTEMPS 10
/** @brief Disable Keyboard */
#define KBC_DISABLE_KEYBOARD BIT(4)

#endif
