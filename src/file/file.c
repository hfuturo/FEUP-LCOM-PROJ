#include <lcom/lcf.h>

#include "file.h"
#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> // for access

#include "../utils.h"

CopyList f_data = {0, 0, 0, 0};

int(create_file)(char *file_name) {
  if (!file_name) {
    printf("Error -> invalid file\n");
    return 1;
  }

  if (access(file_name, F_OK) == 0) {
    printf("Error, file already exists.\n");
    return 1;
  }

  FILE *file = fopen(file_name, "w");
  if (!file) {
    printf("Can not open file %s\n", file_name);
    return 1;
  }

  if (fclose(file)) {
    printf("Error creating file\n");
    return 1;
  }
  return 0;
}

int(delete_file)(char *file_name) {
  if (!file_name) {
    printf("Error -> invalid file\n");
    return 1;
  }

  if (access(file_name, F_OK) != 0) {
    printf("Error, file %s does not exist.\n", file_name);
    return 1;
  }

  if (remove(file_name)) {
    printf("Error deleting file\n");
    printf("%s\n", strerror(errno));
    return 1;
  }
  return 0;
}
char ch[1024 * 100];

void(copy_file_worker)() {
  FILE_DATA *current_copy = &f_data.items[f_data.begin];
  int c = fread(ch, 1, 1024 * 100, current_copy->file_origin);
  fwrite(ch, 1, c, current_copy->file_dest);
  current_copy->current_position += c;

  if (c == 0) {
    free(current_copy->path_origin);
    free(current_copy->path_dest);

    if (fclose(current_copy->file_dest)) {
      printf("Error closing new_file\n");
    }

    if (fclose(current_copy->file_origin)) {
      printf("Error closing old_file\n");
    }

    queue_pop(&f_data);

    if (f_data.count > 0) {
      current_copy = &f_data.items[f_data.begin];

      current_copy->file_origin = fopen(current_copy->path_origin, "r");
      if (!current_copy->file_origin) {
        printf("Error opening file old_file %s %s\n", current_copy->path_origin, strerror(errno));
      }

      current_copy->file_dest = fopen(current_copy->path_dest, "w");
      if (!current_copy->file_dest) {
        printf("Error opening file new_file %s %s\n", current_copy->path_dest, strerror(errno));
      }
    }
  }
}

int(copy_file)(char *old_file, char *new_file) {
  FILE_DATA file;
  file.current_position = 0;

  if (!old_file || !new_file) {
    printf("Error -> invalid files\n");
    return 1;
  }

  file.path_origin = strdup(old_file);
  file.path_dest = strdup(new_file);

  file.file_origin = fopen(file.path_origin, "r");
  if (!file.file_origin) {
    printf("Error opening file old_file %s %s\n", file.path_origin, strerror(errno));
    free(file.path_origin);
    free(file.path_dest);
    return 1;
  }

  file.file_dest = fopen(file.path_dest, "w");
  if (!file.file_dest) {
    printf("Error opening file new_file %s %s\n", file.path_dest, strerror(errno));
    if (fclose(file.file_origin)) {
      printf("Error closing old_file\n");
    }
    free(file.path_origin);
    free(file.path_dest);
    return 1;
  }

  fseek(file.file_origin, 0, SEEK_END);
  file.file_size = ftell(file.file_origin);
  rewind(file.file_origin);

  if (f_data.count > 0) {
    if (fclose(file.file_dest)) {
      printf("Error closing new_file\n");
    }

    if (fclose(file.file_origin)) {
      printf("Error closing old_file\n");
    }

    file.file_dest = NULL;
    file.file_origin = NULL;
  }
  queue_append(&f_data, file);

  return 0;
}

int(change_file)(char *old_file, char *new_file) {
  if (!old_file || !new_file) {
    printf("Inavalid arguments.\n");
    return 1;
  }

  if ((access(old_file, F_OK)) != 0) {
    printf("File %s does not exist.\n", old_file);
    return 1;
  }

  if (rename(old_file, new_file) == -1) {
    printf("Error renaming/moving file.\n");
    return 1;
  }
  return 0;
}

int(rename_file)(char *file_name, char *new_name) {
  return change_file(file_name, new_name);
}

int(move_file)(char *old_path, char *new_path) {
  return change_file(old_path, new_path);
}
