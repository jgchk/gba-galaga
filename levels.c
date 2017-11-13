//
// Created by jake on 11/9/17.
//

#include "game.h"
#include "main.h"

ALIEN lv0Aliens[8] = {{0, 6, DIVE, 0, 0, 0}, {0, 7, DIVE, 0, 0, 0}, {0, 8, DIVE, 0, 0, 0}, {0, 9, DIVE, 0, 0, 0},
                       {1, 6, PASSIVE, 0, 0, 0}, {1, 7, PASSIVE, 0, 0, 0}, {1, 8, PASSIVE, 0, 0, 0}, {1, 9, PASSIVE, 0, 0, 0}};
BULLET lv0B[4];
LEVEL lv0 = {.levelNum = 0, .animFrame = 0, .animDir = 1, .aliens = lv0Aliens, .numAliens = LENGTH(lv0Aliens),
        .bullets = lv0B, .numBullets = 0};
