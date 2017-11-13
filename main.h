//
// Created by jake on 11/9/17.
//

#ifndef HW09_3_MAIN_H
#define HW09_3_MAIN_H

#include "game.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef char s8;
typedef short s16;
typedef int s32;

#define OFFSET(r, c, rowlen) ((r) * (rowlen) + (c))
#define LENGTH(arr) (sizeof(arr) / sizeof(*(arr)))

void initGame(GAME *game);

void initLevels(LEVEL *levels);

void moveAliens(LEVEL *level, PLAYER *player);

int getAlienIdleRow(ALIEN *a);

int getAlienIdleCol(ALIEN *a, int animFrame);

int getNextDiveCol(ALIEN *a);

void movePlayer(PLAYER *player);

void moveBullets(LEVEL *level);

void shootIfPossible(PLAYER *player, LEVEL *level);

void destroyBullet(LEVEL *level, int index);

int checkCollisions(LEVEL *level, PLAYER *player, int *scorePointer);

int isCollision(int aRow, int aCol, int aWidth, int aHeight, int bRow, int bCol, int bWidth, int bHeight);

#endif //HW09_3_MAIN_H
