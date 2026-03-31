#include "optimal_tree.h"
#include "sort.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    char author[100];
    char genre[50];
    int weight;
    Song **song_ptrs;
} CompositeStatsNode;

static CompositeStatsNode* collect_composite(Song **sorted_songs, int numSongs, int *unique_count)
{
    if (numSongs <= 0) return NULL;

    CompositeStatsNode *stats = malloc(numSongs * sizeof(CompositeStatsNode));
    if (!stats) return NULL;

    int count = 0;

    for (int i = 0; i < numSongs; i++) {
        Song *curr = sorted_songs[i];

        if (count > 0 &&
            strcmp(stats[count - 1].author, curr->author) == 0 &&
            strcmp(stats[count - 1].genre, curr->genre) == 0) {

            int w = stats[count - 1].weight;
            stats[count - 1].song_ptrs = realloc(stats[count - 1].song_ptrs, (w + 1) * sizeof(Song*));
            if (stats[count - 1].song_ptrs)
                stats[count - 1].song_ptrs[w] = curr;
            stats[count - 1].weight++;
        } else {
            strcpy(stats[count].author, curr->author);
            strcpy(stats[count].genre, curr->genre);
            stats[count].weight = 1;
            stats[count].song_ptrs = malloc(sizeof(Song*));
            if (stats[count].song_ptrs)
                stats[count].song_ptrs[0] = curr;
            count++;
        }
    }

    *unique_count = count;
    return realloc(stats, count * sizeof(CompositeStatsNode));
}

static ResultCompositeTreeNode* build_composite_tree(CompositeStatsNode *stats, int start, int end)
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

    ResultCompositeTreeNode *node = malloc(sizeof(ResultCompositeTreeNode));
    if (!node) return NULL;

    strcpy(node->author, stats[best].author);
    strcpy(node->genre, stats[best].genre);
    node->weight = stats[best].weight;
    node->song_ptrs = stats[best].song_ptrs;
    node->record_count = stats[best].weight;

    node->left  = build_composite_tree(stats, start, best - 1);
    node->right = build_composite_tree(stats, best + 1, end);

    return node;
}

ResultCompositeTreeNode* build_optimal_tree_by_author_genre(Song **songs, int numSongs)
{
    if (numSongs <= 0 || !songs) return NULL;

    Song **temp = malloc(numSongs * sizeof(Song*));
    if (!temp) return NULL;

    for (int i = 0; i < numSongs; i++) temp[i] = songs[i];

    quick_sort_by_author_genre_asc(temp, 0, numSongs - 1);

    int unique_cnt;
    CompositeStatsNode *stats = collect_composite(temp, numSongs, &unique_cnt);
    if (!stats) {
        free(temp);
        return NULL;
    }

    ResultCompositeTreeNode *root = build_composite_tree(stats, 0, unique_cnt - 1);

    free(stats);
    free(temp);
    return root;
}

static void collect_composite_matches(ResultCompositeTreeNode *node,
                                      const char *author, const char *genre,
                                      Song **results, int max_results, int *found)
{
    if (!node || *found >= max_results) return;

    int cmp_author = strcmp(node->author, author);
    int cmp_genre  = (cmp_author == 0) ? strcmp(node->genre, genre) : 0;

    if (cmp_author == 0 && cmp_genre == 0) {
        for (int i = 0; i < node->record_count && *found < max_results; i++) {
            results[*found] = node->song_ptrs[i];
            (*found)++;
        }
    }
    else if (cmp_author < 0 || (cmp_author == 0 && cmp_genre < 0)) {
        collect_composite_matches(node->right, author, genre, results, max_results, found);
    }
    else {
        collect_composite_matches(node->left, author, genre, results, max_results, found);
    }
}

int find_songs_by_author_genre(Song *songs, int count, const char *author, const char *genre, Song **results, int max_results)
{
    if (count <= 0 || !songs || !author || !genre || !results || max_results <= 0)
        return 0;

    Song **ptrs = malloc(count * sizeof(Song*));
    if (!ptrs) return 0;

    for (int i = 0; i < count; i++)
        ptrs[i] = &songs[i];

    quick_sort_by_author_genre_asc(ptrs, 0, count - 1);

    int unique_cnt;
    CompositeStatsNode *stats = collect_composite(ptrs, count, &unique_cnt);

    if (!stats) {
        free(ptrs);
        return 0;
    }

    ResultCompositeTreeNode *root = build_composite_tree(stats, 0, unique_cnt - 1);

    int found = 0;
    collect_composite_matches(root, author, genre, results, max_results, &found);

    free(stats);
    free_optimal_composite_tree(root);
    free(ptrs);

    return found;
}

void free_optimal_composite_tree(ResultCompositeTreeNode *node)
{
    if (!node) return;

    free_optimal_composite_tree(node->left);
    free_optimal_composite_tree(node->right);

    if (node->song_ptrs) {
        free(node->song_ptrs);
        node->song_ptrs = NULL;
    }
    free(node);
}