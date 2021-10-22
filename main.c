// 2048 GBA game
// Author: Arda Pekis

#include "main.h"
#include "screens.h"
#include "game.h"
#include "hardware.h"
#include "Tile2.h"

int main(void)
{

    display_init();

    enum GBAState state = DRAW_START;

    while(1)
    {
        switch(state)
        {
        case DRAW_START:
            waitForVBlank();
            drawStart();
            clearGame();
            state = START;
            break;
        case START:
            updateAllKeys();
            if( buttonJustPressed( START_BUTTON ))
            {
                state = DRAW_GAME;
                addNewTile();
                waitForVBlank();
                fillBg( BACKGROUND );
                drawRect( 6, 6, 152, 152, TEXT_COLOR );
            }
            drawGame();
            break;
        case DRAW_GAME:
            waitForVBlank();
            drawGame();
            if (gameOver())
            {
                state = DRAW_END;
            }
            else
            {
                state = GAME;
            }
            break;
        case GAME:
            updateAllKeys();
            if (buttonJustPressed(SELECT_BUTTON))
            {
                state = DRAW_END;
                break;
            }
            else if (buttonJustPressed(START_BUTTON))
            {
                state = DRAW_START;
                break;
            }
            else if (buttonJustPressed(UP_BUTTON))
            {
                play(UP_BUTTON);
            }
            else if (buttonJustPressed(DOWN_BUTTON))
            {
                play(DOWN_BUTTON);
            }
            else if (buttonJustPressed(LEFT_BUTTON))
            {
                play(LEFT_BUTTON);
            }
            else if (buttonJustPressed(RIGHT_BUTTON))
            {
                play(RIGHT_BUTTON);
            }
            else
            {
                break;
            }
            state = DRAW_GAME;
            break;
        case DRAW_END:
            waitForVBlank();
            drawEnd();
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
        drawGame();
    }

    return 0;
}
