#ifndef SORT_H
#define SORT_H

#include "record.h"

void quick_sort_by_author_asc(Song **arr, int low, int high);
void quick_sort_by_author_desc(Song **arr, int low, int high);

void quick_sort_by_title_asc(Song **arr, int low, int high);
void quick_sort_by_title_desc(Song **arr, int low, int high);

#endif