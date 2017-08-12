#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define clear() printf("\033[H\033[J") //to clear the linux term

typedef struct snakeSegment{
    char tok;
    size_t x;
    size_t y;
    size_t d; //Direction
    struct snakeSegment *next;
}segm;

typedef struct snake{
    size_t x;
    size_t y;
    size_t d; //Direction
    segm *seg;
}snek;

void printField(size_t cols, size_t rows, snek *snek);

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
        head->next->tok = segCnt+'0';
        head->next->d = head->d;
        head->next->x = head->x;
        head->next->y = head->y - 1;
        head->next->next = NULL;

        head = head->next;
        segCnt++;
    }
}

int main(void){
    snek snek;
    snek.x = 5;
    snek.y = 15;
    //initial segment
    snek.seg = malloc(1 * sizeof(segm));
    snek.seg->tok = 'O';
    snek.seg->x = 5;
    snek.seg->y = 15;
    snek.seg->d = 2;
    snek.seg->next = NULL;

    addSegments(&snek, 2);

    while(1){
        printField(50, 30, &snek);
        snek.seg->y++;
        sleep(1);
    }
}

void printField(size_t cols, size_t rows, snek *snek){
    //clear();
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
                }else{
                    //skip = 0;
                }
                
                sS = sS->next;
            }
            if(skip == 0)
            printf("%c ", tok);
            skip = 0;


            //if(snek->x == r && snek->y == c\
            //        )
            //    printf("%c ", snek->seg->next->tok);
            //else
            //    printf("%c ", tok);
        }
        printf("\n");
    }
    printf("\n");
}
