/*
 * hardware.h
 *
 */

#ifndef GBA_HARDWARE_H_
#define GBA_HARDWARE_H_
#include "gba.h"


static inline void display_init(void)
{
    REG_DISPCNT = MODE_3 | BG2_EN;
}


#define BUTTONS       *(volatile unsigned short*) 0x4000130
#define KEY_DOWN_NOW(key) (~(BUTTONS) & (1 << key))

#endif /* GBA_HARDWARE_H_ */
