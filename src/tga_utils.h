#ifndef TGA_UTILS_H
#define TGA_UTILS_H

#include <stdio.h>

enum tgaImageDescriptor{
    ATTRIB_BITS =   8,
    BOTTOM_LEFT =   0x00,
    TOP_LEFT    =   0x20
};

enum tgaImageType{
    IMGTYPE_COLORMAPPED =       1,
    IMGTYPE_COLORMAPPED_RLE =   9
};

/* function prototypes */
void qvrToTgaPal(const struct qvr_palette_s *qvr_palette);
void init_tga_hdr(void);
void set_tga_hdr(WORD width, WORD height, enum tgaImageType imgType, enum tgaImageDescriptor ImageDesc);
void write_tga_hdr(FILE *stream);
void write_tga_pal(FILE *stream);
int shrink_tga(BYTE imgDest[], BYTE imgBuf[], DWORD size);


#endif /* TGA_UTILS_H */
