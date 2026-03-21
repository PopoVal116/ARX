#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "record.h"

int load_database(const char *filename, Song *array, int max_records);

int save_database(const char *filename, Song *array, int count);

int backup_database(const char *source_file, const char *backup_file);

#endif