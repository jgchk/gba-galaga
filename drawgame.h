//
// Created by jake on 11/9/17.
//

#ifndef HW09_3_DRAWGAME_H
#define HW09_3_DRAWGAME_H

#define ALIEN_WIDTH 13
#define ALIEN_HEIGHT 13
#define VMARGIN 1
#define HMARGIN 1
#define PLAYER_ROW 125
#define IDLE_ANIM_FRAMES 5
#define STATUS_ROW (SCR_HEIGHT - STATUSBAR_HEIGHT)
#define STATUS_COL 0
#define LIVES_ROW (STATUS_ROW + 5)
#define LIVES_COL (STATUS_COL + 11)
#define LEVELNUM_ROW (STATUS_ROW + 5)
#define LEVELNUM_COL (STATUS_COL + 217)
#define SCORE_ROW (STATUS_ROW + 5)
#define SCORE_COL (STATUS_COL + 181)

void drawStartScreen();

void drawLives(int lives);

void drawScore(int score);

#endif //HW09_3_DRAWGAME_H
