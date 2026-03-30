#include <stdio.h>

int test_sort_all(void);
int test_binary_search_all(void);
int test_tree_all(void);

int main(void) {
    printf("------------------------------------------------\n");
    printf("Unit Tests \n\n");

    int failed = 0;
    failed += test_sort_all();
    failed += test_binary_search_all();
    failed += test_tree_all();

    printf("\n*ИТОГ* \n");
    if (failed == 0)
        printf("Все UNIT-тесты прошли УСПЕШНО!\n");
    else
        printf("Провалено тестов: %d\n", failed);

    return failed;
}