cmake_minimum_required(VERSION 3.8)
project(hw09_3)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c main.h input.c input.h graphics.c graphics.h game.h levels.c levels.h font.c drawgame.c drawgame.h sprites.c sprites.h)
add_executable(hw09_3 ${SOURCE_FILES})