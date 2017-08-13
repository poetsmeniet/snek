#ifndef snek_H_
#define snek_H_

typedef struct snakeSegment{
    char tok;
    int x;
    int y;
    size_t d; //Direction
    struct snakeSegment *next;
}segm;

typedef struct snake{
    size_t d; //Direction
    segm *seg;
}snek;

#endif
