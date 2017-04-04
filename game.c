#include "game.h"
#include "draw.h"
#include "screens.h"

#include <stdlib.h>

char board[ROWS][COLS];

void startGame()
{
	int randRow = rand() % ROWS;
	int randCol = rand() % COLS;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (i == randRow && j == randCol)
			{
				board[i][j] = 1;
			}
			else
			{
				board[i][j] = 0;
			}
		}
	}
}

void animate(Button button)
{
	if (button == UP_BUTTON)
	{
		int r = 0;
		int c = 0;
		int dr = 4;
		int dc = 0;
		for (int i = 0; i < 20; i++)
		{
			waitForVBlank();
			drawTile(r, c, (r+dr) % 160, (c+dc) % 240, 0x0);
			r = (r+dr) % 160;
			c = (c+dc) % 240;
		}
	}
}
