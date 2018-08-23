#ifndef QUIVER_RIPPER_H
#define QUIVER_RIPPER_H

#define SOUNDS_FD_OFFSET    0x67A4L
#define SND_ENTRIES         69

#define MUSIC_FD_OFFSET     0x6AA4L
#define MUS_ENTRIES         11

#define IMAGES_FD_OFFSET    0x6B64L
#define IMG_ENTRIES         31      /* this actually needs to be polished after acquiring it */


typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned int    DWORD;

typedef struct sound_fd_s{
    DWORD offset, size;
}sound_fd_t;


struct qvr_palette_s{
    BYTE red, green, blue;
};


enum imgFdPointers{
    ENGINE_VALUES,
    PALETTESET1,
    PALETTESET2,
    PALETTESET3,
    HUD_PICS,
    FLATS_TEXTURES,
    DOOR_TEXTURES,
    PATCHES,
    SPRITES,
    TITLE_PIC,
    MAINMENU_PIC,
    QUICK_LOADNSAVE_PIC,
    SKILL_PIC,
    CONTROLS_PIC,
    EP_SELECTION_PIC,
    MUSICTYPE_PIC,
    OPTIONS_PIC,
    ESDGAMES2_PIC,
    INTROMISSIONBORDER_PIC,
    CREDITS_PIC,
    SND_MUS_MOUSE_REG_PIC,
    ADVSOFT_PIC,
    ESDGAMES_PIC,
    HUD_PIC,
    COMPASS_PIC,
    INTROMISSIONMAP_PIC,
    SKYTEXTURE,
    FONT,
    HUD_BLUENUMS,
    QVR_SHRWR_QUIT_MSG,
    QVR_REGSTRD_QUIT_MSG
};

#endif /* QUIVER_RIPPER_H */
