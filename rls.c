#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    static char stdout_buf[8192];
    //ファイル一覧を収集
    struct File files[MAX_FILES];
    int counter;
    //標準出力のバッファリングを設定
    setvbuf(stdout, stdout_buf, _IOFBF, sizeof(stdout_buf));
    //タイムゾーンを設定
    tzset();
    //ファイル一覧を収集
    counter = collect_files(files, MAX_FILES);
    if (counter < 0) {
        return 1;
    }
    //ファイルを新しい順にソート
    qsort(files, (size_t)counter, sizeof(struct File), compare_mtime);
    //ファイル一覧を表示
    print_files(files, counter);
    //正常終了
    return 0;
}
