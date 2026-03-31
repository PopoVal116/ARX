#ifndef OPTIMAL_TREE_H
#define OPTIMAL_TREE_H

#include "record.h"

typedef struct ResultCompositeTreeNode {
    char author[100];
    char genre[50];
    int weight;
    Song **song_ptrs;
    int record_count;
    struct ResultCompositeTreeNode *left;
    struct ResultCompositeTreeNode *right;
} ResultCompositeTreeNode;

/* ====================== ВЫСОКОУРОВНЕВЫЕ ФУНКЦИИ ====================== */

/**
 * Строит оптимальное дерево по составному ключу (author, genre)
 * Вызывать один раз после загрузки базы данных
 */
ResultCompositeTreeNode* build_optimal_composite_tree(Song *songs, int count);

/**
 * Быстрый поиск песен по точной паре автор + жанр
 * Использует уже построенное дерево — очень быстро
 */
int search_by_author_and_genre(ResultCompositeTreeNode *tree,
                               const char *author,
                               const char *genre,
                               Song **results,
                               int max_results);

/**
 * Освобождение всего дерева
 * Вызывать при завершении программы
 */
void free_optimal_composite_tree(ResultCompositeTreeNode *node);

#endif