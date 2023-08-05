#ifndef _LCOM_FILE_H_
#define _LCOM_FILE_H_

#include <stdio.h>

/**
 * @brief Data structure that represents all the data related to copying a file.
 * 
 * @param file_size The size of the file
 * @param current_position The current position of the copy
 * @param path_origin The name of the file that is going to be copied
 * @param path_dest The name of the new file
 * @param file_origin The current file
 * @param file_dest The new file
*/
typedef struct FILE_DATA {
  uint64_t file_size;
  uint64_t current_position;
  char *path_origin;
  char *path_dest;
  FILE *file_origin;
  FILE *file_dest;
} FILE_DATA;

/**
 * @brief Data structure that represents a queue of items that are waiting to be copied
 * 
 * @param items The queue of items to be copied
 * @param count The current number of items that are waiting to be copied
 * @param capacity The maximum number of items that can be copied
 * @param begin The beginning of the queue
*/
typedef struct {
  FILE_DATA *items;
  int count;
  int capacity;
  int begin;
} CopyList;

/** @brief Queue of items waiting to be copied */
extern CopyList f_data;

/**
 * @brief Create new file
 *
 * @param file_name Name of new file
 * @return Return 0 upon success and 1 otherwise
 */
int(create_file)(char *file_name);
/**
 * @brief Delete file
 *
 * @param file_name Name of the file to delete
 * @return Return 0 upon success and 1 otherwise
 */
int(delete_file)(char *file_name);
/**
 * @brief Copy file
 *
 * @param old_file Path of the file that will be copy
 * @param new_file Name of the new file
 * @return Return 0 upon success and 1 otherwise
 */
int(copy_file)(char *old_file, char *new_file);
/**
 * @brief Auxiliar function to copy a file
 */
void(copy_file_worker)();
/**
 * @brief Rename existing file
 *
 * @param file_name Old name of the file
 * @param new_name New name of the file
 * @return Return 0 upon success and 1 otherwise
 */
int(rename_file)(char *file_name, char *new_name);
/**
 * @brief Move existing file
 *
 * @param old_path Old path of the file
 * @param new_path New path of the file
 * @return Return 0 upon success and 1 otherwise
 */
int(move_file)(char *old_path, char *new_path);

#endif
