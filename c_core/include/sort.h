#ifndef SORT_H
#define SORT_H

#include "record.h"

#ifdef __cplusplus 
extern "C" {
#endif

void quick_sort_by_title_asc(Song **arr, int low, int high);
void quick_sort_by_title_desc(Song **arr, int low, int high);

void sort_songs_by_title_asc(Song *songs, int count, Song **ptrs);
void sort_songs_by_title_desc(Song *songs, int count, Song **ptrs);

#ifdef __cplusplus
}
#endif

#endif