#include "game.h"
#include "input.h"
#include "graphics.h"
#include "levels.h"
#include "drawgame.h"
#include "sprites.h"
#include <stdlib.h>

enum GBAState {
    START,              // Set up GBA graphics and draw the start screen
    START_NODRAW,       // Wait for the player to hit START
    RESET_GAME,         // Reset all game stats (score, alien positions, etc.)
    DRAW_NEW_LEVEL,     // Draw the current level to the screen
    NEW_LEVEL_NODRAW,   // Wait for the player to hit START
    LEVEL_LOOP,         // The main game loop
    GAME_OVER,          // Draw the game over screen
    WIN,                // Draw the win screen
    END_SCREEN_NODRAW   // Wait for the player to hit START to restart
};

int main() {
    GAME game; // Keep track of game stats
    LEVEL levels[NUM_LEVELS]; // Store levels
    enum GBAState state = START; // Store current state

    while (1) {
        keyPoll(); // Update our current and last keys pressed
        waitForVblank(); // Wait for v blank to prevent tearing

        if (keyHit(BUTTON_SELECT)) { // Check for reset key
            state = START;
        }

        switch (state) {
            case START:
                drawStartScreen();
                initGraphics(); // Set up Mode 3 and BG2
                state = START_NODRAW;
                break;
            case START_NODRAW:
                if (keyHit(BUTTON_START)) {
                    state = RESET_GAME;
                }
                break;
            case RESET_GAME:
                initGame(&game); // Set level, player stats, etc. to default values
                initLevels(levels); // Load level data
                state = DRAW_NEW_LEVEL;
                break;
            case DRAW_NEW_LEVEL:
                drawBkg(0, 0, SCR_WIDTH, SCR_HEIGHT, space_bkg);
                drawImage(STATUS_ROW, STATUS_COL, STATUSBAR_WIDTH, STATUSBAR_HEIGHT, statusbar);
                drawInt(LEVELNUM_ROW, LEVELNUM_COL, game.currLevel + 1, WHITE);
                drawLives(game.player.lives);
                drawScore(game.player.score);
                drawString(PRESS_START_ROW, PRESS_START_COL, "Press START!", WHITE);
                state = NEW_LEVEL_NODRAW;
                break;
            case NEW_LEVEL_NODRAW:
                if (keyHit(BUTTON_START)) {
                    drawBkg(PRESS_START_ROW, PRESS_START_COL, 71, 7, space_bkg);
                    state = LEVEL_LOOP;
                }
                break;
            case LEVEL_LOOP:;
                LEVEL *level = &levels[game.currLevel];
                if (level->numAliens == 0) { // If all aliens have died, go to next level
                    game.currLevel++;
                    if (game.currLevel == NUM_LEVELS) { // If we have run out of levels, go to win screen
                        state = WIN;
                    } else { // Otherwise just draw the next level
                        state = DRAW_NEW_LEVEL;
                    }
                    break;
                }
                moveAliens(level, &game.player); // Calculated and draw new alien positions
                movePlayer(&game.player); // Calculate and draw new player position
                moveBullets(level); // Calculate and draw new bullet positions
                // Check for bullet and player collisions with aliens
                // lostLife will be 1 if the player has collided with an alien, or 0 if not
                int lostLife = checkCollisions(level, game.player.pos, &game.player.score);
                if (lostLife) { // If the player lost a life, decrement lives and check if all lives have been lost
                    game.player.lives--;
                    resetDiveAliens(level);
                    if (game.player.lives == 0) { // If no more lives, go to game over
                        state = GAME_OVER;
                    } else { // Otherwise, redraw the level and keep going
                        state = DRAW_NEW_LEVEL;
                    }
                    break;
                }
                shootIfPossible(&game.player, level); // Spawn bullets if shooting
                break;
            case GAME_OVER:
                drawGameOverScreen();
                state = END_SCREEN_NODRAW;
                break;
            case WIN:
                drawWinScreen(game.player.score);
                state = END_SCREEN_NODRAW;
                break;
            case END_SCREEN_NODRAW:
                if (keyHit(BUTTON_START)) { // Wait for the player to hit START then reset
                    state = RESET_GAME;
                }
                break;
        }
    }
}

/**
 * Initialize default game values
 *
 * @param game the GAME struct
 */
void initGame(GAME *game) {
    PLAYER player = {.pos = 0, .lives = 3, .score = 0, .cooldown = 0};
    *game = (GAME) {player, 0};
}

/**
 * Load level data into the level array
 *
 * @param levels the level array
 */
void initLevels(LEVEL *levels) {
    initLevelContents();
    levels[0] = lv0;
    levels[1] = lv1;
    levels[2] = lv2;
}

/**
 * Calculate new alien positions and redraw them
 *
 * @param level pointer to the current LEVEL
 * @param player pointer to the PLAYER struct
 */
