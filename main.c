// 2048 GBA game
// Author: Arda Pekis

#include "main.h"

// State enum definition
enum GBAState {
	DRAW_START,
	START,
	DRAW_GAME,
	GAME,
	DRAW_END,
	END
};

void waitForVBlank()
{
	while(REG_VCOUNT >= 160);
	while(REG_VCOUNT < 160);
}

int main() {

	REG_DISPCNT = MODE_3 | BG2_EN;

	enum GBAState state = START;
	int select_depressed = 0;
	int left_depressed = 0;
	int right_depressed = 0;
	int up_depressed = 0;
	int down_depressed = 0;

	while(1) {
		waitForVBlank();
		switch(state) {
		case DRAW_START:
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
