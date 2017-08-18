#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "keybInterface.h"
#include "joyInterface.h"
#include "snek.h"
#include "stage.h"

int main(void){
    snek snek;

    initSnek(&snek);

    addSegments(&snek, 10);

    food *mice = malloc(sizeof(food));
    spawnFood(5, mice);

    if(!startKeybInterface(&snek)){
        printf("Error with keyboard interface thread, exiting..\n");
        return 1;
    }
    if(!startJoyInterface(&snek))
        printf("Error with joy interface thread, cont..\n");

    while(1){
        printField(50, 30, &snek, mice);
        moveSnek(&snek);
        usleep(200000);
    }
}

