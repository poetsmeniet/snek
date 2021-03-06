#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "snek.h"
#include "stage.h"
#define clear() printf("\033[H\033[J") //to clear the linux term

static void exitGame(snek *snek, food *mice, char *printBuf){
    segm *head = snek->seg;
    segm *curr;
    while ((curr = head) != NULL) { // set curr to head, stop if list empty.
        head = head->next;          // advance head to next element.
        free (curr);                // delete saved pointer.
    }
    
    fSeg *head2 = mice->seg;
    fSeg *curr2;
    while ((curr2 = head2) != NULL) { // set curr to head, stop if list empty.
        head2 = head2->next;          // advance head to next element.
        free (curr2);                // delete saved pointer.
    }

    free(printBuf);
    exit(1);
}

void addPoints(snek *snek, fSeg *seg){
    snek->totalPoints += seg->points;
}

static void reverseDirection(fSeg *ft){
            int newD;
            if(ft->d < 2)
                newD = ft->d + 2;
            else
                newD = ft->d - 2;

            ft->d = newD;
}

static int foodSnekCollision(fSeg *obj, snek *snek){
    //Check for food segment next x/y if collision with snek occurs
    //Up
    if(obj->d == 0){
        segm *head = snek->seg;

        while(head->next != NULL){
            if(((int)obj->x - 1) == head->x && (int)obj->y == head->y)
                return 1;
            head = head->next;
        }
    }
    ////Right
    if(obj->d == 1){
        segm *head = snek->seg;

        while(head->next != NULL){
            if(((int)obj->y + 1) == head->y && (int)obj->x == head->x)
                return 1;
            head = head->next;
        }
    }
    //Down
    if(obj->d == 2){
        segm *head = snek->seg;

        while(head->next != NULL){
            if(((int)obj->x + 1) == head->x && (int)obj->y == head->y)
                return 1;
            head = head->next;
        }
    }
    //Left
    if(obj->d == 3){
        segm *head = snek->seg;

        while(head->next != NULL){
            if(((int)obj->y - 1) == head->y && (int)obj->x == head->x)
                return 1;
            head = head->next;
        }
    }
    return 0;
}

static int foodFieldCollision(fSeg *obj, snek *snek){
        if(obj->d == 1 && (int)(obj->y) >= (snek->cols - 1))
            return 1;
        if(obj->d == 3 && (int)(obj->y) <= 1)
            return 1;
        if(obj->d == 2 && (int)(obj->x) >= (snek->rows - 1))
            return 1;
        if(obj->d == 0 && (int)(obj->x) <= 1)
            return 1;
        return 0;
}

extern void moveFood(food *mice, snek *snek){
    float speed = 0.1; //Hardcoding this for now
    fSeg *ft = mice->seg;

    while(ft != NULL){
        //Food collides with field boundary
        if(foodFieldCollision(ft, snek))
            reverseDirection(ft);
        
        //Food collides with snek
        if(foodSnekCollision(ft, snek))
            reverseDirection(ft);
        
        //Speed multiplier
        int mp = ft->speed;

        //Move segment 
        if(ft->d == 0)
            ft->x -= speed * mp;
        if(ft->d == 2)
            ft->x += speed * mp;
        if(ft->d == 1)
            ft->y += speed * mp;
        if(ft->d == 3)
            ft->y -= speed * mp;

        ft->d = rand() % 4;
        
        ft = ft->next;
    }
}

static int fieldCollision(snek *snek){
        if(snek->seg->d == 1 && (snek->seg->y) == snek->cols){
            return 1;
        }
        if(snek->seg->d == 3 && (snek->seg->y) == -1){
            return 1;
        }
        if(snek->seg->d == 2 && (snek->seg->x) == snek->rows){
            return 1;
        }
        if(snek->seg->d == 0 && (snek->seg->x) == -1){
            return 1;
        }
        return 0;
}

static int selfCollision(snek *snek){
        segm *s = snek->seg->next;

        while(s != NULL){
            if(s->x == snek->seg->x\
                    && s->y == snek->seg->y\
                    )
                return 1;
            s = s->next;
        }
        return 0;
}

