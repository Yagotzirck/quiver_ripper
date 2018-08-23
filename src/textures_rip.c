#include <stdio.h>

#include "textures_rip.h"
#include "quiver_ripper.h"
#include "pic_handler.h"

#define TEXTURE_ENTRIES 176
#define DOORTEX_ENTRIES 12
#define PATCH_ENTRIES   24

extern DWORD img_fd[];

void rip_textures(FILE *stream){
    unsigned int i;
    char numName[4];

    /* rip the standard 64x64 textures/flats */
    fseek(stream, img_fd[FLATS_TEXTURES], SEEK_SET);
    for(i = 1; i <= TEXTURE_ENTRIES; ++i){
        sprintf(numName, "%u", i);
        handle_picture(DIR_TEXTURES, numName, 64, 64, ftell(stream), PIC_TEXTURE, stream);
    }

    /* rip the stretched 64x128 door textures */
    fseek(stream, img_fd[DOOR_TEXTURES], SEEK_SET);
    for(i = 1; i <= DOORTEX_ENTRIES; ++i){
        sprintf(numName, "%u", i);
        handle_picture(DIR_DOORTEXTURES, numName, 64, 64, ftell(stream), PIC_TEXTURE_64x128, stream);
    }

    /* rip the 32x32 patches */
    fseek(stream, img_fd[PATCHES], SEEK_SET);
    for(i = 1; i <= PATCH_ENTRIES; ++i){
        sprintf(numName, "%u", i);
        handle_picture(DIR_PATCHES, numName, 32, 32, ftell(stream), PIC_TEXTURE, stream);
    }



}

