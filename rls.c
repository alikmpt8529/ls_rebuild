#include <stdio.h>
#include <stdlib.h>

#include "fileinfo.h"
#include "listing.h"
#include "sort.h"

/**
 * @file rls.c
 * @brief カレントディレクトリのファイル一覧を ls 風に表示する
 */

/**
 * @brief カレントディレクトリを読み、新しい順に一覧表示する
 * @return 正常終了なら 0
 */
int main(void) {
    struct File files[MAX_FILES];
    int counter;

    counter = collect_files(files, MAX_FILES);
    if (counter < 0) {
        return 1;
    }
    qsort(files, (size_t)counter, sizeof(struct File), compare_mtime);
    print_files(files, counter);
    return 0;
}
