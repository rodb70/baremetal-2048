#define	ROWS	4
#define	COLS	4

#include "keys.h"

typedef struct {
	int r;
	int c;
	int dr;
	int dc;
	const unsigned short *image;
	int count;
	int value;
} Tile;

extern Tile board[ROWS][COLS];

void addNewTile();
void animate(Button button);
void clearGame();
void drawGame();
