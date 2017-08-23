#ifndef snek_H_
#define snek_H_

typedef struct snakeSegment{
    char tok;
    int x;
    int y;
    size_t d; //Direction
    struct snakeSegment *next;
}segm;

typedef struct snakeSegmentPointers{
    segm *segmP;
}segmPs;

typedef struct snake{
    int cols;
    int rows;
    size_t d; //Direction
    size_t animate;
    size_t animateCnt;
    size_t totalPoints;
    pthread_mutex_t moveLock;
    segm *seg;
}snek;

void addSegments(snek *snek, size_t amount);
void moveSnek(snek *snek);
void initSnek(snek *snek, int COLS, int ROWS);
extern void animateSnek(snek *snek, size_t type);

#endif
