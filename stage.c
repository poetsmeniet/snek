#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "snek.h"
#include "stage.h"
#define clear() printf("\033[H\033[J") //to clear the linux term

void deleteFoodSegment(food *mice, fSeg *seg){
    fSeg *head = mice->seg;
    fSeg *prevSeg = head;

    while(head != NULL){
        if(head == seg){
            //relink previous item in list to next, if needed
            if(head->next != NULL){
                prevSeg->next = head->next;
            }
            else{
                if(prevSeg == seg){
                    //printf("this is first and last item..\n");
                }else{
                    //printf(" - last item, so deleting and removing prev link(%c) to last item\n", prevSeg->tok);
                    prevSeg->next = NULL;
                    free(head);
                }
            }
        }
        prevSeg = head;
        head = head->next;
    }

    size_t foodCnt = 0;
    fSeg *ft = mice->seg;
    while(ft != NULL){
        ft = ft->next;
        foodCnt++;
    }
    if(foodCnt == 1)
        pushFoodSegments(mice, (rand() % 5) + 1);
}

extern void pushFoodSegments(food *mice, size_t amount){
    fSeg *head = mice->seg;
    size_t segCnt = 0; //Segment counter

    while(head->next != NULL){
        head = head->next;
        segCnt++;
    }
    segCnt++;
    
    size_t i;

    for(i = 0; i < amount; i++){
        head->next = malloc(1 * sizeof(segm));
        head->next->tok = i+1+'0';
        head->next->d = head->d;
        head->next->x = rand() % 30;
        head->next->y = rand() % 50;
        head->next->next = NULL;

        head = head->next;
        segCnt++;
    }
}

void spawnFood(size_t amount, food *mice){
    srand(time(NULL));

    mice->d = 0;
    mice->seg = malloc(sizeof(fSeg));
    mice->seg->tok = '0';
    mice->seg->x = rand() % 30;
    mice->seg->y = rand() % 50;
    mice->seg->d = 0;
    mice->seg->next = NULL;

    pushFoodSegments(mice, amount);
}

extern void printField(int cols, int rows, snek *snek, food *mice){
    char *printBuf = malloc((3 * (cols * rows)) * sizeof(char));
    size_t bufMmb = 0;

    //clear();

    char tok = '.';
    int c;
    int r;
    int skip = 0;
    int tstCnt1= 0;

    //Go through grid and place snek segments in print buf
    for(r = 0; r < rows; r++){
        for(c = 0; c < cols; c++){
            tstCnt1++;
            //Add snek to buffer/ field
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

            //Add food to buffer/ field
            fSeg *m = mice->seg;
            size_t foodCnt = 0;

            while(m != NULL){
                //Detect colision with "food" and snek head
                if(m->x == snek->seg->x && m->y == snek->seg->y){//Verify segments
                    //move food out of field..
                    m->x += 1000;
                    deleteFoodSegment(mice, m);
            pthread_mutex_lock(&snek->moveLock);
                    addSegments(snek, 1);
            pthread_mutex_unlock(&snek->moveLock);
                    skip = 1;
                }else{
                if(m->x == r && m->y == c){//Verify segments
                    printBuf[bufMmb] = m->tok;
                    bufMmb++;
                    printBuf[bufMmb] = ' ';
                    bufMmb++;
                    skip = 1;
                }

                }
                foodCnt++;
                m = m->next;
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
