#ifndef stage_H_
#define stage_H_

typedef struct foodSegments{
    char tok;
    float x;
    float y;
    size_t d; //Direction
    float speed;
    struct foodSegments *next;
}fSeg;

typedef struct food{
    size_t d; //Direction
    fSeg *seg;
}food;

void printField(int cols, int rows, snek *snek, food *mice);
void spawnFood(size_t amount, food *mice, snek *snek);
void pushFoodSegments(food *mice, size_t amount, snek *snek);
void moveFood(food *mice, snek *snek);

#endif
