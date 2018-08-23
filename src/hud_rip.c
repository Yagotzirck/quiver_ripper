#include <stdio.h>

#include "hud_rip.h"
#include "quiver_ripper.h"
#include "pic_handler.h"

extern DWORD img_fd[];

void rip_hud_font(FILE *stream){
    unsigned int i;
    char numName[10];

    /* rip the main HUD picture */
    handle_picture(DIR_HUD, "HUD", 320, 200, img_fd[HUD_PIC], PIC_IMAGE, stream);

    /* rip compass picture */
    handle_picture(DIR_HUD, "Compass", 320, 14, img_fd[COMPASS_PIC], PIC_IMAGE, stream);

    /* rip block of 35 icons, 32x32 each */
    fseek(stream, img_fd[HUD_PICS], SEEK_SET);
    for(i = 1; i <= 35; ++i){
        sprintf(numName, "%u", i);
        handle_picture(DIR_HUD, numName, 32, 32, ftell(stream), PIC_IMAGE, stream);
    }

    /* rip HUD blue numbers, 15x16 each */
    fseek(stream, img_fd[HUD_BLUENUMS], SEEK_SET);
    for(i = 0; i < 10; ++i){
        sprintf(numName, "HUD num %u", i);
        handle_picture(DIR_HUD, numName, 15, 16, ftell(stream), PIC_IMAGE, stream);
    }

    /* the last blue symbol is "%" */
    sprintf(numName, "HUD num %%");
    handle_picture(DIR_HUD, numName, 15, 16, ftell(stream), PIC_IMAGE, stream);


    /* rip the font, 5x5 each character */
    fseek(stream, img_fd[FONT], SEEK_SET);
    for(i = 0; i <= 9; ++i){
        sprintf(numName, "%u", i);
        handle_picture(DIR_FONT, numName, 5, 5, ftell(stream), PIC_IMAGE, stream);
    }

    for(i = 'A'; i <= 'Z'; ++i){
        sprintf(numName, "%c", i);
        handle_picture(DIR_FONT, numName, 5, 5, ftell(stream), PIC_IMAGE, stream);
    }

    /* there's some weird "gems" at the end, separated from the last letter by 17 blank chars */
    fseek(stream, 5*5 * 17, SEEK_CUR);
    for(i = 1; i <= 5; ++i){
        sprintf(numName, "gem %u", i);
        handle_picture(DIR_FONT, numName, 5, 5, ftell(stream), PIC_IMAGE, stream);
    }

}
