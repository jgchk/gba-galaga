//
// Created by jake on 11/9/17.
//

#include <stdio.h>
#include "drawgame.h"
#include "graphics.h"
#include "sprites.h"

/**
 * Draw the start screen
 */
void drawStartScreen() {
    drawImage(0, 0, SCR_WIDTH, SCR_HEIGHT, startbkg);
    drawString(125, 63, "Press START to play!", WHITE);
}

/**
 * Draw the current amount of lives to the screen
 *
 * @param lives the current number of lives
 */
void drawLives(int lives) {
    drawRect(LIVES_ROW, LIVES_COL, (LIFE_WIDTH + 1) * 3, LIFE_HEIGHT, BLACK); // Erase the last life images
    for (int i = 0; i < lives; i++) { // Loop the number of times given by "lives"
        drawImage(LIVES_ROW, LIVES_COL + i * (LIFE_WIDTH + 1), LIFE_WIDTH, LIFE_HEIGHT, life); // Draw the life image
    }
}

/**
 * Draw the current score to the screen
 *
 * @param score the current game score
 */
void drawScore(int score) {
    drawRect(SCORE_ROW, SCORE_COL, 31, 8, BLACK);
    drawInt(SCORE_ROW, SCORE_COL, score, WHITE);
}

/**
 * Draw the game over screen
 */
void drawGameOverScreen() {
    drawImage(0, 0, SCR_WIDTH, SCR_HEIGHT, endbkg);
    drawString(145, 45, "Press START to play again!", WHITE);
}

/**
 * Draw the win screen, which also shows the final score
 *
 * @param score the final game score
 */
void drawWinScreen(int score) {
    drawImage(0, 0, SCR_WIDTH, SCR_HEIGHT, winbkg);
    char buffer[41];
    sprintf(buffer, "Score: %d", score);
    drawString(130, 85, buffer, WHITE);
    drawString(140, 45, "Press START to play again!", WHITE);
}
