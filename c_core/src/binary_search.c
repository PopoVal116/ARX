#include "binary_search.h"
#include <string.h>

int binary_search_by_title(Song **arr, int n, const char *title) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(arr[mid]->song, title);
        if (cmp == 0) return mid;
        if (cmp < 0) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}