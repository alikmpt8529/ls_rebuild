#include "listing.h"
#include "perm.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/xattr.h>
#include <time.h>
#include <unistd.h>

/**
 * @file listing.c
 * @brief ディレクトリの読み込みと表示
 */
int collect_files(struct File *files, int max) {
    DIR *dir;
    struct dirent *file;
    int counter = 0;
    int dfd;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return -1;
    }

    dfd = dirfd(dir);
    while (counter < max && (file = readdir(dir)) != NULL) {
        //ドットファイルをスキップ
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
            continue;
        }
        //ファイル情報を取得
        if (fstatat(dfd, file->d_name, &files[counter].st, 0) != 0) {
            continue;
        }
        //ファイル名をコピー
        strncpy(files[counter].name, file->d_name, sizeof(files[counter].name) - 1);
        files[counter].name[sizeof(files[counter].name) - 1] = '\0';
        files[counter].mtime = files[counter].st.st_mtimespec;
        files[counter].ctime = files[counter].st.st_ctimespec;
        //アクセス時刻をコピー
        files[counter].atime = files[counter].st.st_atimespec;
        //拡張属性があるかどうかをチェック
        files[counter].has_xattr =
        listxattr(files[counter].name, NULL, 0, XATTR_NOFOLLOW) > 0;
        counter++;
    }
    closedir(dir);
    return counter;
}

/**
 * @brief ファイルサイズをフォーマットする
 * @param bytes ファイルサイズ
 * @param out フォーマット後の文字列
 * @param out_size フォーマット後の文字列のサイズ
 */
static void format_size(off_t bytes, char *out, size_t out_size) {
    const char *unit[] = {"B", "KB", "MB", "GB", "TB"};
    double n = (double)bytes;
    int i = 0;
    const int unit_count = 5;
    while (n >= 1024.0 && i < unit_count - 1) {
        n /= 1024.0;
        i++;
    }
    if (i == 0) {
        snprintf(out, out_size, "%d%s", (int)bytes, unit[i]);
    } else {
        snprintf(out, out_size, "%.1f%s", n, unit[i]);
    }
}
/**
 * @brief ファイル一覧を表示する
 * @param files ファイル一覧
 * @param count ファイル数
 */
void print_files(const struct File *files, int count) {
    for (int i = 0; i < count; i++) {
        char perm[12];
        char time_str[32];
        char size_str[16];
        struct tm tm;

        format_perm(files[i].st.st_mode, files[i].has_xattr, perm);
        format_size(files[i].st.st_size, size_str, sizeof(size_str));
        if (localtime_r(&files[i].mtime.tv_sec, &tm) == NULL) {
            snprintf(time_str, sizeof(time_str), "%s", "---- -- -- --:--");
        } else {
            snprintf(time_str, sizeof(time_str), "%04d-%02d-%02d %02d:%02d",
                     tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                     tm.tm_hour, tm.tm_min);
        }
        printf("%s %s %s %8s\n", perm, files[i].name, time_str, size_str);
    }
}
