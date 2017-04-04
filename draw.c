#include "draw.h"

u16 *videoBuffer = (u16*) 0x06000000;

/**
 * Draws one row of an image onto the screen using DMA.
 *
 * @param[in]	rs	The row of the screen to draw on.
 * @param[in]	cs	The column of the screen to start drawing.
 * @param[in]	ri	The row of the image to draw.
 * @param[in]	wi	The width of the image being drawn.
 * @param[in]	image	The image to draw a row of.
 */
void drawrow(int rs, int cs, int ri, int wi, const unsigned short *image)
{
	if (rs >= SCREEN_HEIGHT) return;
	if (cs >= SCREEN_WIDTH) return;
	DMA[3].src = image + (SCREEN_WIDTH * ri * wi);
	DMA[3].dst = videoBuffer + (SCREEN_WIDTH * rs) + cs;
	int count = (cs + wi >= SCREEN_WIDTH) ? SCREEN_WIDTH - cs : cs + wi;
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
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for (int i = 0; i < height; i++)
	{
		drawrow(r + i, c, i, width, image);
	}
}

/**
 * Draws an image over the entire screen using DMA. Use this preferentially
 * over drawImage when applicable.
 *
 * @param[in]	image	A 240 x 160 pixel image to replace the screen with.
 */
void drawBgImage(const u16* image)
{
	DMA[3].src = image;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = DMA_SOURCE_INCREMENT | DMA_ON | DMA_DESTINATION_INCREMENT | (SCREEN_WIDTH * SCREEN_HEIGHT);
}

/**
 * Draws a pixel of onto the screen. Use drawImage when drawing large
 * blocks of pixels.
 *
 * @param[in]	r	Row coordinate of pixel.
 * @param[in]	c	Column coordinate of pixel.
 * @param[in]	color	Color of pixel.
 */
void setPixel(int r, int c, unsigned short color)
{
	videoBuffer[SCREEN_WIDTH * r + c] = color;
}
