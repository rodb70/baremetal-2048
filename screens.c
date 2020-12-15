#include "draw.h"
#include "screens.h"
#include "startScreen.h"
#include "endScreen.h"
#include "game.h"

void drawStart()
{
	drawBgImage(startScreen);
}

void drawEnd()
{
	drawBgImage(endScreen);
	drawNumber(100, 140, score);
}
