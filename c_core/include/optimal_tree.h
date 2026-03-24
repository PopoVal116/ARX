#ifndef OPTIMAL_TREE_H
#define OPTIMAL_TREE_H

#include "record.h"

typedef struct ResultSongTreeNode {
    char song[100];              
    int weight;
    Song *song_ptr;             
    struct ResultSongTreeNode *left;
    struct ResultSongTreeNode *right;
} ResultSongTreeNode;

ResultSongTreeNode* build_optimal_song_tree(Song **songs, int numSongs);
Song* search_in_optimal_song_tree(ResultSongTreeNode *root, const char *song_title);
void free_optimal_song_tree(ResultSongTreeNode *node);

Song* find_song_by_title(Song *songs, int count, const char *title);

#endif