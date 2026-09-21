#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fileinfo.h"
#include "listing.h"
#include "sort.h"

/**
 * @file rls.c
 * @brief 指定ディレクトリ（省略時はカレント）のファイル一覧を ls 風に表示する
 */

/**
 * @brief ディレクトリを読み、新しい順に一覧表示する
 * @param argc 引数の数
 * @param argv 引数（argv[1] があれば対象パス）
 * @return 正常終了なら 0
 */
int main(int argc, char *argv[]) {
    static char stdout_buf[8192];
    struct File files[MAX_FILES];
    int counter;
    const char *path = ".";

    if (argc >= 2) {
        path = argv[1];
    }

    setvbuf(stdout, stdout_buf, _IOFBF, sizeof(stdout_buf));
    tzset();
    // ファイル情報を収集
    counter = collect_files(files, MAX_FILES, path);
    if (counter < 0) {
        return 1;
    }
    // ファイル情報をソート
    qsort(files, (size_t)counter, sizeof(struct File), compare_mtime);
    print_files(files, counter);
    return 0;
}
