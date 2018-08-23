#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "quiver_ripper.h"
#include "pictures_rip.h"
#include "tga_utils.h"
#include "pic_handler.h"

#define PIC_ENTRIES_320x200 14

extern DWORD img_fd[];

void rip_pictures(FILE *stream){
    char *filenames[] = {
        "Title",
        "Main menu",
        "Quick load-save",
        "Skill",
        "Controls",
        "Episode selection",
        "Music type",
        "Options",
        "ESD games 2",
        "Intro mission - borders",
        "Credits",
        "Sound - music - mouse regulations",
        "Advertainment soft",
        "ESD games",
    };
    unsigned int i;


    /* rip the 320x200 pictures */
    for(i = TITLE_PIC; i < TITLE_PIC + PIC_ENTRIES_320x200; ++i)
        handle_picture(DIR_MENUS, filenames[i - TITLE_PIC], 320, 200, img_fd[i], PIC_IMAGE, stream);


    /* rip the 640x200 map picture */
    handle_picture(DIR_MENUS, "Intro mission - map", 640, 200, img_fd[INTROMISSIONMAP_PIC], PIC_IMAGE, stream);

    /* rip the 1280x200 sky texture */
    handle_picture(DIR_MENUS, "Sky texture", 1280, 200, img_fd[SKYTEXTURE], PIC_IMAGE, stream);

}
