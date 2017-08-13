#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "snek.h"
#include "keybInterface.h"
#define clear() printf("\033[H\033[J") //to clear the linux term

void printField(int cols, int rows, snek *snek);

void addSegments(snek *snek, size_t amount){
    segm *head = snek->seg;
    size_t segCnt = 0; //Segment counter

    while(head->next != NULL){
        head = head->next;
        segCnt++;
    }
    segCnt++;
    
    size_t i;

    for(i = 0; i < amount; i++){
        printf("jib; head->y: %d, \n", head->y);
        head->next = malloc(1 * sizeof(segm));
        //head->next->tok = segCnt+'0';
        head->next->tok = 'O';
        head->next->d = head->d;
        head->next->x = head->x;
        head->next->y = head->y - 1;
        head->next->next = NULL;

        head = head->next;
        segCnt++;
    }
}

void moveSnek(snek *snek){
    //d (direction):
    // 0: north, up
    // 1: east, right
    // 2: south, down
    // 3: west: left    
    segm *segHead = snek->seg;

    while(segHead->next != NULL){
        switch(segHead->d){
            case 0:
                segHead->x--;
                break;
            case 1:
                segHead->y++;
                break;
            case 2:
                segHead->x++;
                break;
            case 3:
                segHead->y--;
                break;
            default:
                break;
        }
        //get next segment
        segHead = segHead->next;
    }
    //Not so nice, last element in linked list
    switch(segHead->d){
        case 0:
            segHead->x--;
            break;
        case 1:
            segHead->y++;
            break;
        case 2:
            segHead->x++;
            break;
        case 3:
            segHead->y--;
            break;
        default:
            break;
    }
    
    //align next segment
    segm *algnSeg = snek->seg;
    while(algnSeg->next != NULL){
        if(algnSeg->d != algnSeg->next->d){
            algnSeg->next->d = algnSeg->d;

            if(algnSeg->next->next != NULL)
                algnSeg = algnSeg->next->next;
            else
                break;
        }else{
            algnSeg = algnSeg->next;
        }
    }

}

int main(void){
    snek snek;
    //initial segment
    snek.seg = malloc(1 * sizeof(segm));
    snek.seg->tok = 'O';
    snek.seg->x = 20;
    snek.seg->y = 10;
    snek.seg->d = 1;
    snek.seg->next = NULL;

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

void printField(int cols, int rows, snek *snek){
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
