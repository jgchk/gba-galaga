//
// Created by jake on 11/9/17.
//

#include "drawgame.h"
#include "graphics.h"
#include "sprites.h"

void drawStartScreen() {
    drawImage(0, 0, SCR_WIDTH, SCR_HEIGHT, startbkg);
    drawString(125, 63, "Press START to play!", WHITE);
}

void drawLives(int lives) {
    for (int i = 0; i < lives; i++) {
        drawImage(LIVES_ROW, LIVES_COL + i * (LIFE_WIDTH + 1), LIFE_WIDTH, LIFE_HEIGHT, life);
    }
}

void drawScore(int score) {
    drawRect(SCORE_ROW, SCORE_COL, 31, 8, BLACK);
    drawInt(SCORE_ROW, SCORE_COL, score, WHITE);
}
