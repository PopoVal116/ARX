#include <stdio.h>
#include <string.h>
#include "../c_core/include/record.h"
#include "../c_core/include/optimal_tree.h"

void print_tree_inorder(ResultSongTreeNode* node) {
    if (node == NULL) return;

    print_tree_inorder(node->left);

    printf("  %s (weight: %d) -> ", node->song, node->weight);
    for (int i = 0; i < node->record_count && i < 3; i++) { 
        printf("%s", node->song_ptrs[i]->author);
        if (i < node->record_count - 1 && i < 2) printf(", ");
    }
    if (node->record_count > 3) printf("...");
    printf("\n");

    print_tree_inorder(node->right);
}

int test_optimal_tree(void) {
    printf("------------------------------------------------\n");
    printf("Тестирование оптимального дерева:\n");

    Song songs[7] = {
        {"John Lennon", "Imagine", "", "", "", ""},
        {"Eagles", "Hotel California", "", "", "", ""},
        {"Ariana Grande", "Imagine", "", "", "", ""},
        {"Queen", "Bohemian Rhapsody", "", "", "", ""},
        {"The Beatles", "Yesterday", "", "", "", ""},
        {"John Lennon", "Imagine", "", "", "", ""},
        {"Led Zeppelin", "Stairway to Heaven", "", "", "", ""}
    };

    Song* ptrs[7];
    for (int i = 0; i < 7; i++) ptrs[i] = &songs[i];

    ResultSongTreeNode* root = build_optimal_song_tree(ptrs, 7);
    if (!root) {
        printf("- build_optimal_song_tree returned NULL\n");
        return 1;
    }
    printf("+ Дерево построено успешно!\n");
    printf("Корневой элемент: %s (weight: %d)\n\n", root->song, root->weight);

    printf("Обход дерева слева направо:\n");
    printf("------------------------------------------------\n");
    print_tree_inorder(root);
   

    free_optimal_song_tree(root);
    return 0;
}

int test_find_song_by_title_tree(void) {
    Song songs[6] = {
        {"John Lennon", "Imagine", "", "", "", ""},
        {"Eagles", "Hotel California", "", "", "", ""},
        {"Ariana Grande", "Imagine", "", "", "", ""},
        {"Queen", "Bohemian Rhapsody", "", "", "", ""},
        {"The Beatles", "Yesterday", "", "", "", ""},
        {"John Lennon", "Imagine", "", "", "", ""}
    };

    Song* results[6];
    int found = find_song_by_title(songs, 6, "Imagine", results, 6);

    if (found != 3) {
        printf("- find_song_by_title failed: expected 3, got %d\n", found);
        return 1;
    }

    printf("\n+ find_song_by_title passed (найдено %d песен с названием 'Imagine')\n", found);
    return 0;
}

int test_tree_all(void) {
    int failed = 0;
    failed += test_optimal_tree();
    failed += test_find_song_by_title_tree();
    return failed;
}