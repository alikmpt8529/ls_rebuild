#include "listing.h"
#include "perm.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief ディレクトリ配下のファイルサイズを合計する
 * @param dirfd 親ディレクトリの fd
 * @param name  対象ディレクトリ名
 * @return 中身の合計バイト数
 */
static off_t dir_total_size(int dirfd, const char *name) {
    int fd;
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    off_t total = 0;

    fd = openat(dirfd, name, O_RDONLY | O_DIRECTORY);
    if (fd < 0) {
        return 0;
    }
    dir = fdopendir(fd);
    if (dir == NULL) {
        close(fd);
        return 0;
    }
    // ディレクトリ内のファイルを読み込む
    while ((ent = readdir(dir)) != NULL) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }
        if (fstatat(fd, ent->d_name, &st, AT_SYMLINK_NOFOLLOW) != 0) {
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            total += dir_total_size(fd, ent->d_name);
        } else {
            total += st.st_size;
        }
    }
    // ディレクトリを閉じる
    closedir(dir);
    return total;
}

/**
 * @brief ディレクトリパスと名前を結合する
 */
static void join_path(char *out, size_t out_size, const char *dir, const char *name) {
    size_t len = strlen(dir);
    // ディレクトリパスの末尾が '/' であれば、パスと名前を結合
    if (len > 0 && dir[len - 1] == '/') {
        snprintf(out, out_size, "%s%s", dir, name);
    } else {
        snprintf(out, out_size, "%s/%s", dir, name);
    }
}

/**
 * @file listing.c
 * @brief ディレクトリの読み込みと表示
 */
int collect_files(struct File *files, int max, const char *path) {
    DIR *dir;
    struct dirent *file;
    int counter = 0;
    int dfd;
    // パスが NULL または空の場合は "." を使用
    if (path == NULL || path[0] == '\0') {
        path = ".";
    }
    // ディレクトリを開く
    dir = opendir(path);
    if (dir == NULL) {
        perror(path);
        return -1;
    }
    // ディレクトリの fd を取得
    dfd = dirfd(dir);
    // ディレクトリ内のファイルを読み込む
    while (counter < max && (file = readdir(dir)) != NULL) {
        char full[512];

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
        //拡張属性があるかどうかをチェック（指定ディレクトリ上のパスで調べる）
        join_path(full, sizeof(full), path, files[counter].name);
        // 拡張属性があるかどうかをチェック（指定ディレクトリ上のパスで調べる）
        files[counter].has_xattr =
            listxattr(full, NULL, 0, XATTR_NOFOLLOW) > 0;
        if (S_ISDIR(files[counter].st.st_mode)) {
            files[counter].size = dir_total_size(dfd, files[counter].name);
        } else {
            files[counter].size = files[counter].st.st_size;
        }
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
 */
void print_files(const struct File *files, int count) {
    for (int i = 0; i < count; i++) {
        char perm[12];
        char time_str[32];
        char size_str[16];
        struct tm tm;

        format_perm(files[i].st.st_mode, files[i].has_xattr, perm);
        format_size(files[i].size, size_str, sizeof(size_str));
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
