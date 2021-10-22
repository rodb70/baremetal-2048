#include "draw.h"
#include "game.h"
#include "Digits.h"
#include <stdint.h>

uint16_t *videoBuffer = (uint16_t*) 0x06000000;

/**
 * Draws one row of an image onto the screen using DMA.
 *
 * @param[in]	rs	The row of the screen to draw on.
 * @param[in]	cs	The column of the screen to start drawing.
 * @param[in]	ri	The row of the image to draw.
 * @param[in]	wi	The width of the image being drawn.
 * @param[in]	image	The image to draw a row of.
 */
void drawImageRow(int rs, int cs, int ri, int wi, const uint16_t* image)
{
	if (rs >= SCREEN_HEIGHT) return;
	if (cs >= SCREEN_WIDTH) return;
	DMA[3].src = image + (ri * wi);
	DMA[3].dst = videoBuffer + (SCREEN_WIDTH * rs) + cs;
	int count = (cs + wi >= SCREEN_WIDTH) ? SCREEN_WIDTH - cs : wi;
	DMA[3].cnt = DMA_SOURCE_INCREMENT | DMA_ON | DMA_DESTINATION_INCREMENT | count;
}

/**
 * Draws an image onto the screen at provided coordinates using DMA.
 *
 * @param[in]	r	The row of the screen for the top-left corner.
 * @param[in]	c	The column of the screen for the top-left corner.
 * @param[in]	width	The width of the image being drawn.
 * @param[in]	height	The height of the image being drawn.
 * @param[in]	image	The image to draw on the screen.
 */
void drawImage3(int r, int c, int width, int height, const uint16_t* image)
{
	for (int i = 0; i < height; i++)
	{
		drawImageRow(r + i, c, i, width, image);
	}
}

/**
 * Draw a selection of an image onto the screen at provided coordinates using DMA.
 *
 * @param[in]	rs	The row of the screen for the top-left corner.
 * @param[in]	cs	The column of the screen for the top-left corner.
 * @param[in]	width	The width of the image being drawn.
 * @param[in]	height	The height of the image being drawn.
 * @param[in]	ri		The row of the image for the top-left corner.
 * @param[in]	ci		The column of the image for the top-left corner.
 * @param[in]	rows	Number of rows to draw.
 * @param[in]	cols	Number of columns in draw.
 * @param[in]	image	The image to draw on the screen.
 */
void drawImageSection(int rs, int cs, int width, int height, int ri, int ci, int rows, int cols, const uint16_t* image)
{
	if (rs >= SCREEN_HEIGHT) return;
	if (cs >= SCREEN_WIDTH) return;
	if (ri + rows > height) return;
	if (ci + cols > width) return;
	for (int i = 0; i < rows; i++)
	{
	    const uint16_t* row = image + (width * (ri+i)) + ci;
	    const uint16_t* vid = videoBuffer + (SCREEN_WIDTH * (rs+i)) + cs;
	    for( int z = 0; z < cols; z++ )
	    {
	        *vid = *row;
	        vid++;
	        row++;
	    }
		//DMA[3].dst = videoBuffer + (SCREEN_WIDTH * (rs+i)) + cs;
		//DMA[3].cnt = DMA_SOURCE_INCREMENT | DMA_ON | DMA_DESTINATION_INCREMENT | cols;
	}
}


void drawNumber(int rs, int cs, int number)
{
	static int subtractors[] = {100000, 10000, 1000, 100, 10, 1};
	for (int i = 0; i < 6; i++)
	{
		int count = 0;
		while (number >= subtractors[i])
		{
			number -= subtractors[i];
			count += 1;
		}
		drawImageSection(
			rs,
			cs + (NUM_WIDTH + 2) * i,
			DIGITS_WIDTH,
			DIGITS_HEIGHT,
			0,
			count * NUM_WIDTH,
			NUM_HEIGHT,
			NUM_WIDTH,
			Digits
		);
	}
}

/**
 * Draws an image over the entire screen using DMA. Use this preferentially
 * over drawImage when applicable.
 *
 * @param[in]	image	A 240 x 160 pixel image to replace the screen with.
 */
void drawBgImage(const uint16_t* image)
{
	DMA[3].src = image;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_SOURCE_INCREMENT | DMA_ON | DMA_DESTINATION_INCREMENT | (SCREEN_WIDTH * SCREEN_HEIGHT);
}

void fillBg(uint16_t color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_SOURCE_FIXED | DMA_ON | DMA_DESTINATION_INCREMENT | (SCREEN_WIDTH * SCREEN_HEIGHT);
}

/**
 * Draws a moving tile.
 * @param[in]	oldr	The old row coordinate of the tile.
 * @param[in]	oldc	The old column coordinate of the tile.
 * @param[in]	r	The new row coordinate of the tile.
 * @param[in]	c	The new column coordinate of the tile.
 * @param[in]	color	The color of the tile.
 */
void drawTile(int oldr, int oldc, int r, int c, const uint16_t* image)
{
	uint16_t textColor = TEXT_COLOR;
	int deleteR = (r >= oldr) ? oldr : r + TILE_SIZE;
	int deleteC = (c >= oldc) ? oldc : c + TILE_SIZE;
	int deleteWidth = (c != oldc) ? (c > oldc) ? c - oldc : oldc - c : TILE_SIZE;
	int deleteHeight = (r != oldr) ? (r > oldr) ? r - oldr : oldr - r : TILE_SIZE;
	drawRect(deleteR, deleteC, deleteWidth, deleteHeight, textColor);
	drawImage3(r, c, TILE_SIZE, TILE_SIZE, image);
}

/**
 * Draws a rectangle.
 *
 * @param[in]	r	The row coordinate of the rectangle.
 * @param[in]	c	The column coordinate of the rectangle.
 * @param[in]	width	The width of the rectangle.
 * @param[in]	height	The height of the rectangle.
 * @param[in]	color	The color of the rectangle.
 */
void drawRect(int r, int c, int width, int height, uint16_t color)
{
	DMA[3].src = &color;
	for (int i = 0; i < height; i++)
	{
		DMA[3].dst = videoBuffer + ((r+i) * SCREEN_WIDTH) + c;
		DMA[3].cnt = DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON | width;
	}
}

/**
 * Draws a pixel onto the screen. Use drawImage when drawing large
 * blocks of pixels.
 *
 * @param[in]	r	Row coordinate of pixel.
 * @param[in]	c	Column coordinate of pixel.
 * @param[in]	color	Color of pixel.
 */
void setPixel(int r, int c, uint16_t color)
{
	videoBuffer[SCREEN_WIDTH * r + c] = color;
}

/**
 * Waits for VBlank for large draw operations.
 */
void waitForVBlank()
{
	while(GET_BLANK(VBLANK));
	while(!GET_BLANK(VBLANK));
}

/**
 * Waits for HBlank for single row draw operations.
 */
void waitForHBlank()
{
	while(GET_BLANK(HBLANK));
	while(!GET_BLANK(HBLANK));
}
