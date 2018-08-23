#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quiver_ripper.h"
#include "pictures_rip.h"
#include "tga_utils.h"
#include "makedirs.h"
#include "hud_rip.h"
#include "sprites_rip.h"
#include "textures_rip.h"
#include "sounds_music_rip.h"


/* global variables */
sound_fd_t snd_fd[SND_ENTRIES];
sound_fd_t mus_fd[MUS_ENTRIES];
DWORD img_fd[IMG_ENTRIES];

char path[FILENAME_MAX];
char *name;


/* function prototypes */
static void fd_init(FILE *G_fp);
static void rip_palette(struct qvr_palette_s qvr_palette[], DWORD pal_offset, FILE *stream);

static int imgFdCmp(const void *a, const void *b);



/* program's starting point */
int main(int argc, char **argv){
    FILE *G_fp;

    struct qvr_palette_s qvr_palette[256];


    puts("\t\tQuiver resource ripper by Yagotzirck\n");





    if(argc != 2){
        fputs("Usage: quiver_ripper G.bin", stderr);
        return 1;
    }

    if((G_fp = fopen(argv[1], "rb")) == NULL){
        perror("Couldn't open the specified file");
        return 1;
    }

    /* create directories */
    puts("Creating directories");
    makedirs(argv[1]);

    /* initialize file descriptors */
    puts("Initializing file descriptors");
    fd_init(G_fp);

    /* rip palette */
    puts("Ripping palette");
    rip_palette(qvr_palette, img_fd[PALETTESET1], G_fp);

    /* tga initializations */
    qvrToTgaPal(qvr_palette);
    init_tga_hdr();


    /* rip pictures */
    puts("Ripping pictures");
    rip_pictures(G_fp);

    /* rip hud and font */
    puts("Ripping HUD\nRipping HUD font");
    rip_hud_font(G_fp);

    /* rip textures */
    puts("Ripping textures");
    rip_textures(G_fp);

    /* rip sprites */
    puts("Ripping sprites");
    rip_sprites(G_fp);


    /* rip sounds and music */
    puts("Ripping sounds and music");
    rip_sounds_music(G_fp);


    /* the ripping process is now complete */
    fclose(G_fp);
    *name = '\0';
    printf("Quiver's resources have been successfully ripped into the following folder:\n%s\\\n", path);

    return 0;

}



static void fd_init(FILE *G_fp){
    const unsigned int unpolshdImgEntries = 41;
    unsigned int i = 0, discarded;
    DWORD unpolshdImgFd[41];    /* unpolished images file descriptor */

    /* acquire the sounds file descriptor */
    fseek(G_fp, SOUNDS_FD_OFFSET, SEEK_SET);
    fread(snd_fd, sizeof(snd_fd[0]), SND_ENTRIES, G_fp);

    /* acquire the music file descriptor */
    fseek(G_fp, MUSIC_FD_OFFSET, SEEK_SET);
    fread(mus_fd, sizeof(mus_fd[0]), MUS_ENTRIES, G_fp);

    /* acquire the images file descriptor */
    fseek(G_fp, IMAGES_FD_OFFSET, SEEK_SET);
    fread(unpolshdImgFd, sizeof(unpolshdImgFd[0]), unpolshdImgEntries, G_fp);

    /* polish the images file descriptor */
    qsort(unpolshdImgFd, unpolshdImgEntries, sizeof(unpolshdImgFd[0]), imgFdCmp);

    /* discard all the zero-entries */
    for(i = 0; unpolshdImgFd[i] == 0; ++i)
        ;
    memmove(&unpolshdImgFd[0], &unpolshdImgFd[i], sizeof(unpolshdImgFd[0]) * (unpolshdImgEntries - i));

    /* discard all the duplicate entries */
    discarded = i;
    for(i = 0; i < unpolshdImgEntries - discarded;){
        if(unpolshdImgFd[i] == unpolshdImgFd[i+1]){
            memmove(&unpolshdImgFd[i], &unpolshdImgFd[i+1], sizeof(unpolshdImgFd[0]) * (unpolshdImgEntries - discarded - i));
            ++discarded;
        }
        else
            ++i;
    }


    /* copy the now-polished images' file descriptor to the global array */
    memcpy(img_fd, unpolshdImgFd, sizeof(img_fd[0]) * IMG_ENTRIES);


    /* swap hud pics index with sky texture index so that pics can be ripped sequentially with a for() loop later on;
       use the first position of unpolshdImgFd[] as swapping variable since we don't need it anymore
    */
    unpolshdImgFd[0] = img_fd[SKYTEXTURE];
    img_fd[SKYTEXTURE] = img_fd[HUD_PICS];
    img_fd[HUD_PICS] = unpolshdImgFd[0];

    /* swap compass pic index with credits pic index so that we'll have all the 320x200 pics in a single sequence */
    unpolshdImgFd[0] = img_fd[COMPASS_PIC];
    img_fd[COMPASS_PIC] = img_fd[CREDITS_PIC];
    img_fd[CREDITS_PIC] = unpolshdImgFd[0];

    /* swap HUD pic index with ESD Games 2 pic index (HUD pic needs to be saved in "HUD - misc" folder) */
    unpolshdImgFd[0] = img_fd[HUD_PIC];
    img_fd[HUD_PIC] = img_fd[ESDGAMES2_PIC];
    img_fd[ESDGAMES2_PIC] = unpolshdImgFd[0];
}

static void rip_palette(struct qvr_palette_s qvr_palette[], DWORD pal_offset, FILE *stream){
    FILE *out_fp;
    fseek(stream, pal_offset, SEEK_SET);
    fread(qvr_palette, sizeof(struct qvr_palette_s), 256, stream);

    strcpy(name, "/palette.pal");

    if((out_fp = fopen(path, "wb")) == NULL){
        fprintf(stderr, "Couldn't create %s\n", path);
        exit(EXIT_FAILURE);
    }

    fwrite(qvr_palette, sizeof(qvr_palette[0]), 256, out_fp);
    fclose(out_fp);
}


static int imgFdCmp(const void *a, const void *b){
    return *(DWORD *)a - *(DWORD *)b;
}
