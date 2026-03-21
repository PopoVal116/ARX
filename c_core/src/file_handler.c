#include "../include/file_handler.h"
#include <stdio.h>

int load_database(const char *filename, Song *array, int max_records) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        return -1;
    }
    int count = fread(array, sizeof(Song), max_records, file);
    fclose(file);
    return count;
}

int save_database(const char *filename, Song *array, int count) {
    FILE *file = fopen(filename, "wb");
    if (!file) return -1;
    int written = fwrite(array, sizeof(Song), count, file);
    fclose(file);
    return written;
}

int backup_database(const char *source_file, const char *backup_file) {
    Song temp[100];
    int count = load_database(source_file, temp, 100);
    if (count > 0) {
        return save_database(backup_file, temp, count);
    }
    return -1;
}