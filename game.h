//
// Created by jake on 11/9/17.
//

#ifndef HW09_3_GAME_H
#define HW09_3_GAME_H

#define NUM_ROWS 5
#define NUM_COLS 10

#define NUM_LEVELS 3

#define COOLDOWN_THRESHOLD 10
#define BULLET_COOLDOWN 15

#define IDLE_ALIEN_SCORE 100
#define DIVE_ALIEN_SCORE 300
#define DIVE_MISS_SCORE (-500)
#define SHOOT_SCORE (-25)

typedef struct Player {
    int pos;
    int lives;
    int score;
    int cooldown;
} PLAYER;

enum AlienType {
    PASSIVE,
    DIVE
};

typedef struct Alien {
    int y;
    int x;
    int type;
    int diveRow;
    int diveCol;
    int diveTarget;
} ALIEN;

//typedef struct DiveAlien {
//    int y;
//    int x;
//    int diveCol;
//} DIVE_ALIEN;

typedef struct Bullet {
    int row;
    int col;
} BULLET;

typedef struct Level {
    int animFrame;
    int animDir;
    ALIEN *aliens;
    int numAliens;
    BULLET *bullets;
    int numBullets;
    int diveChance;
} LEVEL;

typedef struct Game {
    PLAYER player;
    int currLevel;
} GAME;

#endif //HW09_3_GAME_H
