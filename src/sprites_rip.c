#include <stdio.h>

#include "sprites_rip.h"
#include "quiver_ripper.h"
#include "tga_utils.h"
#include "pic_handler.h"

#define SPRITE_ENTRIES 486

#define SPRITEDESCR_OFFSET 0X300L

extern DWORD img_fd[];


void rip_sprites(FILE *stream){
    unsigned int i;
    char numName[4];

    struct sprites_descriptor_s{
        unsigned int size;
        BYTE buf_width, buf_height;
        BYTE width, height;
        BYTE xOffset, xOffset2;
        BYTE yOffset, yOffset2;
    }sprites_descriptor[SPRITE_ENTRIES];

    /* get array of sprite descriptors */
    fseek(stream, SPRITEDESCR_OFFSET, SEEK_SET);
    fread(sprites_descriptor, sizeof(struct sprites_descriptor_s), SPRITE_ENTRIES, stream);

    fseek(stream, img_fd[SPRITES], SEEK_SET);
    for(i = 0; i < SPRITE_ENTRIES; ++i){
        /* each sprite data block is preceeded by a 2-byte index which needs to be skipped */
        fseek(stream, 2L, SEEK_CUR);

        sprintf(numName, "%u", i + 1);
        handle_picture(DIR_SPRITES, numName, sprites_descriptor[i].width, sprites_descriptor[i].height, ftell(stream), PIC_SPRITE, stream);
    }
}
