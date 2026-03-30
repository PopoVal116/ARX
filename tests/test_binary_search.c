#include <stdio.h>
#include <string.h>
#include "../c_core/include/record.h"
#include "../c_core/include/binary_search.h"

int test_binary_search(void) {
    Song songs[6] = {
        {"John Lennon", "Imagine", "", "", "", ""},
        {"Eagles", "Hotel California", "", "", "", ""},
        {"Ariana Grande", "Imagine", "", "", "", ""},
        {"Queen", "Bohemian Rhapsody", "", "", "", ""},
        {"The Beatles", "Yesterday", "", "", "", ""},
        {"John Lennon", "Imagine", "", "", "", ""}
    };

    Song* results[5];
    int found = binary_search_song(songs, 6, "Imagine", results, 5);

    if (found != 3) {
        printf(" - binary_search_song failed: expected 3, got %d\n", found);
        return 1;
    }

    printf("+ binary_search_song passed (found %d matches for 'Imagine')\n", found);
    return 0;
}

int test_binary_search_all(void) {
    printf("Тестирование бинарного поиска:\n");
    return test_binary_search();
    printf("------------------------------------------------\n");
}
