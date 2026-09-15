#ifndef SORT_H
#define SORT_H

/**
 * @file sort.h
 * @brief ファイル一覧の並び替え
 */

/**
 * @brief qsort 用の比較関数（更新時間が新しい順）
 * @param a 比較対象のファイルA
 * @param b 比較対象のファイルB
 * @return Aが古いとき 1、Aが新しいとき -1、同じなら 0
 */
int compare_mtime(const void *a, const void *b);

#endif