void moveAliens(LEVEL *level, PLAYER *player) {
    for (int i = 0; i < level->numAliens; i++) {
        ALIEN *a = &level->aliens[i];
        if (a->type == DIVE) {
            if (a->diveTarget) {
                drawBkg(a->diveRow, a->diveCol, ALIEN_WIDTH, ALIEN_HEIGHT, space_bkg);
                a->diveRow++;
                a->diveCol += getNextDiveCol(a);
                if (a->diveRow == SCR_HEIGHT - STATUSBAR_HEIGHT - ALIEN_HEIGHT + 1) {
                    a->diveTarget = 0;
                    a->diveCol = 0;
                    a->diveRow = 0;
                    updateScore(&player->score, DIVE_MISS_SCORE);
                }
            } else {
                drawBkg(getAlienIdleRow(a), getAlienIdleCol(a, level->animFrame), ALIEN_WIDTH, ALIEN_HEIGHT,
                        space_bkg);
                int random = rand() % level->diveChance;
                if (random == 0) {
                    a->diveTarget = player->pos;
                    a->diveRow = getAlienIdleRow(a);
                    a->diveCol = getAlienIdleCol(a, level->animFrame);
                }
            }
        } else {
            drawBkg(getAlienIdleRow(a), getAlienIdleCol(a, level->animFrame), ALIEN_WIDTH, ALIEN_HEIGHT,
                    space_bkg);
        }
    }

    if (level->animDir == 1) {
        level->animFrame++;
    } else {
        level->animFrame--;
    }

    if (level->animFrame == IDLE_ANIM_FRAMES * 11) {
        level->animDir = -1;
    } else if (level->animFrame == 3) {
        level->animDir = 1;
    }

    for (int i = 0; i < level->numAliens; i++) {
        ALIEN *a = &level->aliens[i];
        if (a->type == DIVE) {
            if (a->diveTarget) {
                drawImage(a->diveRow, a->diveCol, ALIEN_WIDTH, ALIEN_HEIGHT, da_0);
            } else {
                drawImage(getAlienIdleRow(a), getAlienIdleCol(a, level->animFrame), ALIEN_WIDTH, ALIEN_HEIGHT, da_0);
            }
        } else {
            drawImage(getAlienIdleRow(a), getAlienIdleCol(a, level->animFrame), ALIEN_WIDTH, ALIEN_HEIGHT, pa_0);
        }
    }
}

/**
 * Calculate the screen row to draw the given ALIEN when in the idle state
 *
 * @param a pointer to an ALIEN struct
 * @return the screen row to draw the given ALIEN
 */
int getAlienIdleRow(ALIEN *a) {
    return a->y * (ALIEN_HEIGHT + VMARGIN);
}

/**
 * Calculate the screen column to draw the given ALIEN when in the idle state
 *
 * @param a pointer to an ALIEN struct
 * @param animFrame the current idle animation frame
 * @return the screen column to draw the given ALIEN
 */
int getAlienIdleCol(ALIEN *a, int animFrame) {
    return a->x * (ALIEN_WIDTH + HMARGIN) + animFrame / 10;
}

/**
 * Calculate the column increment for an ALIEN in the diving state
 *
 * @param a pointer to an ALIEN struct
 * @return the column increment for the given ALIEN
 */
int getNextDiveCol(ALIEN *a) {
    return (a->diveTarget - a->diveCol) / abs(a->diveTarget - a->diveCol);
}

/**
 * Calculate new PLAYER position and redraw the PLAYER
 *
 * @param player pointer to the PLAYER struct
 */
void movePlayer(PLAYER *player) {
    drawBkg(PLAYER_ROW, player->pos, PLAYER_0_WIDTH, PLAYER_0_HEIGHT, space_bkg);
    if (KEY_DOWN(BUTTON_LEFT) && player->pos > 1) {
        player->pos--;
    }
    if (KEY_DOWN(BUTTON_RIGHT) && player->pos < (SCR_WIDTH - PLAYER_0_WIDTH)) {
        player->pos++;
    }
    drawImage(PLAYER_ROW, player->pos, PLAYER_0_WIDTH, PLAYER_0_HEIGHT, player_0);
}

/**
 * Calculate new BULLET positions and redraw them
 *
 * @param level pointer to the current LEVEL
 */
void moveBullets(LEVEL *level) {
    for (int i = 0; i < level->numBullets; i++) {
        BULLET *bullet = &level->bullets[i];
        drawBkg(bullet->row, bullet->col, BULLET_0_WIDTH, BULLET_0_HEIGHT, space_bkg);
        bullet->row -= 4;
        if (bullet->row < 0) {
            destroyBullet(level, i);
        } else {
            drawImage(bullet->row, bullet->col, BULLET_0_WIDTH, BULLET_0_HEIGHT, bullet_0);
        }
    }
}

/**
 * Determine whether the player can shoot, and whether they are pressing the button to do so. If so, spawn
 * a new bullet in the level.
 *
 * @param player pointer to the PLAYER struct
 * @param level pointer to the current LEVEL
 */
void shootIfPossible(PLAYER *player, LEVEL *level) {
    if (keyHit(BUTTON_A) && player->cooldown < COOLDOWN_THRESHOLD) {
        BULLET bullet = {.row = PLAYER_ROW - PLAYER_0_HEIGHT + 7, .col = player->pos + PLAYER_0_WIDTH / 2 - 1};
        drawImage(bullet.row, bullet.col, BULLET_0_WIDTH, BULLET_0_HEIGHT, bullet_0);
        level->bullets[level->numBullets++] = bullet;
        player->cooldown += BULLET_COOLDOWN;
        updateScore(&player->score, SHOOT_SCORE);
    }
    if (player->cooldown > 0) {
        player->cooldown--;
    }
}

