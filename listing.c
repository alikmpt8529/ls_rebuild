#include "listing.h"
#include "perm.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**
 * @file listing.c
 * @brief ディレクトリの読み込みと表示
 */
int collect_files(struct File *files, int max) {
    DIR *dir;
    struct dirent *file;
    int counter = 0;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    while (counter < max && (file = readdir(dir)) != NULL) {
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
            continue;
        }
        if (stat(file->d_name, &files[counter].st) != 0) {
            continue;
        }
        strncpy(files[counter].name, file->d_name, sizeof(files[counter].name) - 1);
        files[counter].name[sizeof(files[counter].name) - 1] = '\0';
        files[counter].mtime = files[counter].st.st_mtimespec;
        files[counter].ctime = files[counter].st.st_ctimespec;
        files[counter].atime = files[counter].st.st_atimespec;
        counter++;
    }
    closedir(dir);
    return counter;
}

/**
 * @brief ファイル一覧を表示する
 * @param files ファイル一覧
 * @param count ファイル数
 */
void print_files(const struct File *files, int count) {
    for (int i = 0; i < count; i++) {
        char perm[12];
        format_perm(files[i].name, files[i].st.st_mode, perm);
        printf("%s %s %s", perm, files[i].name, ctime(&files[i].st.st_mtime));
    }
}
