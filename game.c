#include "game.h"
#include "hardware.h"
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
int srcBoard[ COLS ][ ROWS ];

// The destination board state.
int dstBoard[ COLS ][ ROWS ];

int score;

// Sample locations until we successfully add a new tile or exceed spawn attempts.
void addNewTile( void )
{
    int attempts = 0;
    while (attempts < (COLS * ROWS))
    {
        int randCol = rand() % COLS;
        int randRow = rand() % ROWS;
        // 75% chance of a 2 Tile and 25% change of 4 Tile.
        int randVal = (rand() % 4) ? 2 : 4;
        if (!dstBoard[randCol][randRow])
        {
            dstBoard[randCol][randRow] = randVal;
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
    for (int c = 0; c < COLS; c++)
    {
        for (int r = 0; r < ROWS; r++)
        {
            dstBoard[ c ][ r ] = 0;
        }
    }

    switch (button)
    {
    case UP_BUTTON:
        for (int c = 0; c < COLS; c++)
        {
            for (int r = 0; r < ROWS; r++)
            {
                moveTile(c, r, -1, 0);
            }
        }
        break;

    case DOWN_BUTTON:
        for (int c = COLS-1; c >= 0; c-- )
        {
            for (int r = 0; r < ROWS; r++)
            {
                moveTile(c, r, 1, 0);
            }
        }
        break;

    case LEFT_BUTTON:
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                moveTile(c, r, 0, -1);
            }
        }
        break;

    case RIGHT_BUTTON:
        for (int r = ROWS-1; r >= 0; r--)
        {
            for (int c = 0; c < COLS; c++)
            {
                moveTile(c, r, 0, 1);
            }
        }
        break;

    default:
        break;
    }

    addNewTile();
}

// Calculate tile moves and mergers. Assume tiles in movement direction are fixed.
void moveTile(int srcCol, int srcRow, int dc, int dr)
{
    // No tile? No problem.
    if (!srcBoard[srcCol][srcRow])
    {
        return;
    }

    // We are moving across columns (left / right).
    if( dr )
    {
        // If the tile can't possibly move, copy and halt.
        switch( srcRow + dr )
        {
        case -1:
            dstBoard[ srcCol ][ srcRow ] = srcBoard[ srcCol ][ srcRow ];
            return;

        case ROWS:
            dstBoard[ srcCol ][ srcRow ] = srcBoard[ srcCol ][ srcRow ];
            return;

        default:
            break;
        }

        // Find the closest occupied tile in the movement direction.
        int blkRow = srcRow + dr;
        while (0 <= blkRow && blkRow < ROWS)
        {
            if (dstBoard[srcCol][blkRow])
            {
                break;
            }
            else
            {
                blkRow += dr;
            }
        }

        // It there is no occupied tile, move and halt.
        if (0 > blkRow || blkRow >= COLS)
        {
            dstBoard[srcCol][blkRow-dr] = srcBoard[srcCol][srcRow];
            return;
        }

        // If we can merge with the occupied tile, merge and halt, otherwise, move and halt.
        if (dstBoard[srcCol][blkRow] == srcBoard[srcCol][srcRow])
        {
            dstBoard[srcCol][blkRow] = 2 * srcBoard[srcCol][srcRow];
            // Add merged value to score.
            score += dstBoard[srcCol][blkRow];
            return;
        }
        else
        {
            dstBoard[srcCol][blkRow-dr] = srcBoard[srcCol][srcRow];
            return;
        }
    }
    // We are moving across rows (up / down).
    else if (dc)
    {
        // If the tile can't possibly move, copy and halt.
        switch (srcCol + dc)
        {
        case -1:
            dstBoard[srcCol][srcRow] = srcBoard[srcCol][srcRow];
            return;

        case COLS:
            dstBoard[srcCol][srcRow] = srcBoard[srcCol][srcRow];
            return;

        default:
            break;
        }

        // Find the closest occupied tile in the movement direction.
        int blkCol = srcCol + dc;
        while( 0 <= blkCol && blkCol < COLS )
        {
            if (dstBoard[blkCol][srcRow])
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
            dstBoard[ blkCol - dc ][ srcRow ] = srcBoard[ srcCol ][ srcRow ];
            return;
        }

        // If we can merge with the occupied tile, merge and halt, otherwise, move and halt.
        if( dstBoard[ blkCol ][ srcRow ] == srcBoard[ srcCol ][ srcRow ])
        {
            dstBoard[ blkCol ][ srcRow ] = 2 * srcBoard[ srcCol ][ srcRow ];
            // Add merged value to score.
            score += dstBoard[ blkCol ][ srcRow ];
        }
        else
        {
            dstBoard[ blkCol - dc ][ srcRow ] = srcBoard[ srcCol ][ srcRow ];
        }
    }
}

void drawGame( void )
{
    // Draw the tiles.
    for( int c = 0; c < COLS; c++ )
    {
        for( int r = 0; r < ROWS; r++ )
        {
            if( dstBoard[ c ][ r ] != srcBoard[ c ][ r ])
            {
                if( dstBoard[ c ][ r ])
                {
                    drawImage3( OFFSET_R + CELL_R * r, OFFSET_C + CELL_C * c, TILE_SIZE, TILE_SIZE, getImage( dstBoard[ c ][ r ]));
                }
                else
                {
                    drawRect( OFFSET_C + CELL_C * c, OFFSET_R + CELL_R * r, TILE_SIZE, TILE_SIZE, TEXT_COLOR );
                }
            }
        }
    }

    // Copy dstBoard to srcBoard.
    for (int c = 0; c < COLS; c++)
    {
        for (int r = 0; r < ROWS; r++)
        {
            srcBoard[c][r] = dstBoard[c][r];
        }
    }
}

void clearGame()
{
    score = 0;
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            srcBoard[i][j] = 0;
            dstBoard[i][j] = 0;
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

int gameOver(void)
{
    // If the 2048 tile has been created, the game is over.
    for (int c = 0; c < COLS; c++)
    {
        for (int r = 0; r < ROWS; r++)
        {
            if (srcBoard[c][r] == 2048)
            {
                return 1;
            }
        }
    }
    // If any values are 0, the game is not over.
    for (int c = 0; c < COLS; c++)
    {
        for (int r = 0; r < ROWS; r++)
        {
            if (!srcBoard[c][r])
            {
                return 0;
            }
        }
    }
    // If any neighbors are equal, the game is not over.
    for (int c = 0; c < COLS; c++)
    {
        for (int r = 0; r < ROWS; r++)
        {
            if (c > 0)
            {
                if (srcBoard[c][r] == srcBoard[c-1][r])
                {
                    return 0;
                }
            }
            if (c < COLS-1)
            {
                if (srcBoard[c][r] == srcBoard[c+1][r])
                {
                    return 0;
                }
            }
            if (r > 0)
            {
                if (srcBoard[c][r] == srcBoard[c][r-1])
                {
                    return 0;
                }
            }
            if (r < ROWS-1)
            {
                if (srcBoard[c][r] == srcBoard[c][r+1])
                {
                    return 0;
                }
            }
        }
    }
    // Otherwise, the game is over.
    return 1;
}
