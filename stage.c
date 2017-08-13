#include <stdio.h>
#include "snek.h"
#define clear() printf("\033[H\033[J") //to clear the linux term

extern void printField(int cols, int rows, snek *snek){
    clear();
    char tok = '.';
    int c;
    int r;
    int skip = 0;
    for(r = 0; r < rows; r++){
        for(c = 0; c < cols; c++){//Print tokens
            //check for each segment if it nees printing
            segm *sS = snek->seg;
            while(sS != NULL){
                if(sS->x == r && sS->y == c){
                    printf("%c ", sS->tok);
                    skip = 1;
                }
                sS = sS->next;
            }
            if(skip == 0)
            printf("%c ", tok);
            skip = 0;
        }
        printf("\n");
    }
    printf("\n");
}
