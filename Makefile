# Makefile

CROSS_COMPILE := /opt/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-

COMPILER := gcc

#CPU := rpi1
CPU := host
BLD_TARGET := 2048
BLD_TYPE := debug

ifeq ($(CPU),rpi1)
PROJ_DIRS := rpi1
endif
ifeq ($(CPU),host)
PROJ_DIRS := host
EXTRA_LIBS := -lSDL2
SHORT_ENUMS := n
endif

INC := .

CSRC += Digits.c
CSRC += endScreen.c
CSRC += game.c
CSRC += keys.c
CSRC += main.c
CSRC += screens.c
CSRC += startScreen.c
CSRC += Tile1024.c
CSRC += Tile128.c
CSRC += Tile16.c
CSRC += Tile2048.c
CSRC += Tile256.c
CSRC += Tile2.c
CSRC += Tile32.c
CSRC += Tile4.c
CSRC += Tile512.c
CSRC += Tile64.c
CSRC += Tile8.c

include makefiles/main.mk

distclean:
	rm -rf build
