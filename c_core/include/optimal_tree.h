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

ResultCompositeTreeNode* build_optimal_tree_by_author_genre(Song **songs, int numSongs);

int find_songs_by_author_genre(Song *songs, int count, const char *author, const char *genre, Song **results, int max_results);

void free_optimal_composite_tree(ResultCompositeTreeNode *node);

#endif