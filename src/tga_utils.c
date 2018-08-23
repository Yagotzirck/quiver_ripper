#include <stdio.h>

#include "quiver_ripper.h"
#include "tga_utils.h"

typedef struct _TgaHeader
{
  BYTE IDLength;        /* 00h  Size of Image ID field */
  BYTE ColorMapType;    /* 01h  Color map type */
  BYTE ImageType;       /* 02h  Image type code */
  WORD CMapStart;       /* 03h  Color map origin */
  WORD CMapLength;      /* 05h  Color map length */
  BYTE CMapDepth;       /* 07h  Depth of color map entries */
  WORD XOffset;         /* 08h  X origin of image */
  WORD YOffset;         /* 0Ah  Y origin of image */
  WORD Width;           /* 0Ch  Width of image */
  WORD Height;          /* 0Eh  Height of image */
  BYTE PixelDepth;      /* 10h  Image pixel size */
  BYTE ImageDescriptor; /* 11h  Image descriptor byte */
} TGAHEAD;

struct tga_palette_s{
    BYTE blue, green, red, alpha;
};

/* global variables (file scope only) */
static TGAHEAD tga_header;
static struct tga_palette_s tga_palette[256], tga_shrunk_palette[256];


/* local functions declarations */
static WORD shrink_palette(BYTE imgBuf[], DWORD size, BYTE used_indexes[]);


/* functions definitions */
void qvrToTgaPal(const struct qvr_palette_s *qvr_palette){
    int i;

    tga_palette[0].blue = 0;
    tga_palette[0].green = 0;
    tga_palette[0].red = 0;
    tga_palette[0].alpha = 0;

    for(i = 1; i < 256; ++i){
        tga_palette[i].blue = qvr_palette[i].blue;
        tga_palette[i].green = qvr_palette[i].green;
        tga_palette[i].red = qvr_palette[i].red;
        tga_palette[i].alpha = 0xFF;
    }
}



void init_tga_hdr(void){
    tga_header.IDLength =          0;   /* No image ID field used, size 0 */
    tga_header.ColorMapType =      1;   /* colormapped */
    tga_header.ImageType =         0;   /* it's going to be either 1(colormapped) or 9 (colormapped + encoded) */
    tga_header.CMapStart =         0;   /* Color map origin */
    tga_header.CMapLength =        0;   /* Depends on image, will be set after image processing */
    tga_header.CMapDepth =         32;  /* Depth of color map entries */
    tga_header.XOffset =           0;   /* X origin of image */
    tga_header.YOffset =           0;   /* Y origin of image */
    tga_header.Width =             0;   /* Width of image */
    tga_header.Height =            0;   /* Height of image */
    tga_header.PixelDepth =        8;   /* Image pixel size */
    tga_header.ImageDescriptor =   0;   /* Image descriptor */
}



void set_tga_hdr(WORD width, WORD height, enum tgaImageType imgType, enum tgaImageDescriptor ImageDesc){
    tga_header.Width = width;
    tga_header.Height = height;
    tga_header.ImageType = imgType;
    tga_header.ImageDescriptor = ImageDesc;
}

void write_tga_hdr(FILE *stream){
    fwrite(&tga_header.IDLength, sizeof(tga_header.IDLength), 1, stream);
    fwrite(&tga_header.ColorMapType, sizeof(tga_header.ColorMapType), 1, stream);
    fwrite(&tga_header.ImageType, sizeof(tga_header.ImageType), 1, stream);
    fwrite(&tga_header.CMapStart, sizeof(tga_header.CMapStart), 1, stream);
    fwrite(&tga_header.CMapLength, sizeof(tga_header.CMapLength), 1, stream);
    fwrite(&tga_header.CMapDepth, sizeof(tga_header.CMapDepth), 1, stream);
    fwrite(&tga_header.XOffset, sizeof(tga_header.XOffset), 1, stream);
    fwrite(&tga_header.YOffset, sizeof(tga_header.YOffset), 1, stream);
    fwrite(&tga_header.Width , sizeof(tga_header.Width), 1, stream);
    fwrite(&tga_header.Height, sizeof(tga_header.Height), 1, stream);
    fwrite(&tga_header.PixelDepth, sizeof(tga_header.PixelDepth), 1, stream);
    fwrite(&tga_header.ImageDescriptor, sizeof(tga_header.ImageDescriptor), 1, stream);
}

void write_tga_pal(FILE *stream){
    fwrite(tga_shrunk_palette, sizeof(struct tga_palette_s), tga_header.CMapLength, stream);
}


/* returns the shrunk size */
int shrink_tga(BYTE imgDest[], BYTE imgBuf[], DWORD size){
    BYTE used_indexes[256] = {0};
    unsigned int i = 0, j = 0;

    union counter_u{
        struct RLE_counter_s{
            BYTE counter : 7;
            BYTE highBit : 1;
        }RLE_counter;

        BYTE RLE_byte;
    }counter;

    tga_header.CMapLength = shrink_palette(imgBuf, size, used_indexes);

    counter.RLE_counter.highBit = 1;
    do{
        counter.RLE_counter.counter = 0;
        while(i + 1 < size && imgBuf[i+1] == imgBuf[i]){
            ++counter.RLE_counter.counter;
            ++i;

            if(counter.RLE_counter.counter == 127)
                break;
        }

        imgDest[j++] =  counter.RLE_byte;
        imgDest[j++] =  used_indexes[imgBuf[i++]];
    }while(i < size);

    /* if the RLE compression resulted in increased size keep the data in its raw form
    ** (update the pixel indexes to the new shrunk palette first) */
    if(j >= size){
        for(i = 0; i < size; ++i)
            imgBuf[i] = used_indexes[imgBuf[i]];

        return -1;
    }

    return j;
}




/* local functions definitions */
static WORD shrink_palette(BYTE imgBuf[], DWORD size, BYTE used_indexes[]){
    unsigned int i, j;

    /* scan the whole image to track the palette colors actually used */
    for(i = 0; i < size; ++i)
        used_indexes[imgBuf[i]] = 1;

    /* remap the palette with the used palette colors placed sequentially */
    for(i = 0, j = 0; i < 256; ++i)
        if(used_indexes[i]){
            tga_shrunk_palette[j] = tga_palette[i];
            used_indexes[i] = j++;
        }

    return j;
}