//Needs work
void deleteFoodSegment(food *mice, fSeg *seg, snek *snek){
    fSeg *head = mice->seg;
    fSeg *prevSeg = head;

    while(head != NULL){
        if(head == seg){
            //relink previous item in list to next, if needed
            if(head->next != NULL){
                prevSeg->next = head->next;
            }else{
                if(prevSeg != seg){
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

    //New level achieved
    if(foodCnt == 1){
        pushFoodSegments(mice, (rand() % 4) + 1, snek);
        snek->lvl += 1;
    }

    animateSnek(snek, 1);
}

static void addFood(food *mice, snek *snek, char *printBuf, size_t *bufMmb, int *skip, int r, int c){
    fSeg *m = mice->seg;
    size_t foodCnt = 0;
    
    while(m != NULL){
        //Detect colision with "food" and snek head
        if((int) m->x == snek->seg->x && (int) m->y == snek->seg->y){//Verify segments
            m->x += 1000;//move food out of field..
            deleteFoodSegment(mice, m, snek);
            addPoints(snek, m);
            addSegments(snek, 1);
            skip++;
        }else{
            if((int) m->x == r && (int) m->y == c && *skip == 0){//Verify segments
                printBuf[*bufMmb] = m->tok;
                *bufMmb = *bufMmb + 1;
                printBuf[*bufMmb] = ' ';
                *bufMmb = *bufMmb + 1;
                *skip = *skip + 1;;
            }
        }
        foodCnt++;
        m = m->next;
    }
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
        head->next = malloc(1 * sizeof(fSeg));
        //head->next->tok = i+1+'0';
        head->next->tok = '0';
        head->next->d = head->d;
        head->next->x = rand() % snek->rows;
        head->next->y = rand() % snek->cols;
        head->next->speed = rand() % 3;
        head->next->points = 1;
        head->next->next = NULL;

        //Move food a bit from borders
        if(head->next->x == 0 || head->next->x == 1)
            head->next->x+=2;
        if(head->next->y == 0 || head->next->y == 1)
            head->next->y+=2;
        if(head->next->x == snek->rows || head->next->x == (snek->rows - 1))
            head->next->x-=2;
        if(head->next->y == snek->cols || head->next->y == (snek->cols - 1))
            head->next->y-=2;

        head = head->next;
        segCnt++;
    }
}

void spawnFood(size_t amount, food *mice, snek *snek){
    srand(time(NULL));

    mice->d = 0;
    mice->seg = malloc(sizeof(fSeg));
    mice->seg->tok = '0';
    mice->seg->x = rand() % snek->rows;
    mice->seg->y = rand() % snek->cols;
    mice->seg->speed = rand() % 3;
    mice->seg->d = rand() % 4;
    mice->seg->points = 1;
    mice->seg->next = NULL;

    if(mice->seg->x == 0 || mice->seg->x == 1)
        mice->seg->x+=2;
    if(mice->seg->y == 0 || mice->seg->y == 1)
        mice->seg->y+=2;
    if(mice->seg->x == snek->rows || mice->seg->x == (snek->rows - 1))
        mice->seg->x-=2;
    if(mice->seg->y == snek->cols || mice->seg->y == (snek->cols -1))
        mice->seg->y-=2;

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
                    skip++;
                }
                sS = sS->next;
            }

            //Add food to buffer/ field
            addFood(mice, snek, printBuf, &bufMmb, &skip, r, c);

            if(skip == 0){ //Add regular field token
                printBuf[bufMmb] = tok1;
                bufMmb++;
                printBuf[bufMmb] = tok2;
                bufMmb++;
            }
            skip = 0;
        }

        //Detect snek body colision on next coord
        if(selfCollision(snek)){
                printf("Game over, you cannot eat yourself\n");
                exitGame(snek, mice, printBuf);
        }

        //Detect playfield boundary collision
        if(fieldCollision(snek)){
                printf("Game over, you cannot eat the wall\n");
                exitGame(snek, mice, printBuf);
        }

        //Kind of an animation
        if(snek->animate == 1 && snek->animateCnt == 20)
            animateSnek(snek, 0);
        else if(snek->animate == 1 && snek->animateCnt <= 20)
            snek->animateCnt++;

        printBuf[bufMmb] = '\n';
        bufMmb++;
    }

    //Food moves
    moveFood(mice, snek);

    clear();
    printf("%s\n", printBuf);
    printf("\nLevel %d, Points: %d\n", snek->lvl, snek->totalPoints);
    free(printBuf);
}
