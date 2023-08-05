#ifndef _LCOM_DIRECTORY_H_
#define _LCOM_DIRECTORY_H_

/**
 * @brief Create new directory
 *
 * @param dir_name Name of new directory
 * @return Return 0 upon success and 1 otherwise
 */
int(create_directory)(char *dir_name);
/**
 * @brief Delete directory
 *
 * @param dir_name Name of the directory to delete
 * @return Return 0 upon success and 1 otherwise
 */
int(delete_directory)(char *dir_name);
/**
 * @brief Rename existing directory
 *
 * @param old_dir_name Old name of the directory
 * @param new_dir_name New name of the directory
 * @return Return 0 upon success and 1 otherwise
 */
int(rename_directory)(char *old_dir_name, char *new_dir_name);
/**
 * @brief Move existing directory
 *
 * @param old_path Old path of the directory
 * @param new_path New path of the directory
 * @return Return 0 upon success and 1 otherwise
 */
int(move_directory)(char *old_path, char *new_path);
/**
 * @brief Copy the current directory and all its content
 * 
 * @param old_path The path of the directory that is going to be copied
 * @param new_path The path where we are going to copy the directory
 * @return Return 0 upon success and 1 otherwise
*/
int(copy_directory)(char *old_path, char *new_path);

#endif
