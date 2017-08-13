#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "keybInterface.h"
#include "snek.h"
#include "stage.h"

void printField(int cols, int rows, snek *snek);

int main(void){
    snek snek;

    initSnek(&snek);

    addSegments(&snek, 25);

    if(!startKeybInterface(&snek)){
        printf("Error with keyboard interface thread, exiting..\n");
        return 1;
    }

    while(1){
        printField(50, 30, &snek);
        moveSnek(&snek);
        usleep(80000);
    }
}

