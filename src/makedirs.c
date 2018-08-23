#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "makedirs.h"

extern char path[];
extern char *name;

void makedirs(const char *archivename){
    strcpy(path, archivename);

    /* position to "G.bin" (excluding path) */

    name = path + strlen(archivename);

    while(*--name != '\\')
        ;

    ++name;

    if(strcmp(name, "G.bin") != 0){
        fputs("You have to pass Quiver's game archive file G.bin as argument.", stderr);
        exit(EXIT_FAILURE);
    }

    /* create main directory */
    strcpy(name, "Quiver rip");
    CreateDirectory(path, NULL);

    /* create subdirectories */
    name = path + strlen(path);

    strcpy(name, "/menus-pics");
    CreateDirectory(path, NULL);

    strcpy(name, "/flats-textures");
    CreateDirectory(path, NULL);

    strcpy(name, "/door and bars textures");
    CreateDirectory(path, NULL);

    strcpy(name, "/patches");
    CreateDirectory(path, NULL);

    strcpy(name, "/HUD - misc");
    CreateDirectory(path, NULL);

    strcpy(name, "/HUD - misc/HUD font");
    CreateDirectory(path, NULL);

    strcpy(name, "/sprites");
    CreateDirectory(path, NULL);

    strcpy(name, "/music");
    CreateDirectory(path, NULL);

    strcpy(name, "/sounds");
    CreateDirectory(path, NULL);

}

