#include "draw.h"
#include "screens.h"
#include "startScreen.h"
#include "endScreen.h"

void drawStart()
{
	drawBgImage(startScreen);
}

void drawEnd(int score)
{
	drawBgImage(endScreen);
	int i = score;
	score = i;
}
