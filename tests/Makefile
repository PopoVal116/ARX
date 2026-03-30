CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g

INCLUDES = -I../c_core/include

CORE_SRC = ../c_core/src/sort.c \
           ../c_core/src/binary_search.c \
           ../c_core/src/optimal_tree.c

TEST_SRC = run_tests.c \
           test_sort.c \
           test_binary_search.c \
           test_tree.c

test: $(TEST_SRC) $(CORE_SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o run_tests $^
	@echo "\n unit-тесты"
	./run_tests

clean:
	rm -f run_tests

.PHONY: test clean