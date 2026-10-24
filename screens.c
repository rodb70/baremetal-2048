#include "hardware.h"
#include "screens.h"
#include "startScreen.h"
#include "endScreen.h"
#include "game.h"

void drawStart(void)
{
    drawBgImage( startScreen );
}

void drawEnd(void)
{
    drawBgImage( endScreen );
    drawNumber( 100, 140, score );
}
