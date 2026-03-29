#include "binary_search.h"
#include "sort.h"
#include <string.h>
#include <stdlib.h>

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

int binary_search_song(Song *songs, int count, const char *title, Song **results, int max_results)
{
    if (count <= 0 || !songs || !title || !results || max_results <= 0)
        return 0;

    Song **ptrs = malloc(count * sizeof(Song*));
    if (!ptrs) return 0;

    for (int i = 0; i < count; i++)
        ptrs[i] = &songs[i];

    quick_sort_by_title_asc(ptrs, 0, count - 1);

    int L = binary_search_by_title(ptrs, count, title, 1);  
    int R = binary_search_by_title(ptrs, count, title, 0); 

    if (L == -1) {
        free(ptrs);
        return 0;
    }

    int found = 0;
    for (int i = L; i <= R && found < max_results; i++) {
        results[found++] = ptrs[i];
    }

    free(ptrs);
    return found;
}