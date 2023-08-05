#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

/*  MACROS AND VARIABLES  */

/** @brief Represents the current theme displayed */
extern bool darkMode;

/** @brief Port which must be loaded with the address of the RTC register to be accessed */
#define RTC_ADDR_REG        0x70

/** @brief Port which is used to transfer the data to/from the RTC's register accessed */
#define RTC_DATA_REG        0x71

/** @brief Represents the IRQ line of the RTC */
#define IRQ_RTC             8

/** @brief Address location of seconds */
#define SECOND              0

/** @brief Address location of seconds alarm */
#define SECOND_ALARM        1

/** @brief Address location of minutes */
#define MINUTE              2

/** @brief Address location of minutes alarm */
#define MINUTE_ALARM        3

/** @brief Address location of hour */
#define HOUR                4

/** @brief Address location of hour alarm */
#define HOUR_ALARM          5

/** @brief Address location of day of the weak */
#define DAY_OF_WEAK         6

/** @brief Address location of date of month */
#define DAY_OF_MONTH        7

/** @brief Address location of month */
#define MONTH               8

/** @brief Address location of year */
#define YEAR                9

/** @brief Address location of register A */
#define REGISTER_A          10

/** @brief Address location of register B */
#define REGISTER_B          11

/** @brief Address location of register C */
#define REGISTER_C          12

/** @brief Address location of register D */
#define REGISTER_D          13 

/** @brief Data structure that represents the current time given by the RTC 
 * 
 * @param seconds The corresponding seconds of the current time
 * @param minute The corresponding minute of the current time
 * @param hour The corresponding hour of the current time
 * @param day The corresponding day of the current time
 * @param month The corresponding month of the current time
 * @param year The corresponding year of the current time
 * @param weekday The corresponding weekday
*/
typedef struct REAL_TIME {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    char weekday[10];
} REAL_TIME;

/** @brief Current time */
extern REAL_TIME current_time;

/* REGISTER_A */

/** @brief If set to 1, update is in progress. Do not access time/date registers */
#define RTC_UIP     BIT(7)

/* REGISTER_B */

/** @brief If set to 1, RTC is operating in binary. If set to 0, RTC is operating in BCD */
#define RTC_DM      BIT(2)

/** @brief Set to 1 to enable interrupts from alarms */
#define RTC_AIE     BIT(5)

/** @brief Set to 1 to enable update interrupts */
#define RTC_UIE     BIT(4)

/* REGISTER_C */

/** @brief If set to 1 an alarm interrupt is pending */
#define RTC_AF      BIT(5)

/** @brief If set to 1 an update interrupt is pending */
#define RTC_UF      BIT(4)

/* REGISTER_D */

/** @brief Valid RAM/time. If set to 0 then RTC readings may be incorrect */
#define RTC_VRT     BIT(7)

/** @brief These values match any value of the corresponding register of the time of day register set */
#define DONT_CARE_VALUE     (BIT(7) | BIT(6))

/* *********************************************************************************** */

/* FUNCTION DECLARATIONS */

/**
 * @brief Subscribes RTC interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int(rtc_subscribe_int)(uint8_t* bit_no);


/**
 * @brief Unsubscribe RTC interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
*/
int(rtc_unsubcsribe_int)();

/** @brief Handles every interrupt generated by RTC */
void(rtc_ih)();

/**
 * @brief Sets a RTC alarm
 * 
 * @param hour The hour of the alarm
 * @param minute The minute of the alarm
 * @param seconds The seconds of the alarm
 * 
 * @return Return 0 upon success or 1 otherwise
*/
int(rtc_set_alarm)(uint8_t hour, uint8_t minute, uint8_t seconds);

/**
 * @brief Gets the current time
 * 
 * @param real_time Data structure to be initialized with the current time
 * 
 * @return Return 0 upon success or 1 otherwise
*/
int(rtc_get_real_time)(REAL_TIME* real_time);

/**
 * @brief Writes a command byte to RTC register
 * 
 * @param reg The register where the command will be written
 * @param command The command that will be written
 * 
 * @return Return 0 upon success or 1 otherwise
*/
int(write_RTC_command)(uint8_t reg, uint8_t command);

/**
 * @brief Reads the output of a register
 * 
 * @param reg The register that will be read
 * @param output Address of memory to be initialized with the output of the register
 * 
 * @return Return 0 upon success or 1 otherwise 
*/
int(read_RTC_output)(uint8_t reg, uint8_t* output);

/** @brief Clears RTC interrupts */
void(rtc_clear_interrupts)();

/**
 * @brief Checks if RTC is updating registers
 * 
 * @return Return 1 if updating or 0 otherwise
*/
int(rtc_updating)();

/**
 * @brief Checks if RTC is operating in binary mode
 * 
 * @return Return 1 if is in binary or 0 otherwise
*/
int(rtc_binary)();

/**
 * @brief Checks if the RTC readings are correct
 * 
 * @return Return 1 if valid time or 0 otherwise
*/
int(rtc_valid_time)();

/**
 * @brief Converts a BCD number to binary
 * 
 * @param number BCD number to be converted
 * 
 * @return Returns the binary number
*/
uint8_t(bcd_to_binary)(uint8_t number);

/**
 * @brief Converts a binary number to BCD
 * 
 * @param number Binary number to be converted
 * 
 * @return Returns the BCD number
*/
uint8_t(binary_to_bcd)(uint8_t number);

/** @brief Sets an alarm to the corresponding time of the day. */
void(rtc_change_alarms)();

/** @brief Sets the initial background color */
void(background_theme_handler)();

/** @brief Enables updating interrupts */
int(rtc_update_interrupts)();

/** @brief Initializes RTC. Resets interrupts, sets interrupts and sets the current background theme */
void(rtc_init)();

#endif