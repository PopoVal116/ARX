#ifndef SORT_H
#define SORT_H

#include "record.h"

void quick_sort_by_title_asc(Song **arr, int low, int high);
void quick_sort_by_title_desc(Song **arr, int low, int high);

void sort_songs_by_title_asc(Song *songs, int count, Song **ptrs);
void sort_songs_by_title_desc(Song *songs, int count, Song **ptrs);

void quick_sort_by_author_genre_asc(Song **arr, int low, int high);

#endif