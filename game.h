#define	ROWS	4
#define	COLS	4

#include "keys.h"

typedef struct {
	int r;
	int c;
	int dr;
	int dc;
	unsigned short color;
} MOVING_TILE;

extern char board[ROWS][COLS];

void startGame();
void animate(Button button);
