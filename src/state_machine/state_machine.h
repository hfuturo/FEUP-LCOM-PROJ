#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_
#include "ui/menu.h"
#include "ui/ui.h"

/** @brief State of the program */
enum State {
  NORMAL_KBC,
  NORMAL_MOUSE,
  RENAME,
  NEW_FILE,
  NEW_DIRECTORY,
  OPEN_IMAGE,
};
/** @brief Diferent operations to do with control + v */
enum Control_V_Operation {
  NOTHING,
  COPYING,
  MOVING,
};

/** @brief Tells what is the window being used */
enum ActiveWindow {
  LIST_FILES_ACTIVE,
  COPY_WINDOW_ACTIVE,
};

/** @brief Window that is being used at the moment */
extern enum ActiveWindow active_window;

/** @brief Last state of the program (kbc or mouse) */
extern enum State last_state;
/** @brief Current state of the program */
extern enum State state;
/** @brief Gives information about what is the operation that should be done with control + v */
extern enum Control_V_Operation control_v_operation;
/** @brief True if control key is being pressed and false otherwise */
extern bool control;
/** @brief True if shift key is being pressed and false otherwise */
extern bool shift;
/** @brief True if caps key is active and false otherwise */
extern bool caps;
/** @brief True if enter key is being pressed and false otherwise */
extern bool enter;
/** @brief Menu used to the actions on the file list window with the mouse*/
extern Menu *menuFile;
/** @brief Set if there are changes to be performed on the screen since the last time */
extern bool has_changed;

/** @brief Perform create new file action */
void(commit_new_file)();
/** @brief Perform create new directory action */
void(commit_new_dir)();
/** @brief Perform rename file/directory action */
void(commit_rename)();

/** @brief Change state of the program to NEW_DIRECTORY and state of the mouse to NEW_DIRECTORY_ */
void process_new_dir();
/** @brief Change state of the program to RENAME and state of the mouse to RENAME_ */
void process_rename();
/** @brief Change state of the program to NEW_FILE and state of the mouse to NEW_FILE_ */
void process_new_file();
/** @brief Call function that opens the directory selected */
void process_open();
/** @brief Call function that deletes the file or directory selected */
void process_delete();
/** @brief Sets control_v_operation to COPYING and and calls the function that saves the information of the content selected*/
void process_control_c();
/** @brief Does the operating depending if control_v_operation is set to COPYING or MOVING */
void process_control_v();
/** @brief Sets control_v_operation to MOVING and and calls the function that saves the information of the content selected*/
void process_control_x();

/**
 * @brief Process key pressed
 *
 * @param size Size of the text
 * @return Return 0 in sucess, 1 with unsucess or 2 when it's to close the program
 *
 */
int process_keyboard_state(uint16_t *size);
/**
 * @brief Process keys that work the same with kbc or mouse
 *
 * @param size Size of the text
 * @return Return 0 in sucess, 1 with unsucess or 2 when it's to close the program
 *
 */
int(process_kbc_mouse_common)(uint16_t *size);
/**
 * @brief Process special keys when writing name of file/folder
 *
 * @return Return 0 upon success and 1 if user goes back to NORMAl state
 */
int process_keyboard_write();
/**
 * @brief Process special keys that are independent of the state
 *
 * @return Return 0 upon success and 1 otherwise
 */
int process_keyboard_common();
/**
 * @brief Process mouse event
 *
 * @param mouse_x Position of the x component of the mouse
 * @param mouse_y Position of the y component of the mouse
 * @param event Last mouse event
 * @param size Size of used to draw
 * @return Return 0 upon success and 1 otherwise
 */
int(process_mouse_state)(int mouse_x, int mouse_y, struct mouse_ev event, uint16_t size);
/**
 * @brief Draw content that is dependent of the state
 *
 * @param size Size of the text
 * @return Return 0 upon success and 1 otherwise
 */
int(process_draw)(uint16_t size);

/**
 * @brief Setup the right click menu
 */
void makeMenus();

#endif
