#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "record.h"

#ifdef __cplusplus 
extern "C" {
#endif

int load_database(const char *filename, Song *array, int max_records);
int save_database(const char *filename, Song *array, int count);
int backup_database(const char *source_file, const char *backup_file);

#ifdef __cplusplus
}
#endif

#endif 