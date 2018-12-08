#include "game.h"
#include "draw.h"
#include "screens.h"
#include "Tile2.h"
#include "Tile4.h"
#include "Tile8.h"
#include "Tile16.h"
#include "Tile32.h"
#include "Tile64.h"
#include "Tile128.h"
#include "Tile256.h"
#include "Tile512.h"
#include "Tile1024.h"
#include "Tile2048.h"

#include <stdlib.h>

Tile board[ROWS][COLS];

void addNewTile()
{
	int randRow = rand() % ROWS;
	int randCol = rand() % COLS;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (i == randRow && j == randCol)
			{
				Tile newTile;
				newTile.r = i * 36 + 10;
				newTile.c = j * 36 + 10;
				newTile.dr = 0;
				newTile.dc = 0;
				newTile.count = 0;
				newTile.value = 2;
				newTile.image = Tile2;
				if (board[i][j].r == 0)
				{
					board[i][j] = newTile;
				}
				return;
			}
		}
	}
}

void animate(Button button)
{
	if (button == UP_BUTTON)
	{
		for (int i = 1; i < ROWS; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				if (board[i][j].r != 0 && ((board[i-1][j].r == 0) || (board[i][j].value == board[i-1][j].value)))
				{
					board[i][j].dr = -4;
					board[i][j].count = 9;
				}
			}
		}
	} else if (button == DOWN_BUTTON)
	{
		for (int i = ROWS - 2; i >= 0; i--)
		{
			for (int j = 0; j < COLS; j++)
			{
				if (board[i][j].r != 0 && ((board[i+1][j].r == 0) || (board[i][j].value == board[i+1][j].value)))
				{
					board[i][j].dr = 4;
					board[i][j].count = 9;
				}
			}
		}
	} else if (button == LEFT_BUTTON)
	{
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 1; j < COLS; j++)
			{
				if (board[i][j].r != 0 && ((board[i][j-1].r == 0) || (board[i][j].value == board[i][j-1].value)))
				{
					board[i][j].dc = -4;
					board[i][j].count = 9;
				}
			}
		}
	} else if (button == RIGHT_BUTTON)
	{
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = COLS - 2; j >= 0; j--)
			{
				if (board[i][j].r != 0 && ((board[i][j+1].r == 0) || (board[i][j].value == board[i][j+1].value)))
				{
					board[i][j].dc = 4;
					board[i][j].count = 9;
				}
			}
		}
	}
	addNewTile();
}

void drawGame()
{
	int hasNonZero = 1;
	
	while(hasNonZero)
	{
	waitForVBlank();
	hasNonZero = 0;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j].count == 0) 
			{
				board[i][j].dr = 0;
				board[i][j].dc = 0;
				continue;
			}
			else
			{
				hasNonZero = 1;
				drawTile(board[i][j].r, board[i][j].c, board[i][j].r + board[i][j].dr, board[i][j].c + board[i][j].dc, board[i][j].image);
				board[i][j].r = (board[i][j].r + board[i][j].dr) % 160;
				board[i][j].c = (board[i][j].c + board[i][j].dc) % 240;
				board[i][j].count = board[i][j].count - 1;
			}
		}
	}
	}
	
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j].r == 0) continue;
			for (int k = i; k < ROWS; k++)
			{
				for (int l = j; l < COLS; l++)
				{
					if (board[k][l].r == 0) continue;
					if ((i != k) && (j != l) && (board[i][j].r == board[k][l].r) && (board[i][j].c == board[k][l].c))
					{
						int newLocR = ((board[i][j].r - 10) / 36) % ROWS;
						int newLocC = ((board[i][j].c - 10) / 36) % COLS;
						board[newLocR][newLocC] = board[i][j];
						board[i][j].r = 0;
						board[k][l].r = 0;
						board[newLocR][newLocC].r = newLocR * 36 + 10;
						board[newLocR][newLocC].c = newLocC * 36 + 10;
						board[newLocR][newLocC].value *= 2;
						if (board[newLocR][newLocC].value == 4)
						{
							board[newLocR][newLocC].image = Tile4;
						} else if (board[newLocR][newLocC].value == 8)
						{
							board[newLocR][newLocC].image = Tile8;
						} else if (board[newLocR][newLocC].value == 16)
						{
							board[newLocR][newLocC].image = Tile16;
						} else if (board[newLocR][newLocC].value == 32)
						{
							board[newLocR][newLocC].image = Tile32;
						} else if (board[newLocR][newLocC].value == 64)
						{
							board[newLocR][newLocC].image = Tile64;
						} else if (board[newLocR][newLocC].value == 128)
						{
							board[newLocR][newLocC].image = Tile128;
						} else if (board[newLocR][newLocC].value == 256)
						{
							board[newLocR][newLocC].image = Tile256;
						} else if (board[newLocR][newLocC].value == 512)
						{
							board[newLocR][newLocC].image = Tile512;
						} else if (board[newLocR][newLocC].value == 1024)
						{
							board[newLocR][newLocC].image = Tile1024;
						} else {
							board[newLocR][newLocC].image = Tile2048;
						}
					}
				}
			}
		}
	}
	/*
	Tile newBoard[ROWS][COLS];
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j].r == 0) {
				continue;
			}
			int newLocR = ((board[i][j].r - 10) / 36) % ROWS;
			int newLocC = ((board[i][j].r - 10) / 36) % COLS;
			newBoard[newLocR][newLocC] = board[i][j];
		}
	}
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			board[i][j] = newBoard[i][j];
		}
	}
	*/
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (board[i][j].r != 0)
			{
				drawImage3(board[i][j].r, board[i][j].c, TILE_SIZE, TILE_SIZE, board[i][j].image);
			}
		}
	}
}

void clearGame()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			board[i][j].r = 0;
		}
	}
}
