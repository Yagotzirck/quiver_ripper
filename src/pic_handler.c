#include <stdio.h>
#include <stdlib.h>

#include "pic_handler.h"
#include "quiver_ripper.h"
#include "tga_utils.h"

extern char path[FILENAME_MAX];
extern char *name;

/* local functions prototypes */
static void flip_image(BYTE dest[], const BYTE src[], WORD width, WORD height);
static void swap_width_height(WORD *width, WORD *height);


void handle_picture(enum dir_string_e dir_string, const char *filename, WORD width, WORD height, DWORD offset, enum pic_type_e pic_type, FILE *stream){
    static char *dir[] = {
        "menus-pics",
        "flats-textures",
        "door and bars textures",
        "patches",
        "HUD - misc",
        "HUD - misc/HUD font",
        "sprites"
    };


    BYTE imgBuf[1280*200], imgShrunk[1280*200], imgFlipped[255*255];
    BYTE *bufPtr;

    FILE *out_fp;

    int shrunk_size, size;
    unsigned int i, j;
    enum tgaImageType imgType;

    sprintf(name, "/%s/%s.tga", dir[dir_string], filename);

    if((out_fp = fopen(path, "wb")) == NULL){
        fprintf(stderr, "Couldn't create %s\n", path);
        exit(EXIT_FAILURE);
    }



    fseek(stream, offset, SEEK_SET);

    fread(imgBuf, 1, width * height, stream);

    switch(pic_type){
        case PIC_IMAGE:
            bufPtr = imgBuf;
            break;

        case PIC_SPRITE:
        case PIC_TEXTURE:
            flip_image(imgFlipped, imgBuf, width, height);
            swap_width_height(&width, &height);
            bufPtr = imgFlipped;
            break;

        case PIC_TEXTURE_64x128:
            /* each column pixel needs to be doubled since that's what the engine does to achieve 64x128 doors;
            ** use imgShrunk as temporary buffer before flipping the image */
            for(i = 0, j = 0; i < width * height; ++i){
                imgShrunk[j++] = imgBuf[i];
                imgShrunk[j++] = imgBuf[i];
            }
            width = 128;
            height = 64;

            flip_image(imgFlipped, imgShrunk, width, height);
            swap_width_height(&width, &height);
            bufPtr = imgFlipped;
            break;
    }

    /* shrink tga */
    shrunk_size = shrink_tga(imgShrunk, bufPtr, width * height);

    /* if the RLE compression resulted in increased size save the data in its raw form */
    if(shrunk_size == -1){
        imgType = IMGTYPE_COLORMAPPED;
        size = width * height;
    }
    else{
        imgType = IMGTYPE_COLORMAPPED_RLE;
        size = shrunk_size;
        bufPtr = imgShrunk;
    }

    /* set the tga header */
    set_tga_hdr(width, height, imgType, TOP_LEFT | ATTRIB_BITS);

    /* write tga header */
    write_tga_hdr(out_fp);

    /* write the palette */
    write_tga_pal(out_fp);

    /* write the pixel data */
    fwrite(bufPtr, 1, size, out_fp);

    fclose(out_fp);
}


static void flip_image(BYTE dest[], const BYTE src[], WORD width, WORD height){
    unsigned int x, y;

    for(x = 0; x < height; ++x)
        for(y = 0; y < width; ++y)
            dest[y*height + x] = src[x*width + y];
}


static void swap_width_height(WORD *width, WORD *height){
    WORD temp = *width;
    *width = *height;
    *height = temp;
}
