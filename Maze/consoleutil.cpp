#include <windows.h>	// SetConsoleCursorPosition(HANDLE,COORD)
#include "consoleutil.h"

/**
 * moves the console cursor to the given x/y coordinate
 * @param x
 * @param y
 */
void moveCursor(int x, int y)
{
	COORD c = {x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
