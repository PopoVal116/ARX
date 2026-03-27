#include "binary_search.h"
#include <string.h>

int binary_search_by_title(Song **arr, int n, const char *title, int find_first) {
    int low = 0;
    int high = n - 1;
    int res = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(arr[mid]->song, title);

        if (cmp == 0) {
            res = mid; 
            if (find_first) {
                high = mid - 1; 
            } else {
                low = mid + 1;  
            }
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}