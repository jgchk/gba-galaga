//
// Created by jake on 11/9/17.
//

#include <stdio.h>
#include "graphics.h"

u16 *videoBuffer = (u16 *) 0x6000000;

/**
 * Set up Mode 3 and BG2
 */
void initGraphics() {
    REG_DISPCNT = MODE_3 | BG2_ENABLE;
}

/**
 * Wait for GBA V Blank to prevent tearing
 */
void waitForVblank() {
    while (SCANLINECOUNTER > 160);
    while (SCANLINECOUNTER < 160);
}

/**
 * Draw a colored rectangle on screen using DMA
 *
 * @param row the row to draw the rectangle
 * @param col the column to draw the rectangle
 * @param width the width of the rectangle
 * @param height the height of the rectangle
 * @param color the color of the rectangle
 */
void drawRect(int row, int col, int width, int height, u16 color) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[3].cnt = (u32) (width | DMA_ON | DMA_SOURCE_FIXED);
    }
}

/**
 * Draw an image on screen using DMA
 *
 * @param row the row to draw the image
 * @param col the column to draw the image
 * @param width the width of the image
 * @param height the height of the image
 * @param image pointer to the image array
 */
void drawImage(int row, int col, int width, int height, const u16 *image) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &image[OFFSET(r, 0, width)];
        DMA[3].dst = &videoBuffer[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[3].cnt = (u32) (width | DMA_ON);
    }
}

/**
 * Draw a sub-rectangle of a background image that is the size of the screen. Used to erase the last position of an
 * on-screen object.
 *
 * @param row the row to draw the background rectangle
 * @param col the column to draw the background rectangle
 * @param width the width the of the background rectangle
 * @param height the height of the background rectangle
 * @param image pointer to the background image array
 */
void drawBkg(int row, int col, int width, int height, const u16 *image) {
    for (int r = 0; r < height; r++) {
        DMA[2].src = &image[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[2].dst = &videoBuffer[OFFSET(row + r, col, SCR_WIDTH)];
        DMA[2].cnt = (u32) (width | DMA_ON);
    }
}

/**
 * Set the pixel at a certain position to the given color
 *
 * @param row the on-screen row
 * @param col the on-screen column
 * @param color the color to set the pixel to
 */
void setPixel(int row, int col, u16 color) {
    videoBuffer[OFFSET(row, col, SCR_WIDTH)] = color;
}

/**
 * Draw a single character to a position on screen with the given color
 *
 * @param row the row to draw the character
 * @param col the column to draw the character
 * @param ch the character to draw
 * @param color the color to draw the character with
 */
void drawChar(int row, int col, char ch, u16 color) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 6; c++) {
            if (fontdata_6x8[OFFSET(r, c, 6) + ch * 48]) {
                setPixel(row + r, col + c, color);
            }
        }
    }
}

/**
 * Draw a string of characters to a position on screen with the given color
 *
 * @param row the row to draw the string
 * @param col the column to draw the string
 * @param str the string to draw
 * @param color the color to draw the string with
 */
void drawString(int row, int col, char str[], u16 color) {
    while (*str) {
        drawChar(row, col, *str++, color);
        col += 6;
    }
}

/**
 * Draw an integer to a position on screen with the given color
 *
 * @param row the row to draw the integer
 * @param col the column to draw the integer
 * @param n the integer to draw
 * @param color the color to draw the integer with
 */
void drawInt(int row, int col, int n, u16 color) {
    char buffer[41];
    sprintf(buffer, "%d", n);
    drawString(row, col, buffer, color);
}
