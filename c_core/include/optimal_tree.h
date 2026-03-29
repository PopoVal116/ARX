#ifndef OPTIMAL_TREE_H
#define OPTIMAL_TREE_H

#include "record.h"

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct ResultSongTreeNode {
    char song[100];              
    int weight;
    Song **song_ptrs;   
    int record_count;          
    struct ResultSongTreeNode *left;
    struct ResultSongTreeNode *right;
} ResultSongTreeNode;

int find_song_by_title(Song *songs, int count, const char *title, Song **results, int max_results);

ResultSongTreeNode* build_optimal_tree_from_songs(Song *songs, int count);
ResultSongTreeNode* build_optimal_song_tree(Song **songs, int numSongs);
void free_optimal_song_tree(ResultSongTreeNode *node);

#ifdef __cplusplus
}
#endif

#endif