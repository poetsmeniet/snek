#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "keybInterface.h"
#include "joyInterface.h"
#include "snek.h"
#include "stage.h"
#define COLS 15 
#define ROWS 20 

int main(void){
    snek snek;

    initSnek(&snek, COLS, ROWS);

    addSegments(&snek, 12);

    food *mice = malloc(sizeof(food));
    spawnFood(1, mice, &snek);

    if(!startKeybInterface(&snek)){
        printf("Error with keyboard interface thread, exiting..\n");
        return 1;
    }
    if(!startJoyInterface(&snek)){
        printf("Error with joy interface thread, cont..\n");
        sleep(1);
    }

    while(1){
        moveSnek(&snek);
        moveFood(mice, &snek);
        printField(COLS, ROWS, &snek, mice);
        //Todo: add check for negative values
        usleep(300000 - (snek.lvl * 5000));
    }
    return 0;
}

