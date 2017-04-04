#define REG_DISPCNT *(unsigned short*) 0x04000000
#define MODE_3 3
#define BG2_EN (1 << 10)
#define REG_VCOUNT *(volatile unsigned short*) 0x04000006

enum GBAState {
	DRAW_START,
	START,
	DRAW_GAME,
	GAME,
	DRAW_END,
	END
};
