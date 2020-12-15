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

// The current board state.
int srcBoard[ROWS][COLS];

// The destination board state.
int dstBoard[ROWS][COLS];

// Sample locations until we sucessfully add a new tile or exceed spawn attempts.
void addNewTile()
{
	int attempts = 0;
	while (attempts < (ROWS * COLS))
	{
		int randRow = rand() % ROWS;
		int randCol = rand() % COLS;
		// 75% chance of a 2 Tile and 25% change of 4 Tile.
		int randVal = (rand() % 4) ? 2 : 4;
		if (!dstBoard[randRow][randCol])
		{
			dstBoard[randRow][randCol] = randVal;
			return;
		}
		else
		{
			attempts += 1;
		}
	}
}

// Find the destination state given the current state and the player action.
void play(Button button)
{
	// Clear the destination board.
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			dstBoard[r][c] = 0;
		}
	}

	switch (button)
	{
	case UP_BUTTON:
		for (int r = 0; r < ROWS; r++)
		{
			for (int c = 0; c < COLS; c++)
			{
				moveTile(r, c, -1, 0);
			}
		}
		break;

	case DOWN_BUTTON:
		for (int r = ROWS-1; r >= 0; r--)
		{
			for (int c = 0; c < COLS; c++)
			{
				moveTile(r, c, 1, 0);
			}
		}
		break;

	case LEFT_BUTTON:
		for (int c = 0; c < COLS; c++)
		{
			for (int r = 0; r < ROWS; r++)
			{
				moveTile(r, c, 0, -1);
			}
		}
		break;

	case RIGHT_BUTTON:
		for (int c = COLS-1; c >= 0; c--)
		{
			for (int r = 0; r < ROWS; r++)
			{
				moveTile(r, c, 0, 1);
			}
		}
		break;
	
	default:
		break;
	}
	addNewTile();
}

// Calculate tile moves and mergers. Assume tiles in movement direction are fixed.
void moveTile(int srcRow, int srcCol, int dr, int dc)
{
	// No tile? No problem.
	if (!srcBoard[srcRow][srcCol])
	{
		return;
	}
	// We are moving across columns (left / right).
	if (dc)
	{
		// If the tile can't possibly move, copy and halt.
		switch (srcCol + dc)
		{
		case -1:
			dstBoard[srcRow][srcCol] = srcBoard[srcRow][srcCol];
			return;

		case COLS:
			dstBoard[srcRow][srcCol] = srcBoard[srcRow][srcCol];
			return;
		
		default:
			break;
		}

		// Find the closest occupied tile in the movement direction.
		int blkCol = srcCol + dc;
		while (0 <= blkCol && blkCol < COLS)
		{
			if (dstBoard[srcRow][blkCol])
			{
				break;
			}
			else
			{
				blkCol += dc;
			}
		}

		// It there is no occupied tile, move and halt.
		if (0 > blkCol || blkCol >= COLS)
		{
			dstBoard[srcRow][blkCol-dc] = srcBoard[srcRow][srcCol];
			return;
		}

		// If we can merge with the occupied tile, merge and halt, otherwise, move and halt.
		if (dstBoard[srcRow][blkCol] == srcBoard[srcRow][srcCol])
		{
			dstBoard[srcRow][blkCol] = 2 * srcBoard[srcRow][srcCol];
			return;
		}
		else
		{
			dstBoard[srcRow][blkCol-dc] = srcBoard[srcRow][srcCol];
			return;
		}
	}
	// We are moving across rows (up / down).
	else if (dr)
	{
		// If the tile can't possibly move, copy and halt.
		switch (srcRow + dr)
		{
		case -1:
			dstBoard[srcRow][srcCol] = srcBoard[srcRow][srcCol];
			return;

		case COLS:
			dstBoard[srcRow][srcCol] = srcBoard[srcRow][srcCol];
			return;
		
		default:
			break;
		}

		// Find the closest occupied tile in the movement direction.
		int blkRow = srcRow + dr;
		while (0 <= blkRow && blkRow < ROWS)
		{
			if (dstBoard[blkRow][srcCol])
			{
				break;
			}
			else
			{
				blkRow += dr;
			}
		}

		// It there is no occupied tile, move and halt.
		if (0 > blkRow || blkRow >= ROWS)
		{
			dstBoard[blkRow-dr][srcCol] = srcBoard[srcRow][srcCol];
			return;
		}

		// If we can merge with the occupied tile, merge and halt, otherwise, move and halt.
		if (dstBoard[blkRow][srcCol] == srcBoard[srcRow][srcCol])
		{
			dstBoard[blkRow][srcCol] = 2 * srcBoard[srcRow][srcCol];
			return;
		}
		else
		{
			dstBoard[blkRow-dr][srcCol] = srcBoard[srcRow][srcCol];
			return;
		}
	}
}

void drawGame()
{
	// Draw the Background.
	//fillBg(BACKGROUND);
	drawRect(6, 6, 152, 152, TEXT_COLOR);
	// Draw the tiles.
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			if (dstBoard[r][c])
			{
				drawImage3(OFFSET_R + CELL_R * r, OFFSET_C + CELL_C * c, TILE_SIZE, TILE_SIZE, getImage(dstBoard[r][c]));
			}
		}
	}
	// Copy dstBoard to srcBoard.
	for (int r = 0; r < ROWS; r++)
	{
		for (int c = 0; c < COLS; c++)
		{
			srcBoard[r][c] = dstBoard[r][c];
		}
	}
}

void clearGame()
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			srcBoard[i][j] = 0;
		}
	}
}

const unsigned short *getImage(int tile)
{
	switch (tile)
	{
	case 2:
		return Tile2;
	
	case 4:
		return Tile4;

	case 8:
		return Tile8;

	case 16:
		return Tile16;

	case 32:
		return Tile32;

	case 64:
		return Tile64;

	case 128:
		return Tile128;

	case 256:
		return Tile256;

	case 512:
		return Tile512;

	case 1024:
		return Tile1024;

	case 2048:
		return Tile2048;
	
	// This should never happen.
	default:
		return Tile2;
	}
}