#include <dirent.h>
#include <ftw.h>
#include <lcom/lcf.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "directory.h"
#include "file.h"

int(create_directory)(char *dir_name) {
  if (!dir_name) {
    printf("Invalid argument\n");
    return 1;
  }

  DIR *dir = opendir(dir_name);

  if (dir) {
    printf("Folder with %s name already exists\n", dir_name);
    closedir(dir);
    return 1;
  }

  // RWX permissions for owner
  mkdir(dir_name, S_IRWXU);
  return 0;
}

// https://stackoverflow.com/questions/5467725/how-to-delete-a-directory-and-its-contents-in-posix-c
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
  int rv = remove(fpath);

  if (rv)
    perror(fpath);

  return rv;
}

int(delete_directory)(char *dir_name) {
  if (!dir_name) {
    printf("Invalid argument\n");
    return 1;
  }

  return nftw(dir_name, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

int(change_directory)(char *old, char *new) {
  if (!old || !new) {
    printf("Invalid arguments\n");
    return 1;
  }

  DIR *dir = opendir(new);
  if (dir) {
    printf("%s folder already exists\n", new);
    closedir(dir);
    return 1;
  }

  dir = opendir(old);

  if (!dir) {
    printf("%s folder does not exist\n", old);
    return 1;
  }

  closedir(dir);

  if (rename(old, new) == -1) {
    printf("Error renaming/moving directory\n");
    return 1;
  }

  return 0;
}

int(rename_directory)(char *old_dir_name, char *new_dir_name) {
  if (change_directory(old_dir_name, new_dir_name)) {
    printf("Error renaming directory\n");
    return 1;
  }

  return 0;
}

int(move_directory)(char *old_path, char *new_path) {
  if (change_directory(old_path, new_path)) {
    printf("Error moving directory\n");
    return 1;
  }

  return 0;
}

char tmp_old_dir[PATH_MAX];
char tmp_new_dir[PATH_MAX];
int(copy_directory)(char *old_path, char *new_path) {
  printf("copying dir:\n%s\n%s\n", old_path, new_path);
  DIR *d;
  struct dirent *dir;
  d = opendir(old_path);
  if (!d) {
    printf("Error opening directory %s. %s\n", old_path, strerror(errno));
    return 1;
  }
  create_directory(new_path);
  while ((dir = readdir(d)) != NULL) {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
      continue;

    sprintf(tmp_old_dir, "%s/%s", old_path, dir->d_name);
    sprintf(tmp_new_dir, "%s/%s", new_path, dir->d_name);
    if (dir->d_type == DT_DIR) {
      char *tmp_old_dir_keep = strdup(tmp_old_dir);
      char *tmp_new_dir_keep = strdup(tmp_new_dir);
      copy_directory(tmp_old_dir_keep, tmp_new_dir_keep);
      free(tmp_old_dir_keep);
      free(tmp_new_dir_keep);
    }
    else {
      copy_file(tmp_old_dir, tmp_new_dir);
    }
  }
  if (closedir(d) == -1) {
    printf("Error closing directory.\n");
    return 1;
  }

  return 0;
}
