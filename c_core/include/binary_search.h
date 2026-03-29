#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include "record.h"

#ifdef __cplusplus 
extern "C" {
#endif

int binary_search_by_title(Song **arr, int n, const char *title, int find_first);

int binary_search_song(Song *songs, int count, const char *title, Song **results, int max_results);

#ifdef __cplusplus
}
#endif

#endif