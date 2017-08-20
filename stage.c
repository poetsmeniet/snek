#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "snek.h"
#include "stage.h"
#define clear() printf("\033[H\033[J") //to clear the linux term

//Needs work
void deleteFoodSegment(food *mice, fSeg *seg, snek *snek){
    fSeg *head = mice->seg;
    fSeg *prevSeg = head;

    while(head != NULL){
        if(head == seg){
            //relink previous item in list to next, if needed
            if(head->next != NULL){
                //printf("relinking\n");
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
        pushFoodSegments(mice, (rand() % 5) + 1, snek);
}

extern void pushFoodSegments(food *mice, size_t amount, snek *snek){
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
        head->next->x = rand() % snek->rows;
        head->next->y = rand() % snek->cols;
        head->next->next = NULL;
    if(head->next->x == 0)
        head->next->x+=2;
    if(head->next->y == 0)
        head->next->y+=2;
    if(head->next->x == snek->cols)
        head->next->x-=2;
    if(head->next->y == snek->rows)
        head->next->y-=2;

        head = head->next;
        segCnt++;
    }
}

void spawnFood(size_t amount, food *mice, snek *snek){
    srand(time(NULL));

    mice->d = 0;
    mice->seg = malloc(sizeof(fSeg));
    mice->seg->tok = 'X';
    mice->seg->x = rand() % snek->rows;
    mice->seg->y = rand() % snek->cols;
    mice->seg->d = 0;
    mice->seg->next = NULL;

    if(mice->seg->x == 0)
        mice->seg->x+=2;
    if(mice->seg->y == 0)
        mice->seg->y+=2;
    if(mice->seg->x == snek->cols)
        mice->seg->x-=2;
    if(mice->seg->y == snek->rows)
        mice->seg->y-=2;

    //

    pushFoodSegments(mice, amount, snek);
}

extern void printField(int cols, int rows, snek *snek, food *mice){
    char *printBuf = malloc((3 * (cols * rows)) * sizeof(char));
    size_t bufMmb = 0;

    char tok1 = ' ';
    char tok2 = ' ';
    int c;
    int r;
    int skip = 0;

    //Go through grid and place snek segments in print buf
    for(r = 0; r < rows; r++){
        for(c = 0; c < cols; c++){
            if(c == 0 || r == 0){
                if(r == 0){
                    tok1 = '.';
                    tok2 = '.';
                }else{
                    tok1 = '.';
                    tok2 = ' ';
                }
            }else if(c == (snek->cols -1) || r == (snek->rows - 1)){
                if(c == (snek->cols -1)){
                    tok1 = ' ';
                    tok2 = '.';
                }else{
                    tok1 = '.';
                    tok2 = '.';
                }
            }else{
                tok1 = ' ';
                tok2 = ' ';
            }

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
                    m->x += 1000;//move food out of field..
                    deleteFoodSegment(mice, m, snek);
                    addSegments(snek, 1);
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
                printBuf[bufMmb] = tok1;
                bufMmb++;
                printBuf[bufMmb] = tok2;
                bufMmb++;
            }
            skip = 0;

            //Detect snek body colision on next coord
            segm *s = snek->seg->next;

            while(s != NULL){
                if(s->x == snek->seg->x\
                        && s->y == snek->seg->y\
                        ){
                    printf("%s\n", printBuf);
                    exit(1);
                }
                s = s->next;
            }

        }

        //Detect playfield boundary 
        if(snek->seg->d == 1 && (snek->seg->y + 1) == snek->cols){
            printf("%s\n", printBuf);
            exit(1);
        }
        if(snek->seg->d == 3 && (snek->seg->y) == 0){
            printf("%s\n", printBuf);
            exit(1);
        }
        if(snek->seg->d == 2 && (snek->seg->x) == (snek->rows - 1)){
            printf("%s\n", printBuf);
            exit(1);
        }
        if(snek->seg->d == 0 && (snek->seg->x) == 0){
            printf("%s\n", printBuf);
            exit(1);
        }

        printBuf[bufMmb] = '\n';
        bufMmb++;
    }
    printf("%s\n", printBuf);
    free(printBuf);
    clear();
}
