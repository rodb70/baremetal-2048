#define	ROWS	4
#define	COLS	4
#define OFFSET_R	10
#define OFFSET_C	10
#define CELL_R		36
#define CELL_C		36		
#define TRUE	1
#define FALSE	0
#define UP		0
#define DOWN	1
#define LEFT	2
#define RIGHT	3

#include "keys.h"

extern int srcBoard[ROWS][COLS];
extern int dstBoard[ROWS][COLS];
extern int score;

void addNewTile();
void play(Button button);
void moveTile(int srcRow, int srcCol, int dr, int dc);
const unsigned short *getImage(int tile);
void clearGame();
void drawGame();
int gameOver();