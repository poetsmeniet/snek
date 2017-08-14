#ifndef stage_H_
#define stage_H_

typedef struct foodSegments{
    char tok;
    int x;
    int y;
    size_t d; //Direction
    struct foodSegments *next;
}fSeg;

typedef struct food{
    size_t d; //Direction
    fSeg *seg;
}food;

void printField(int cols, int rows, snek *snek, food *mice);
void spawnFood(size_t amount, food *mice);
void pushFoodSegments(food *mice, size_t amount);

#endif
