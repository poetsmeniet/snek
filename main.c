#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "keybInterface.h"
#include "joyInterface.h"
#include "snek.h"
#include "stage.h"
#define COLS 20 
#define ROWS 30

int main(void){
    snek snek;

    initSnek(&snek, COLS, ROWS);

    addSegments(&snek, 1);

    food *mice = malloc(sizeof(food));
    spawnFood(5, mice, &snek);

    if(!startKeybInterface(&snek)){
        printf("Error with keyboard interface thread, exiting..\n");
        return 1;
    }
    if(!startJoyInterface(&snek))
        printf("Error with joy interface thread, cont..\n");

    while(1){
        printField(COLS, ROWS, &snek, mice);
        moveSnek(&snek);
        usleep(200000);
    }
}

