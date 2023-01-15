#pragma once
#pragma once

#include "coordinate.h"
#include "consoleutil.h"

/**
* basic game entity
*/
class Entity
{
protected:
	/** where the entity is */
	Coordinate pos;
	/** what the entity looks like */
	char icon;
	/** */
	int howToMoveNext;
public:
	/** used to determine which move to make (based on what key was pressed) */
	static const int MOVE_NONE = -1, MOVE_UP = 0, MOVE_LEFT = 1, MOVE_DOWN = 2, MOVE_RIGHT = 3;

	/**
	* @param x where
	* @param y where
	* @param icon what it looks like
	*/
	Entity(int x, int y, char icon)
		: pos(x, y), icon(icon), howToMoveNext(MOVE_NONE) { }
	Entity() { }

	/** draws the entity at it's known location, dependend on moveCursor(,) and putchar() */
	void draw();

	short getX() { return pos.x; }
	short getY() { return pos.y; }
	void setX(int value) { pos.x = value; }
	void setY(int value) { pos.y = value; }
	void addX(int value) { pos.x += value; }
	void addY(int value) { pos.y += value; }

	/** returns a the Entity's position */
	Coordinate getPosition() { return pos; }

	/** re-initialize */
	void init(int x, int y, char icon);

	/**
	* @param MOVE_UP, MOVE_LEFT, MOVE_DOWN, or MOVE_RIGHT
	* @return true if the parameter was a valid move
	*/
	bool move(int direction);

	/** implement actual changes to the Entity's state, including movement */
	virtual void update();

	/** access what the Entity looks like */
	char getIcon() { return icon; }
};
