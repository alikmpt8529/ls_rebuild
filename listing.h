#ifndef LISTING_H
#define LISTING_H

#include "fileinfo.h"

/**
 * @file listing.h
 * @brief ディレクトリの読み込みと表示
 */

/**
 * @brief カレントディレクトリからファイル情報を集める
 * @param files 結果を入れる配列
 * @param max   配列の最大件数
 * @return 読み込んだ件数。失敗時は -1
 */
int collect_files(struct File *files, int max);

/**
 * @brief ファイル一覧を表示する
 * @param files 表示する配列
 * @param count 件数
 */
void print_files(const struct File *files, int count);

#endif
