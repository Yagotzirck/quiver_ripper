#ifndef PIC_HANDLER_H
#define PIC_HANDLER_H

#include <stdio.h>

#include "quiver_ripper.h"

enum pic_type_e{
    PIC_IMAGE,
    PIC_TEXTURE,
    PIC_TEXTURE_64x128,
    PIC_SPRITE
};

enum dir_string_e{
    DIR_MENUS,
    DIR_TEXTURES,
    DIR_DOORTEXTURES,
    DIR_PATCHES,
    DIR_HUD,
    DIR_FONT,
    DIR_SPRITES
};


void handle_picture(enum dir_string_e dir_string, const char *filename, WORD width, WORD height, DWORD offset, enum pic_type_e pic_type, FILE *stream);

#endif /* PIC_HANDLER_H */
