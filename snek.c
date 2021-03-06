#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "snek.h"

extern void animateSnek(snek *snek, size_t type){
    segm *head = snek->seg->next;
    char tok = 'O';

    if(type == 1){
        tok = '@';
        snek->animate = 1;
    }else{
        snek->animateCnt = 0;
        snek->animate = 0;
    }

    while(head->next != NULL){
        head->tok = tok;
        head = head->next;
    }
    head->tok = tok;
}

size_t getListSize(void *list){
    
    segm *head = (segm*) list;
    size_t sz = 0; //Segment counter

    while(head->next != NULL){
        head = head->next;
        sz++;
    }
    sz++;

    return sz;
}

extern void initSnek(snek *snek, int COLS, int ROWS){
    //initial segment of snek
    snek->cols = COLS; //storing field dimensions here for convenience
    snek->rows = ROWS;
    snek->seg = malloc(1 * sizeof(segm));
    snek->seg->tok = '>';
    snek->seg->x = 2;
    snek->seg->y = 5;
    snek->seg->d = 1;
    snek->totalPoints = 0;
    snek->animate = 0;
    snek->animateCnt = 0;
    snek->lvl= 1;
    snek->seg->next = NULL;
    pthread_mutex_init(&snek->moveLock, NULL);
}

extern void addSegments(snek *snek, size_t amount){
    segm *head = snek->seg;
    segm *prev = snek->seg;
    size_t segCnt = 0; //Segment counter

    while(head->next != NULL){
        head = head->next;
        prev = head;
        segCnt++;
    }
    segCnt++;
    
    size_t i;

    for(i = 0; i < amount; i++){
        head->next = malloc(1 * sizeof(segm));
        head->next->tok = 'O';
        head->next->d = prev->d;
        if(prev->d == 0){
            head->next->x = head->x + 1;
            head->next->y = head->y;
        }
        if(prev->d == 1){
            head->next->x = head->x;
            head->next->y = head->y - 1;
        }
        if(prev->d == 2){
            head->next->x = head->x - 1;
            head->next->y = head->y;
        }
        if(prev->d == 3){
            head->next->x = head->x;
            head->next->y = head->y + 1;
        }
        head->next->next = NULL;

        head = head->next;
        segCnt++;
    }
}

void alignNextSegment(snek *snek){ //Creates that "snek" movement
    segm *algnSeg = snek->seg;
    size_t sz = getListSize(snek->seg);
    segmPs segmArr[sz];
    int i = 0;

    while(algnSeg->next != NULL){
        //add to pointer array (for reverse d assignment)
        segmArr[i].segmP = algnSeg;
        algnSeg = algnSeg->next;
        i++;
    }
    segmArr[i].segmP = algnSeg;

    size_t j;
    
    for(j = i; j > 0; j--){//Align direction of segment
        if(segmArr[j].segmP->d != segmArr[j - 1].segmP->d)
            segmArr[j].segmP->d = segmArr[j - 1].segmP->d;
    }
}

extern void moveSnek(snek *snek){
    //d (direction):
    // 0: north, up
    // 1: east, right
    // 2: south, down
    // 3: west: left    

    //Mutex lock, locks movement and direction changes by user
    //pthread_mutex_lock(&snek->moveLock);
    segm *segHead = snek->seg;

    while(segHead->next != NULL){
        //Move applicable segment
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
    
    //align next segment of snek
    alignNextSegment(snek);

    //pthread_mutex_unlock(&snek->moveLock);
}
