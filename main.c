#include "game.h"
#include "input.h"
#include "graphics.h"
#include "levels.h"
#include "drawgame.h"
#include "sprites.h"
#include <stdlib.h>

enum GBAState {
    START,
    START_NODRAW,
    RESET_GAME,
    DRAW_NEW_LEVEL,
    NEW_LEVEL_NODRAW,
    LEVEL_LOOP,
    GAME_OVER,
    GAME_OVER_NODRAW,
    WIN,
    WIN_NODRAW
};

int main() {
    GAME game;
    LEVEL levels[NUM_LEVELS];
    enum GBAState state = START;

    while (1) {
        keyPoll();
        waitForVblank();

//        drawRect(0, SCR_WIDTH - 20, 10, 10, BLACK);
//        drawInt(0, SCR_WIDTH - 20, levels[0].aliens[0].diveCol, WHITE);

        if (keyHit(BUTTON_SELECT)) {
            state = START;
        }

        switch (state) {
            case START:
                drawStartScreen();
                initGraphics();
//                game.currLevel = 0;
//                game.player.lives = 3;
                state = START_NODRAW;
                break;
            case START_NODRAW:
                if (keyHit(BUTTON_START)) {
                    state = RESET_GAME;
                }
                break;
            case RESET_GAME:
                initGame(&game);
                initLevels(levels);
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
                if (level->numAliens == 0) {
                    game.currLevel++;
                    if (game.currLevel == NUM_LEVELS) {
                        state = WIN;
                    } else {
                        state = DRAW_NEW_LEVEL;
                    }
                    break;
                }
                moveAliens(level, &game.player);
                movePlayer(&game.player);
                moveBullets(level);
                int lostLife = checkCollisions(level, game.player.pos, &game.player.score);
                if (lostLife) {
                    game.player.lives--;
                    resetDiveAliens(level);
                    if (game.player.lives == 0) {
                        state = GAME_OVER;
                    } else {
                        state = DRAW_NEW_LEVEL;
                    }
                    break;
                }
                shootIfPossible(&game.player, level);
                break;
            case GAME_OVER:
                drawGameOverScreen();
                state = GAME_OVER_NODRAW;
                break;
            case GAME_OVER_NODRAW:
                if (keyHit(BUTTON_START)) {
                    state = RESET_GAME;
                }
                break;
            case WIN:
                drawWinScreen(game.player.score);
                state = WIN_NODRAW;
                break;
            case WIN_NODRAW:
                if (keyHit(BUTTON_START)) {
                    state = RESET_GAME;
                }
                break;
        }
    }
}

void initGame(GAME *game) {
    PLAYER player = {.pos = 0, .lives = 3, .score = 0, .cooldown = 0};
    *game = (GAME) {player, 0};
}

void initLevels(LEVEL *levels) {
    initLevelContents();
    levels[0] = lv0;
    levels[1] = lv1;
    levels[2] = lv2;
}

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

int getAlienIdleRow(ALIEN *a) {
    return a->y * (ALIEN_HEIGHT + VMARGIN);
}

int getAlienIdleCol(ALIEN *a, int animFrame) {
    return a->x * (ALIEN_WIDTH + HMARGIN) + animFrame / 10;
}

int getNextDiveCol(ALIEN *a) {
    return (a->diveTarget - a->diveCol) / abs(a->diveTarget - a->diveCol);
}

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

void destroyBullet(LEVEL *level, int index) {
    BULLET *b = &level->bullets[index];
    drawBkg(b->row, b->col, BULLET_0_WIDTH, BULLET_0_HEIGHT, space_bkg);
    for (int i = index; i < level->numBullets - 1; i++) {
        level->bullets[i] = level->bullets[i + 1];
    }
    level->numBullets--;
}

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

int isCollision(int aRow, int aCol, int aWidth, int aHeight, int bRow, int bCol, int bWidth, int bHeight) {
    if (aCol < bCol + bWidth - 1
        && aCol + aWidth - 1 > bCol
        && aRow < bRow + bHeight - 1
        && aRow + aHeight - 1 > bRow) {
        return 1;
    }
    return 0;
}

void resetDiveAliens(LEVEL *level) {
    for (int i = 0; i < level->numAliens; i++) {
        level->aliens[i].diveTarget = 0;
        level->aliens[i].diveRow = 0;
        level->aliens[i].diveCol = 0;
    }
}

void updateScore(int *scorePointer, int score) {
    *scorePointer += score;
    if (*scorePointer < 0) {
        *scorePointer = 0;
    }
    drawScore(*scorePointer);
}
