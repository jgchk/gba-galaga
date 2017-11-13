//
// Created by jake on 11/8/17.
//

#include "input.h"

u16 keyCurr = 0, keyPrev = 0;

void keyPoll() {
    keyPrev = keyCurr;
    keyCurr = (u16) KEYS;
}

u16 keyHit(u16 key) {
    return (keyCurr & ~keyPrev) & key;
}
