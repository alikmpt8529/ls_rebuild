#ifndef PERM_H
#define PERM_H

#include <sys/stat.h>

/**
 * @file perm.h
 * @brief 権限文字列の変換
 */

/**
 * @brief st_mode を drwxr-xr-x@ 形式の文字列にする
 * @param mode      ファイルのモード (st_mode)
 * @param has_xattr 拡張属性があるとき 0 以外
 * @param out       結果を書き込むバッファ (12バイト以上)
 */
void format_perm(mode_t mode, int has_xattr, char *out);

#endif
