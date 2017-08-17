#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include <pthread.h>
#include "keybInterface.h"
#include "snek.h"
#include "stage.h"

int main(void){
    snek snek;

    initSnek(&snek);

    addSegments(&snek, 15);

    food *mice = malloc(sizeof(food));
    spawnFood(5, mice);

    if(!startKeybInterface(&snek)){
        printf("Error with keyboard interface thread, exiting..\n");
        return 1;
    }

    while(1){
        printField(50, 30, &snek, mice);
        moveSnek(&snek);
        usleep(200000);
    }
}

