//
// Created by jake on 11/9/17.
//

#include <stdio.h>
#include "drawgame.h"
#include "graphics.h"
#include "sprites.h"

void drawStartScreen() {
    drawImage(0, 0, SCR_WIDTH, SCR_HEIGHT, startbkg);
    drawString(125, 63, "Press START to play!", WHITE);
}

void drawLives(int lives) {
    drawRect(LIVES_ROW, LIVES_COL, (LIFE_WIDTH + 1) * 3, LIFE_HEIGHT, BLACK);
    for (int i = 0; i < lives; i++) {
        drawImage(LIVES_ROW, LIVES_COL + i * (LIFE_WIDTH + 1), LIFE_WIDTH, LIFE_HEIGHT, life);
    }
}

void drawScore(int score) {
    drawRect(SCORE_ROW, SCORE_COL, 31, 8, BLACK);
    drawInt(SCORE_ROW, SCORE_COL, score, WHITE);
}

void drawGameOverScreen() {
    drawImage(0, 0, SCR_WIDTH, SCR_HEIGHT, endbkg);
    drawString(145, 45, "Press START to play again!", WHITE);
}

void drawWinScreen(int score) {
    drawImage(0, 0, SCR_WIDTH, SCR_HEIGHT, winbkg);
    char buffer[41];
    sprintf(buffer, "Score: %d", score);
    drawString(130, 85, buffer, WHITE);
    drawString(140, 45, "Press START to play again!", WHITE);
}
