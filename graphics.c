//
// Created by jake on 11/9/17.
//

#include <stdio.h>
#include "graphics.h"

u16 *videoBuffer = (u16 *) 0x6000000;

void initGraphics() {
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
}

void waitForVblank() {
    while (SCANLINECOUNTER > 160);
    while (SCANLINECOUNTER < 160);
}

void drawRect(int row, int col, int width, int height, u16 color) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[3].cnt = (u32) (width | DMA_ON | DMA_SOURCE_FIXED);
    }
}

void drawImage(int row, int col, int width, int height, const u16 *image) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &image[OFFSET(r, 0, width)];
        DMA[3].dst = &videoBuffer[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[3].cnt = (u32) (width | DMA_ON);
    }
}

void drawBkg(int row, int col, int width, int height, const u16 *image) {
    for (int r = 0; r < height; r++) {
        DMA[2].src = &image[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[2].dst = &videoBuffer[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[2].cnt = (u32) (width | DMA_ON);
    }
}

void setPixel(int row, int col, u16 color) {
    videoBuffer[OFFSET(row, col, SCR_WIDTH)] = color;
}

void drawChar(int row, int col, char ch, u16 color) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 6; c++) {
            if (fontdata_6x8[OFFSET(r, c, 6) + ch * 48]) {
                setPixel(row + r, col + c, color);
            }
        }
    }
}

void drawString(int row, int col, char str[], u16 color) {
    while (*str) {
        drawChar(row, col, *str++, color);
        col += 6;
    }
}

void drawInt(int row, int col, int n, u16 color) {
    char buffer[41];
    sprintf(buffer, "%d", n);
    drawString(row, col, buffer, color);
}