/**
 * Remove a BULLET from the LEVEL
 *
 * @param level pointer to the current LEVEL
 * @param index the index of the BULLET to destroy in the LEVEL's BULLET array
 */
void destroyBullet(LEVEL *level, int index) {
    BULLET *b = &level->bullets[index];
    drawBkg(b->row, b->col, BULLET_0_WIDTH, BULLET_0_HEIGHT, space_bkg);
    for (int i = index; i < level->numBullets - 1; i++) {
        level->bullets[i] = level->bullets[i + 1];
    }
    level->numBullets--;
}

/**
 * Remove an ALIEN from the LEVEL
 *
 * @param level pointer to the current LEVEL
 * @param index the index of the ALIEN to destroy in the LEVEL's ALIEN array
 */
void destroyAlien(LEVEL *level, int index) {
    ALIEN *a = &level->aliens[index];
    if (a->diveTarget) {
        drawBkg(a->diveRow, a->diveCol, PA_0_WIDTH, PA_0_HEIGHT, space_bkg);
    } else {
        drawBkg(getAlienIdleRow(a), getAlienIdleCol(a, level->animFrame), PA_0_WIDTH, PA_0_HEIGHT, space_bkg);
    }
    for (int i = index; i < level->numAliens - 1; i++) {
        level->aliens[i] = level->aliens[i + 1];
    }
    level->numAliens--;
}

/**
 * Check for collisions between BULLETs and ALIENs and between ALIENs and the PLAYER
 *
 * @param level pointer to the current LEVEL
 * @param playerCol the screen column of the PLAYER
 * @param scorePointer pointer to the game score
 * @return 1 if the PLAYER collided with an ALIEN, 0 if not
 */
int checkCollisions(LEVEL *level, int playerCol, int *scorePointer) {
    for (int a = 0; a < level->numAliens; a++) {
        ALIEN *alien = &level->aliens[a];
        if (alien->type == DIVE) {
            int playerCollision = isCollision(PLAYER_ROW, playerCol, PLAYER_0_WIDTH, PLAYER_0_HEIGHT, alien->diveRow,
                                              alien->diveCol, ALIEN_WIDTH, ALIEN_HEIGHT);
            if (playerCollision) {
                destroyAlien(level, a);
                return 1;
            }
        }

        for (int b = 0; b < level->numBullets; b++) {
            BULLET *bullet = &level->bullets[b];
            int bulletCollision;
            int score;
            if (alien->diveTarget) {
                bulletCollision = isCollision(alien->diveRow, alien->diveCol, ALIEN_WIDTH, ALIEN_HEIGHT, bullet->row,
                                              bullet->col, BULLET_0_WIDTH, BULLET_0_HEIGHT);
                score = DIVE_ALIEN_SCORE;
            } else {
                bulletCollision = isCollision(getAlienIdleRow(alien), getAlienIdleCol(alien, level->animFrame),
                                              ALIEN_WIDTH, ALIEN_HEIGHT, bullet->row, bullet->col, BULLET_0_WIDTH,
                                              BULLET_0_HEIGHT);
                score = IDLE_ALIEN_SCORE;
            }
            if (bulletCollision) {
                destroyBullet(level, b);
                destroyAlien(level, a);
                updateScore(scorePointer, score);
            }
        }
    }

    return 0;
}

/**
 * 2D collision detection algorithm between object A and B
 *
 * @param aRow the screen row of object A
 * @param aCol the screen column of object A
 * @param aWidth the width of object A
 * @param aHeight the height of object A
 * @param bRow the screen row of object B
 * @param bCol the screen column of object B
 * @param bWidth the width of object B
 * @param bHeight the height of object B
 * @return 1 if there is a collision, 0 if not
 */
int isCollision(int aRow, int aCol, int aWidth, int aHeight, int bRow, int bCol, int bWidth, int bHeight) {
    if (aCol < bCol + bWidth - 1
        && aCol + aWidth - 1 > bCol
        && aRow < bRow + bHeight - 1
        && aRow + aHeight - 1 > bRow) {
        return 1;
    }
    return 0;
}

/**
 * Reset the diving-related variables for all diving ALIENS in a LEVEL
 *
 * @param level pointer to the current LEVEL
 */
void resetDiveAliens(LEVEL *level) {
    for (int i = 0; i < level->numAliens; i++) {
        level->aliens[i].diveTarget = 0;
        level->aliens[i].diveRow = 0;
        level->aliens[i].diveCol = 0;
    }
}

/**
 * Update and redraw the score
 *
 * @param scorePointer pointer to the game score
 * @param score the amount to add to the current score
 */
void updateScore(int *scorePointer, int score) {
    *scorePointer += score;
    if (*scorePointer < 0) {
        *scorePointer = 0;
    }
    drawScore(*scorePointer);
}
