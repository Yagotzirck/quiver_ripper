#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "makedirs.h"

extern char path[];
extern char *name;

void makedirs(const char *archivename){
    char lowercase_archivename[FILENAME_MAX];
    int i;

    strcpy(path, archivename);

    /* position to "G.bin" (excluding path) */
    name = path + strlen(path);

    do --name;
    while(*name != '\\' && *name != '/' && name != path);

    if(*name == '\\' || *name == '/')
        ++name;

    /* create a lowercase string of the filename passed as an argument, to avoid
    ** issues with case sensitivity when checking the filename
    */
    for(i = 0; name[i] != '\0'; ++i)
        lowercase_archivename[i] = tolower(name[i]);
    lowercase_archivename[i] = '\0';

    if(strcmp(lowercase_archivename, "g.bin") != 0){
        fputs("You have to pass Quiver's game archive file G.bin as an argument.", stderr);
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

