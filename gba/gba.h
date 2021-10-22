#ifndef GBA_H
#define GBA_H 1
#include <stdint.h>

#define REG_VCOUNT_ADDR 0x04000006
#define REG_VCOUNT      (* (volatile uint16_t*) REG_VCOUNT_ADDR)

#define MEM_IO          0x04000000
#define REG_DISPCNT     *((volatile uint32_t*)(MEM_IO))

#define REG_IME_ADDR    0x04000208
#define REG_IME         *((volatile uint16_t*) REG_IME_ADDR)

#define REG_IE_ADDR     0x04000200
#define REG_IE          *((volatile uint16_t*) REG_IE_ADDR)

#define MEM_VRAM        0x06000000
#define vid_mem         ((uint16_t*)MEM_VRAM)

#define REG_DISPSTAT_ADDR 0x04000004
#define REG_DISPSTAT    *((volatile uint16_t*) REG_DISPSTAT_ADDR)

#define DCNT_MODE0      0x0000
#define DCNT_MODE1      0x0001
#define DCNT_MODE2      0x0002
#define DCNT_MODE3      0x0003
#define DCNT_MODE4      0x0004
#define DCNT_MODE5      0x0005

#define DCNT_BG0        0x0100
#define DCNT_BG1        0x0200
#define DCNT_BG2        0x0400
#define DCNT_BG3        0x0800
#define DCNT_OBJ        0x1000

static inline COLOR RGB15(uint32_t red, uint32_t green, uint32_t blue)
{
    return red | (green << 5) | (blue << 10);
}

static inline void vsync(void)
{
    while( REG_VCOUNT >= 160 )
        ;
    while( REG_VCOUNT < 160 )
        ;
}

static inline HardwareInit(void)
{
    REG_DISPCNT = DCNT_MODE3 | DCNT_BG2; //mode 3 graphics, we aren't actually using bg2 right now
    REG_IME = 1; //enable interrupts
}

#endif
