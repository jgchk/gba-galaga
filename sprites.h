/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=3 sprites sprites/bullet_0.bmp sprites/life.bmp sprites/pa_0.bmp sprites/player_0.bmp sprites/space_bkg.bmp sprites/startbkg.bmp sprites/statusbar.bmp sprites/da_0.bmp 
 * Time-stamp: Sunday 11/12/2017, 14:20:10
 * 
 * Image Information
 * -----------------
 * sprites/bullet_0.bmp 3@8
 * sprites/life.bmp 7@7
 * sprites/pa_0.bmp 13@13
 * sprites/player_0.bmp 15@16
 * sprites/space_bkg.bmp 240@160
 * sprites/startbkg.bmp 240@160
 * sprites/statusbar.bmp 240@16
 * sprites/da_0.bmp 13@13
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES_H
#define SPRITES_H

extern const unsigned short bullet_0[24];
#define BULLET_0_SIZE 48
#define BULLET_0_LENGTH 24
#define BULLET_0_WIDTH 3
#define BULLET_0_HEIGHT 8

extern const unsigned short life[49];
#define LIFE_SIZE 98
#define LIFE_LENGTH 49
#define LIFE_WIDTH 7
#define LIFE_HEIGHT 7

extern const unsigned short pa_0[169];
#define PA_0_SIZE 338
#define PA_0_LENGTH 169
#define PA_0_WIDTH 13
#define PA_0_HEIGHT 13

extern const unsigned short player_0[240];
#define PLAYER_0_SIZE 480
#define PLAYER_0_LENGTH 240
#define PLAYER_0_WIDTH 15
#define PLAYER_0_HEIGHT 16

extern const unsigned short space_bkg[38400];
#define SPACE_BKG_SIZE 76800
#define SPACE_BKG_LENGTH 38400
#define SPACE_BKG_WIDTH 240
#define SPACE_BKG_HEIGHT 160

extern const unsigned short startbkg[38400];
#define STARTBKG_SIZE 76800
#define STARTBKG_LENGTH 38400
#define STARTBKG_WIDTH 240
#define STARTBKG_HEIGHT 160

extern const unsigned short statusbar[3840];
#define STATUSBAR_SIZE 7680
#define STATUSBAR_LENGTH 3840
#define STATUSBAR_WIDTH 240
#define STATUSBAR_HEIGHT 16

extern const unsigned short da_0[169];
#define DA_0_SIZE 338
#define DA_0_LENGTH 169
#define DA_0_WIDTH 13
#define DA_0_HEIGHT 13

#endif

