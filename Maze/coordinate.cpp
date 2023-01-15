#include "coordinate.h"
#include <cmath>

/** @return true if the given x, y coordinate mathes this Coordinate's data */
bool Coordinate::is(int x, int y) const
{
	return this->x == x && this->y == y;
}

/** @return true if the given x, y coordinate mathes this Coordinate's data */
bool Coordinate::is(Coordinate const & xy) const
{
	return xy.x == x && xy.y == y;
}

/** re-initialize */
void Coordinate::init(int a_x, int a_y)
{
	x = a_x;
	y = a_y;
}

const Coordinate Coordinate::ZERO = Coordinate();

#include <stdio.h>

void Coordinate::print(){ printf("(%d, %d)", x, y); }
