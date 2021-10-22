#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "game.h"
#include <string.h>
#include <assert.h>
#include "input.h"
#include "hardware.h"
#include "Digits.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint16_t tft_fb[ SCREEN_HEIGHT ][ SCREEN_WIDTH ];

} monitor_t;

//#define BUT_INIT (~(0))
#define BUT_INIT (0)

uint16_t *vid_mem;
uint32_t hw_buttons = BUT_INIT;
uint16_t key_cur;
uint16_t key_prev;


static monitor_t monitor = { 0 };
monitor_t *m = &monitor;

int quit_filter(void *userdata, SDL_Event *event)
{
    (void) userdata;

    if( SDL_WINDOWEVENT == event->type )
    {
        if( SDL_WINDOWEVENT_CLOSE == event->window.event )
        {
            exit( 0 );
        }
    }
    else if( SDL_QUIT == event->type )
    {
        exit( 0 );
    }

    return 1;
}

volatile uint16_t* frame_buffer_init(void)
{
    hw_buttons = ~( 0 );
    /* Initialise the SDL */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialise! SDL_Error: %s\n", SDL_GetError() );
        exit( -1 );
    }

    SDL_SetEventFilter( quit_filter, NULL );

    m->window = SDL_CreateWindow( "2048", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  SCREEN_WIDTH*3, SCREEN_HEIGHT*3, 0 );
    assert( m->window );

    m->renderer = SDL_CreateRenderer( m->window, -1, SDL_RENDERER_SOFTWARE );
    assert( m->renderer );

    m->texture = SDL_CreateTexture( m->renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STATIC,
                                    SCREEN_WIDTH, SCREEN_HEIGHT );
    assert( m->texture );

    SDL_SetTextureBlendMode( m->texture, SDL_BLENDMODE_BLEND );

    vid_mem = &m->tft_fb[0][0];

    return (void*) m->tft_fb;
}

int frame_buffer_switch(int offset)
{
    (void) offset;

    int rslt = SDL_UpdateTexture( m->texture, NULL, m->tft_fb, SCREEN_WIDTH * sizeof( uint16_t ));
    assert( 0 == rslt );
    rslt = SDL_RenderClear( m->renderer );
    assert( 0 == rslt );

    /* Update the renderer with the texture containing the rendered image */
    rslt = SDL_RenderCopy( m->renderer, m->texture, NULL, NULL );
    assert( 0 == rslt );

    SDL_RenderPresent( m->renderer );

    return 0;
}

void drawRect(int col, int row, int height, int width, unsigned short color)
{
    for( int h = 0; h < height; h++ )
    {
        for( int w = 0; w < width; w++ )
        {
            m->tft_fb[ col + h ][ row + w ] = color;
        }
    }
}

void background(int height, int width, unsigned short color)
{
    for( int h = 0; h < height; h++ )
    {
        for( int w = 0; w < width; w++ )
        {
            m->tft_fb[ h ][ w ] = color;
        }
    }
}

void fillBg(uint16_t color)
{
    background( 0, 0, color );
}

void drawBgImage(const unsigned short *image)
{
    memcpy( m->tft_fb, image, sizeof( m->tft_fb ));
}

void drawImage3(int row, int col, int height, int width, const unsigned short *image)
{
    for( int c = 0; c < height; c++ )
    {
        for( int r = 0; r < width; r++ )
        {
            m->tft_fb[ col + c ][ row + r ] = *image;
            image++;
        }
    }
}

void drawHorizontal(int row, int col, int width, unsigned int color)
{
    for( int r = 0; r < width; r++ )
    {
        m->tft_fb[ col ][ row + r ] = color;
    }
}

void drawVertical(int row, int col, int height, int width, unsigned int color)
{
    for( int c = 0; c < height; c++ )
    {
        for( int r = 0; r < width; r++ )
        {
            m->tft_fb[ col + c ][ row + r ] = color;
        }
    }
}

/**
 * Draw a selection of an image onto the screen at provided coordinates using DMA.
 *
 * @param[in]   rs  The row of the screen for the top-left corner.
 * @param[in]   cs  The column of the screen for the top-left corner.
 * @param[in]   width   The width of the image being drawn.
 * @param[in]   height  The height of the image being drawn.
 * @param[in]   ri      The row of the image for the top-left corner.
 * @param[in]   ci      The column of the image for the top-left corner.
 * @param[in]   rows    Number of rows to draw.
 * @param[in]   cols    Number of columns in draw.
 * @param[in]   image   The image to draw on the screen.
 */
void drawImageSection(int cs, int rs, int height, int width, int ri, int ci, int rows, int cols, const uint16_t* image)
{
    if (cs >= SCREEN_HEIGHT) return;
    if (rs >= SCREEN_WIDTH) return;
    if (ri + rows > width) return;
    if (ci + cols > height) return;

    for (int i = 0; i < rows; i++)
    {
        const uint16_t* row = image + (height * (ri+i)) + ci;
        for( int z = 0; z < cols; z++ )
        {
            m->tft_fb[ cs + i ][ rs + z ] =  *(row++);
        }
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

uint32_t poll_controller(uint32_t delay)
{
    SDL_Event event;
    uint32_t mask = 0;
    int keyPressed = 0;

    while( SDL_PollEvent( &event ))
    {
        switch( event.key.keysym.sym )
        {
        case SDLK_a :
            mask |= KEY_A;
            break;

        case SDLK_b :
            mask |= KEY_B;
            break;

        case SDLK_q :
            mask |= KEY_L;
            break;

        case SDLK_w :
            mask |= KEY_R;
            break;

        case SDLK_SPACE :
            mask |= KEY_SELECT;
            break;

        case SDLK_RETURN2 :
        case SDLK_RETURN :
            mask |= KEY_START;
            break;

        case SDLK_KP_0 :
        case SDLK_0 :
            break;

        case SDLK_RIGHT:
        case SDLK_KP_PLUS:
            mask |= KEY_RIGHT;
            break;

        case SDLK_LEFT:
        case SDLK_KP_MINUS:
            mask |= KEY_LEFT;
            break;

        case SDLK_UP:
            mask |= KEY_UP;
            break;

        case SDLK_DOWN:
            mask |= KEY_DOWN;
            break;

        case SDLK_ESCAPE:
            exit( 1 );
            break;
        }

        switch( event.type )
        {
        case SDL_KEYDOWN:
            keyPressed = 1;
            break;

        case SDL_KEYUP:
            keyPressed = 0;
            break;

        default:
            break;

        }
    }

    if( keyPressed )
    {
        hw_buttons |= mask;
    }
    else
    {
        hw_buttons &= ~( mask );
    }

    SDL_Delay( delay ); /* Sleep for 5 millisecond */

    return hw_buttons;
}

uint16_t key_poll( void )
{
    key_prev = key_cur;
    key_cur = hw_buttons;
    return key_cur;
}

void key_init( void )
{
    hw_buttons = BUT_INIT;
}

void waitForVBlank(void)
{
    poll_controller(16);
    frame_buffer_switch(0);
}
