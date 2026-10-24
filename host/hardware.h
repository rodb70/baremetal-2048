/*
 * hardware.h
 *
 *  Created on: 16 Jul 2024
 *      Author: rod
 */

#ifndef HOST_HARDWARE_H_
#define HOST_HARDWARE_H_
#include <stdint.h>
#include <stdbool.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define TILE_SIZE   32
#define TEXT_COLOR  0x031af
#define NUM_WIDTH   6
#define NUM_HEIGHT  9

#define BACKGROUND  0x077ff

void drawNumber(int rs, int cs, int number);

void drawBgImage(const unsigned short *image);
volatile uint16_t* frame_buffer_init(void);
void waitForVBlank(void);
void fillBg(uint16_t color);

static inline void display_init(void)
{
    static bool inited = false;

    if( false == inited )
    {
        (void) frame_buffer_init();
        inited = true;
    }
}

uint32_t button_get(void);

#define BUTTONS button_get()

void drawImage3(int row, int col, int height, int width, const unsigned short *image);
void drawRect(int col, int row, int height, int width, unsigned short color);

uint32_t poll_controller(uint32_t delay);
#define KEY_DOWN_NOW(key) (~(BUTTONS) & (1 << key))

#endif /* HOST_HARDWARE_H_ */
