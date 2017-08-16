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
    pthread_mutex_t moveLock;
    segm *seg;
}snek;

void addSegments(snek *snek, size_t amount);
void moveSnek(snek *snek);
void initSnek(snek *snek);

#endif
