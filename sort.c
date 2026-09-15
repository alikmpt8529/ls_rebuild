#include "sort.h"
#include "fileinfo.h"

/**
 * @file sort.c
 * @brief ファイル一覧の並び替え
 */
int compare_mtime(const void *a, const void *b) {
    const struct File *fileA = a;
    const struct File *fileB = b;

    if (fileA->mtime.tv_sec < fileB->mtime.tv_sec) {
        return 1;
    }
    if (fileA->mtime.tv_sec > fileB->mtime.tv_sec) {
        return -1;
    }
    if (fileA->mtime.tv_nsec < fileB->mtime.tv_nsec) {
        return 1;
    }
    if (fileA->mtime.tv_nsec > fileB->mtime.tv_nsec) {
        return -1;
    }
    return 0;
}
