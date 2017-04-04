// 2048 GBA game
// Author: Arda Pekis

#include "main.h"
#include "screens.h"
#include "game.h"
#include "draw.h"

int main() {

	REG_DISPCNT = MODE_3 | BG2_EN;

	enum GBAState state = DRAW_START;
	int score = 0;	

	while(1) {
		switch(state) {
		case DRAW_START:
			drawStart();
			state = START;
			score = 0;
			break;
		case START:
			updateAllKeys();
			if (buttonJustPressed(START_BUTTON))
			{
				state = DRAW_GAME;
				//startGame();
			}
			break;
		case DRAW_GAME:
			// TODO: Draw game screen.
			animate(UP_BUTTON);
			state = GAME;
			break;
		case GAME:
			// TODO: Check for game over.
			updateAllKeys();
			if (buttonJustPressed(SELECT_BUTTON))
			{
				state = DRAW_START;
				break;
			}
			else if (buttonJustPressed(UP_BUTTON))
			{

			}
			else if (buttonJustPressed(DOWN_BUTTON))
			{

			}
			else if (buttonJustPressed(LEFT_BUTTON))
			{

			}
			else if (buttonJustPressed(RIGHT_BUTTON))
			{

			}
			else break;
			state = DRAW_GAME;
			break;
		case DRAW_END:
			drawEnd(score);
			state = END;
			break;
		case END:
			updateAllKeys();
			if (buttonJustPressed(SELECT_BUTTON))
			{

				state = DRAW_START;
			}
			break;
		}
	}

	return 0;
}
