#ifndef HOST_HARDWARE_H_
#define HOST_HARDWARE_H_
#include <stdbool.h>
#include <stdint.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

#define TILE_SIZE   32
#define TEXT_COLOR  0x031af
#define BACKGROUND  0x077ff

#define TILE_SIZE   32
#define NUM_WIDTH   6
#define NUM_HEIGHT  9

extern uint16_t *vid_mem;
void key_init( void );

static inline void display_init( void )
{
    static bool inited = false;
    extern volatile uint16_t* frame_buffer_init(void);

    key_init();
    if( false == inited )
    {
        inited = true;
        (void)frame_buffer_init();
    }
}

void vsync(void);
uint16_t key_poll( void );

void drawImageRow(int rs, int cs, int ri, int wi, const unsigned short *image);
void drawImage3(int r, int c, int width, int height, const uint16_t* image);
void drawBgImage(const uint16_t* image);
void drawTile(int oldr, int oldc, int r, int c, const uint16_t* image);
void drawNumber(int rs, int cs, int number);
void drawRect(int r, int c, int width, int height, uint16_t color);
void setPixel(int r, int c, unsigned short color);
void waitForVBlank(void);
void waitForHBlank(void);
void fillBg(uint16_t color);

extern uint32_t hw_buttons;

#define BUTTONS       hw_buttons
#define KEY_DOWN_NOW(key) (~(BUTTONS) & (1 << key))
uint32_t poll_controller(uint32_t delay);

#endif /* HOST_HARDWARE_H_ */
