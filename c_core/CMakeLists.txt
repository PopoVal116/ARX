cmake_minimum_required(VERSION 3.10)
project(MusicCoreC)

include_directories(include)

add_library(core STATIC 
    src/file_handler.c 
    src/sort.c
    src/binary_search.c
    src/optimal_tree.c
)

#libcore.a