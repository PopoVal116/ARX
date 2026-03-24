#include "optimal_tree.h"
#include "sort.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    char song[100];
    int weight;
    Song *song_ptr;
} ResultSongNode;

static ResultSongNode* collect_songs(Song **sorted_songs, int numSongs, int *count)
{
    *count = numSongs;
    ResultSongNode *stats = malloc(numSongs * sizeof(ResultSongNode));
    if (!stats) return NULL;

    for (int i = 0; i < numSongs; i++) {
        strcpy(stats[i].song, sorted_songs[i]->song);
        stats[i].weight   = 1;       
        stats[i].song_ptr = sorted_songs[i];
    }
    return stats;
}

static ResultSongTreeNode* build_tree(ResultSongNode *stats, int start, int end)
{
    if (start > end) return NULL;

    int total = 0;
    for (int i = start; i <= end; i++) total += stats[i].weight;

    int cum = 0, root_idx = start;
    for (int i = start; i <= end; i++) {
        cum += stats[i].weight;
        if (cum >= (total + 1) / 2) {
            root_idx = i;
            break;
        }
    }

    int best = root_idx, min_diff = INT_MAX;
    for (int c = root_idx - 1; c <= root_idx + 1; c++) {
        if (c < start || c > end) continue;
        int left = 0;
        for (int i = start; i < c; i++) left += stats[i].weight;
        int right = total - left - stats[c].weight;
        int diff = abs(left - right);
        if (diff < min_diff || (diff == min_diff && c == root_idx)) {
            min_diff = diff;
            best = c;
        }
    }

    ResultSongTreeNode *node = malloc(sizeof(ResultSongTreeNode));
    if (!node) return NULL;

    strcpy(node->song, stats[best].song);
    node->weight   = stats[best].weight;
    node->song_ptr = stats[best].song_ptr;
    node->left     = build_tree(stats, start, best - 1);
    node->right    = build_tree(stats, best + 1, end);

    return node;
}

ResultSongTreeNode* build_optimal_song_tree(Song **songs, int numSongs)
{
    if (numSongs <= 0) return NULL;

    Song **temp = malloc(numSongs * sizeof(Song*));
    if (!temp) return NULL;

    for (int i = 0; i < numSongs; i++) temp[i] = songs[i];

    quick_sort_by_song(temp, 0, numSongs - 1);

    int count;
    ResultSongNode *stats = collect_songs(temp, numSongs, &count);
    if (!stats) {
        free(temp);
        return NULL;
    }

    ResultSongTreeNode *tree = build_tree(stats, 0, count - 1);

    free(stats);
    free(temp);
    return tree;
}

Song* search_in_optimal_song_tree(ResultSongTreeNode *root, const char *song_title)
{
    if (!root) return NULL;

    int cmp = strcmp(root->song, song_title);
    if (cmp == 0) return root->song_ptr;
    if (cmp > 0)
        return search_in_optimal_song_tree(root->left, song_title);
    else
        return search_in_optimal_song_tree(root->right, song_title);
}

void free_optimal_song_tree(ResultSongTreeNode *node)
{
    if (!node) return;
    free_optimal_song_tree(node->left);
    free_optimal_song_tree(node->right);
    free(node);
}

static void collect_all_matches(ResultSongTreeNode *node, const char *title, Song **results, int max_results, int *found)
{
    if (!node || *found >= max_results) 
        return;

    int cmp = strcmp(node->song, title);

    if (cmp == 0) {
        results[*found] = node->song_ptr;
        (*found)++;
        
        collect_all_matches(node->left, title, results, max_results, found);
        collect_all_matches(node->right, title, results, max_results, found);
    }
    else if (cmp > 0) {
        collect_all_matches(node->left, title, results, max_results, found);
    }
    else {
        collect_all_matches(node->right, title, results, max_results, found);
    }
}

int find_song_by_title(Song *songs, int count, const char *title,
                       Song **results, int max_results)
{
    if (count <= 0 || !songs || !title || !results || max_results <= 0)
        return 0;

    Song **ptrs = malloc(count * sizeof(Song*));
    if (!ptrs) return 0;

    for (int i = 0; i < count; i++) {
        ptrs[i] = &songs[i];
    }

    quick_sort_by_title_asc(ptrs, 0, count - 1);

    ResultSongTreeNode *root = build_optimal_song_tree(ptrs, count);

    int found = 0;
    collect_all_matches(root, title, results, max_results, &found);

    free_optimal_song_tree(root);
    free(ptrs);

    return found;
}