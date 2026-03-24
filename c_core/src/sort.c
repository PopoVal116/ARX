#include "sort.h"
#include <string.h>

void swap(Song **a, Song **b) {
    Song *t = *a; *a = *b; *b = t;
}

int partition_author_asc(Song **arr, int low, int high) {
    char *pivot = arr[high]->author;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j]->author, pivot) <= 0) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort_by_author_asc(Song **arr, int low, int high) {
    if (low < high) {
        int pi = partition_author_asc(arr, low, high);
        quick_sort_by_author_asc(arr, low, pi - 1);
        quick_sort_by_author_asc(arr, pi + 1, high);
    }
}

int partition_author_desc(Song **arr, int low, int high) {
    char *pivot = arr[high]->author;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (strcmp(arr[j]->author, pivot) >= 0) { 
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort_by_author_desc(Song **arr, int low, int high) {
    if (low < high) {
        int pi = partition_author_desc(arr, low, high);
        quick_sort_by_author_desc(arr, low, pi - 1);
        quick_sort_by_author_desc(arr, pi + 1, high);
    }
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