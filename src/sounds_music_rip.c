#include <stdio.h>
#include <stdlib.h>

#include "quiver_ripper.h"
#include "sounds_music_rip.h"

extern char path[];
extern char *name;

extern sound_fd_t snd_fd[SND_ENTRIES];
extern sound_fd_t mus_fd[MUS_ENTRIES];

void rip_sounds_music(FILE *stream){
    BYTE buffer[40357];     /* 40357 is the greatest entry size (66.DWD more specifically) */

    unsigned int i;
    FILE *out_fp;

    /* rip music */
    fseek(stream, mus_fd[0].offset, SEEK_SET);
    for(i = 0; i < MUS_ENTRIES; ++i){
        fread(buffer, 1, mus_fd[i].size, stream);

        sprintf(name, "/music/%u.DWM", i + 1);
        if((out_fp = fopen(path, "wb")) == NULL){
            fprintf(stderr, "Couldn't create %s\n", path);
            exit(EXIT_FAILURE);
        }

        fwrite(buffer, 1, mus_fd[i].size, out_fp);
        fclose(out_fp);
    }

    /* rip sounds */
    fseek(stream, snd_fd[0].offset, SEEK_SET);
    for(i = 0; i < SND_ENTRIES; ++i){
        fread(buffer, 1, snd_fd[i].size, stream);

        sprintf(name, "/sounds/%u.DWD", i + 1);
        if((out_fp = fopen(path, "wb")) == NULL){
            fprintf(stderr, "Couldn't create %s\n", path);
            exit(EXIT_FAILURE);
        }

        fwrite(buffer, 1, snd_fd[i].size, out_fp);
        fclose(out_fp);
    }

}
