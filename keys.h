
typedef enum
{
	A_BUTTON = 0,
	B_BUTTON,
	SELECT_BUTTON,
	START_BUTTON,
	RIGHT_BUTTON,
	LEFT_BUTTON,
	UP_BUTTON,
	DOWN_BUTTON,
	R_BUTTON,
	L_BUTTON
} Button;

int buttonJustPressed(Button button);
void updateAllKeys(void);
