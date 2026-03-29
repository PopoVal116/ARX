#ifndef SONG_H
#define SONG_H

#ifdef __cplusplus 
extern "C" {
#endif

typedef struct Song{
    char author[100];
    char song[100];
    char album[100];
    char genre[50];
    char duration[20];
    char image_path[200];
} Song;

#ifdef __cplusplus
}
#endif

#endif