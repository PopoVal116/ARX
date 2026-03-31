#include "sort.h"
#include <string.h>
#include <stdlib.h>

void swap(Song **a, Song **b) {
    Song *t = *a; *a = *b; *b = t;
}

int partition_title_asc(Song **arr, int low, int high) {
    char *pivot = arr[high]->song;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j]->song, pivot) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort_by_title_asc(Song **arr, int low, int high) {
    if (low < high) {
        int pi = partition_title_asc(arr, low, high);
        quick_sort_by_title_asc(arr, low, pi - 1);
        quick_sort_by_title_asc(arr, pi + 1, high);
    }
}

int partition_title_desc(Song **arr, int low, int high) {
    char *pivot = arr[high]->song;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j]->song, pivot) >= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort_by_title_desc(Song **arr, int low, int high) {
    if (low < high) {
        int pi = partition_title_desc(arr, low, high);
        quick_sort_by_title_desc(arr, low, pi - 1);
        quick_sort_by_title_desc(arr, pi + 1, high);
    }
}

void sort_songs_by_title_asc(Song *songs, int count, Song **ptrs)
{
    if (count <= 0 || !songs || !ptrs) return;

    for (int i = 0; i < count; i++)
        ptrs[i] = &songs[i];

    quick_sort_by_title_asc(ptrs, 0, count - 1);
}

void sort_songs_by_title_desc(Song *songs, int count, Song **ptrs)
{
    if (count <= 0 || !songs || !ptrs) return;

    for (int i = 0; i < count; i++)
        ptrs[i] = &songs[i];

    quick_sort_by_title_desc(ptrs, 0, count - 1);
}

int compare_author_genre(const Song *a, const Song *b) {
    int res = strcmp(a->author, b->author);
    if (res != 0) return res;
    return strcmp(a->genre, b->genre);
}

int partition_author_genre_asc(Song **arr, int low, int high) {
    Song *pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (compare_author_genre(arr[j], pivot) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort_by_author_genre_asc(Song **arr, int low, int high) {
    if (low < high) {
        int pi = partition_author_genre_asc(arr, low, high);
        quick_sort_by_author_genre_asc(arr, low, pi - 1);
        quick_sort_by_author_genre_asc(arr, pi + 1, high);
    }
}