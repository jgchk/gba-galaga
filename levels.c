//
// Created by jake on 11/9/17.
//

#include "game.h"
#include "main.h"

ALIEN lv0A[8];
BULLET lv0B[4];
LEVEL lv0 = {.animFrame = 0, .animDir = 1, .aliens = lv0A, .numAliens = LENGTH(
        lv0A), .bullets = lv0B, .numBullets = 0, .diveChance = 1000};

ALIEN lv1A[8];
BULLET lv1B[4];
LEVEL lv1 = {.animFrame = 0, .animDir = 1, .aliens = lv1A, .numAliens = LENGTH(
        lv1A), .bullets = lv1B, .numBullets = 0, .diveChance = 500};

ALIEN lv2A[8];
BULLET lv2B[4];
LEVEL lv2 = {.animFrame = 0, .animDir = 1, .aliens = lv2A, .numAliens = LENGTH(
        lv2A), .bullets = lv2B, .numBullets = 0, .diveChance = 100};

void initLevelContents() {
    for (int i = 6; i <= 9; i++) {
        lv0A[i - 6] = (ALIEN) {0, i, DIVE, 0, 0, 0};
        lv0A[i - 2] = (ALIEN) {1, i, PASSIVE, 0, 0, 0};
    }

    lv1A[0] = (ALIEN) {0, 2, DIVE, 0, 0, 0};
    lv1A[1] = (ALIEN) {0, 3, DIVE, 0, 0, 0};
    lv1A[2] = (ALIEN) {1, 3, DIVE, 0, 0, 0};
    lv1A[3] = (ALIEN) {0, 13, DIVE, 0, 0, 0};
    lv1A[4] = (ALIEN) {0, 14, DIVE, 0, 0, 0};
    lv1A[5] = (ALIEN) {1, 13, DIVE, 0, 0, 0};
    lv1A[6] = (ALIEN) {0, 7, PASSIVE, 0, 0, 0};
    lv1A[7] = (ALIEN) {0, 9, PASSIVE, 0, 0, 0};

    for (int i = 0; i < 8; i++) {
        lv2A[i] = (ALIEN) {0, i + 4, DIVE, 0, 0, 0};
    }
}

//ALIEN lv1A[8] = {};
