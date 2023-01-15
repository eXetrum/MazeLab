#include "entity.h"
#include <stdio.h>

/** draws the entity at it's known location, dependent on moveCursor(,) and putchar() */
void Entity::draw()
{
	moveCursor(pos.x, pos.y);
	putchar(icon);
}

/** re-initialize */
void Entity::init(int a_x, int a_y, char a_icon)
{
	pos.init(a_x, a_y);
	icon = a_icon;
}

/**
* @param MOVE_UP, MOVE_LEFT, MOVE_DOWN, or MOVE_RIGHT
* @return true if the parameter was a valid move
*/
bool Entity::move(int direction)
{
	bool validMove = false;
	switch (direction)
	{
	case MOVE_UP:
	case MOVE_LEFT:
	case MOVE_DOWN:
	case MOVE_RIGHT:
		this->howToMoveNext = direction;
		validMove = true;
	}
	return validMove;
}

/** implement actual changes to the Entity's state, including movement */
void Entity::update()
{
	switch (howToMoveNext)
	{
	case MOVE_UP:	pos.y -= 1;	break;
	case MOVE_LEFT:	pos.x -= 1; break;
	case MOVE_DOWN:	pos.y += 1; break;
	case MOVE_RIGHT:pos.x += 1; break;
	}
	howToMoveNext = MOVE_NONE;
}
