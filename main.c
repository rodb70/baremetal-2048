// 2048 GBA game
// Author: Arda Pekis

#include "main.h"
#include "screens.h"

int main() {

	REG_DISPCNT = MODE_3 | BG2_EN;

	enum GBAState state = DRAW_START;	
	/*
	int select_depressed = 0;
	int left_depressed = 0;
	int right_depressed = 0;
	int up_depressed = 0;
	int down_depressed = 0;
	*/

	while(1) {
		waitForVBlank();
		switch(state) {
		case DRAW_START:
			drawStart();
			state = START;
			break;
		case START:
			break;
		case DRAW_GAME:
			break;
		case GAME:
			break;
		case DRAW_END:
			break;
		case END:
			break;
		}
	}

	return 0;
}

void waitForVBlank()
{
	while(REG_VCOUNT >= 160);
	while(REG_VCOUNT < 160);
}
