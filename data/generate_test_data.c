#include "../c_core/include/record.h"
#include <stdio.h>
#include <string.h>

void fill_song(Song* s, const char* auth, const char* title, const char* alb, const char* gen, const char* dur, const char* img) {
    memset(s, 0, sizeof(Song));
    strncpy(s->author, auth, 99);
    strncpy(s->song, title, 99);
    strncpy(s->album, alb, 99);
    strncpy(s->genre, gen, 49);
    strncpy(s->duration, dur, 19);
    strncpy(s->image_path, img, 199);
}

int main() {
    Song db[22];

    fill_song(&db[0], "The Weeknd", "Blinding Lights", "After Hours", "Synthwave", "3:20", "covers/cover1.png");
    fill_song(&db[1], "Dua Lipa", "Don't Start Now", "Future Nostalgia", "Disco-Pop", "3:03", "covers/cover2.png");
    fill_song(&db[2], "Imagine Dragons", "Believer", "Evolve", "Alt Rock", "3:24", "covers/cover3.png");
    fill_song(&db[3], "Billie Eilish", "Bad Guy", "WWAFAWDWG", "Electropop", "3:14", "covers/cover4.png");
    fill_song(&db[4], "Eminem", "Rap God", "MMLP2", "Hip-Hop", "6:04", "covers/cover5.png");
    fill_song(&db[5], "Queen", "Bohemian Rhapsody", "A Night at the Opera", "Rock", "5:55", "covers/cover6.png");
    fill_song(&db[6], "Nirvana", "Smells Like Teen Spirit", "Nevermind", "Grunge", "5:01", "covers/cover7.png");
    fill_song(&db[7], "Daft Punk", "Get Lucky", "RAM", "Disco", "4:08", "covers/cover8.png");
    fill_song(&db[8], "Linkin Park", "In the End", "Hybrid Theory", "Nu Metal", "3:36", "covers/cover9.png");
    fill_song(&db[9], "Arctic Monkeys", "Do I Wanna Know?", "AM", "Indie Rock", "4:32", "covers/cover10.png");
    fill_song(&db[10], "Bon Jovi", "It's My Life", "Crush", "Hard Rock", "3:44", "covers/cover11.png");
    fill_song(&db[11], "Coldplay", "Yellow", "Parachutes", "Alt Rock", "4:29", "covers/cover12.png");
    fill_song(&db[12], "Depeche Mode", "Enjoy the Silence", "Violator", "Synth-pop", "4:15", "covers/cover13.png");
    fill_song(&db[13], "Gorillaz", "Feel Good Inc.", "Demon Days", "Alt Hip-Hop", "3:41", "covers/cover14.png");
    fill_song(&db[14], "Metallica", "Enter Sandman", "Metallica", "Heavy Metal", "5:31", "covers/cover15.png");
    fill_song(&db[15], "Michael Jackson", "Thriller", "Thriller", "Pop", "5:57", "covers/cover16.png");
    fill_song(&db[16], "Radiohead", "Creep", "Pablo Honey", "Alt Rock", "3:56", "covers/cover17.png");
    fill_song(&db[17], "The Killers", "Mr. Brightside", "Hot Fuss", "Indie Rock", "3:42", "covers/cover18.png");
    fill_song(&db[18], "Rammstein", "Sonne", "Mutter", "Industrial", "4:32", "covers/cover19.png");
    fill_song(&db[19], "AC/DC", "Back In Black", "Back In Black", "Hard Rock", "4:15", "covers/cover20.png");
    fill_song(&db[20], "Abba", "Dancing Queen", "Arrival", "Pop", "3:50", "covers/cover21.png");
    fill_song(&db[21], "Zemfira", "Arrivederci", "Zemfira", "Rock", "3:41", "covers/cover22.png");

    FILE *f = fopen("data/music_db.dat", "wb");
    if (f) {
        fwrite(db, sizeof(Song), 22, f);
        fclose(f);
        printf("Success: 'data/music_db.dat' created with 22 records.\n");
    } else {
        perror("Error creating file");
    }
    return 0;
}