#ifndef PERM_H
#define PERM_H

#include <sys/stat.h>

/**
 * @file perm.h
 * @brief 権限文字列の変換
 */

/**
 * @brief st_mode を drwxr-xr-x@ 形式の文字列にする
 * @param path 拡張属性の有無を調べるパス
 * @param mode ファイルのモード (st_mode)
 * @param out  結果を書き込むバッファ (12バイト以上)
 */
void format_perm(const char *path, mode_t mode, char *out);

#endif
