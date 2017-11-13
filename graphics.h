//
// Created by jake on 11/9/17.
//

#ifndef HW09_3_GRAPHICS_H
#define HW09_3_GRAPHICS_H

#include "main.h"

// VIDEO
extern u16 *videoBuffer;
#define REG_DISPCNT *(unsigned short*) 0x4000000
#define SCANLINECOUNTER *(u16 *) 0x4000006
#define MODE_3 3
#define BG2_ENABLE (1 << 10)
#define OFFSET(r, c, rowlen) ((r) * (rowlen) + (c))
#define SCR_WIDTH 240
#define SCR_HEIGHT 160

// COLOR
#define COLOR(r, g, b) ((b) << 10 | (g) << 5 | (r))
#define BLACK   COLOR(0,0,0)
#define WHITE   COLOR(31, 31, 31)
#define RED     COLOR(31,0,0)
#define GREEN   COLOR(0, 31, 0)
#define BLUE    COLOR(0,0,31)
#define YELLOW  COLOR(31,31,0)
#define CYAN    COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define GRAY    COLOR(25, 25, 25)

// DMA
typedef struct dma {
    const volatile void *src;
    const volatile void *dst;
    u32 cnt;
} DMA_CONTROLLER;
#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)
#define DMA_ON (1 << 31)

// FONT
#define FONT_WIDTH 6
#define FONT_HEIGHT 8
extern const u8 fontdata_6x8[12288];

// PROTOTYPES
void initGraphics();

void waitForVblank();

void drawRect(int row, int col, int width, int height, u16 color);

void drawImage(int row, int col, int width, int height, const u16 *image);

void drawBkg(int row, int col, int width, int height, const u16 *image);

void setPixel(int row, int col, u16 color);

void drawChar(int row, int col, char ch, u16 color);

void drawString(int row, int col, char str[], u16 color);

void drawInt(int row, int col, int n, u16 color);

#endif //HW09_3_GRAPHICS_H
