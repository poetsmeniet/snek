#include <stdio.h>
#include <stdlib.h>
#include "snek.h"
#define clear() printf("\033[H\033[J") //to clear the linux term

extern void printField(int cols, int rows, snek *snek){
    char *printBuf = malloc((3 * (cols * rows)) * sizeof(char));
    size_t bufMmb = 0;

    clear();

    char tok = '.';
    int c;
    int r;
    int skip = 0;

    //Go through grid and place snek segments in print buf
    for(r = 0; r < rows; r++){
        for(c = 0; c < cols; c++){
            segm *sS = snek->seg;
            while(sS != NULL){
                if(sS->x == r && sS->y == c){//Verify segments
                    printBuf[bufMmb] = sS->tok;
                    bufMmb++;
                    printBuf[bufMmb] = ' ';
                    bufMmb++;
                    skip = 1;
                }
                sS = sS->next;
            }
            if(skip == 0){ //Otherwise print something else
                printBuf[bufMmb] = tok;
                bufMmb++;
                printBuf[bufMmb] = ' ';
                bufMmb++;
            }
            skip = 0;
        }
        printBuf[bufMmb] = '\n';
        bufMmb++;
    }
    printf("%s\n", printBuf);
    free(printBuf);
}
