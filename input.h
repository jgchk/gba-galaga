//
// Created by jake on 11/8/17.
//

#ifndef HW09_2_INPUT_H
#define HW09_2_INPUT_H

#include "main.h"

#define BUTTONS *(volatile u32 *) 0x4000130
#define KEYS (~(BUTTONS) & 0x03FF)
#define KEY_DOWN(key) (~(BUTTONS) & (key))
#define BUTTON_A        (1<<0)
#define BUTTON_B        (1<<1)
#define BUTTON_SELECT   (1<<2)
#define BUTTON_START    (1<<3)
#define BUTTON_RIGHT    (1<<4)
#define BUTTON_LEFT     (1<<5)
#define BUTTON_UP       (1<<6)
#define BUTTON_DOWN     (1<<7)
#define BUTTON_R        (1<<8)
#define BUTTON_L        (1<<9)

void keyPoll();

u16 keyHit(u16 key);

#endif //HW09_2_INPUT_H
