#ifndef FILEINFO_H
#define FILEINFO_H

#include <sys/stat.h>
#include <time.h>

#define MAX_FILES 256

/**
 * @file fileinfo.h
 * @brief ファイル1件分の情報
 */

/**
 * @brief ファイル1件分の情報
 */
struct File {
    char name[256];        /**< ファイル名 */
    struct stat st;        /**< stat() で取得した属性 */
    struct timespec mtime; /**< 更新時刻 */
    struct timespec ctime; /**< 状態変更時刻 */
    struct timespec atime; /**< 最終アクセス時刻 */
};

#endif
