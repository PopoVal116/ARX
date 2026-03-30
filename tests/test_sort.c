#include <stdio.h>
#include <string.h>
#include "../c_core/include/record.h"
#include "../c_core/include/sort.h"

int test_sort_asc(void) {
    Song songs[5] = {
        {"Queen", "Bohemian Rhapsody", "", "", "", ""},
        {"John Lennon", "Imagine", "", "", "", ""},
        {"Eagles", "Hotel California", "", "", "", ""},
        {"Led Zeppelin", "Stairway to Heaven", "", "", "", ""},
        {"The Beatles", "Yesterday", "", "", "", ""}
    };

    Song* ptrs[5];
    sort_songs_by_title_asc(songs, 5, ptrs);

    const char *expected[] = {
        "Bohemian Rhapsody",
        "Hotel California",
        "Imagine",
        "Stairway to Heaven",
        "Yesterday"
    };

    int ok = 1;
    for (int i = 0; i < 5; i++) {
        if (strcmp(ptrs[i]->song, expected[i]) != 0) {
            printf("sort_asc failed at %d: expected '%s', got '%s'\n", 
                   i, expected[i], ptrs[i]->song);
            ok = 0;
        }
    }
    if (ok)
        printf("+ sort_songs_by_title_asc passed\n");
    else
        printf("- sort_songs_by_title_asc FAILED\n");
    return ok ? 0 : 1;
}

int test_sort_desc(void) {
    Song songs[5] = {
        {"Queen", "Bohemian Rhapsody", "", "", "", ""},
        {"John Lennon", "Imagine", "", "", "", ""},
        {"Eagles", "Hotel California", "", "", "", ""},
        {"Led Zeppelin", "Stairway to Heaven", "", "", "", ""},
        {"The Beatles", "Yesterday", "", "", "", ""}
    };

    Song* ptrs[5];
    sort_songs_by_title_desc(songs, 5, ptrs);

    const char *expected[] = {
        "Yesterday",
        "Stairway to Heaven",
        "Imagine",
        "Hotel California",
        "Bohemian Rhapsody"
    };

    int ok = 1;
    for (int i = 0; i < 5; i++) {
        if (strcmp(ptrs[i]->song, expected[i]) != 0) {
            printf("sort_desc failed at %d: got '%s'\n", i, ptrs[i]->song);
            ok = 0;
        }
    }
    if (ok) {
        printf("+ sort_songs_by_title_desc passed\n");
        printf("------------------------------------------------\n");
    } else {
        printf("- sort_songs_by_title_desc FAILED\n");
    }
    return ok ? 0 : 1;
}

int test_sort_all(void) {
    printf("Тестирование сортировки:\n");
    return test_sort_asc() + test_sort_desc();
    
}
