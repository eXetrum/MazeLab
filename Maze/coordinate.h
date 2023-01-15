#pragma once

/**
 * Object Oriented implementation of a 2 dimensional Coordinate
 */
struct Coordinate
{
	/** the x, y coordinates */
	int x, y;

	static const Coordinate ZERO;

	/** initializes the Coordinate */
	Coordinate(int x, int y):x(x),y(y){}

	/** default constructor - sets x,y to 0,0 */
	Coordinate():x(0),y(0){}

	/** @return if the given x, y coordinate matches this Coordinate's data */
	bool is(int x, int y) const;

	/** @return if the given x, y coordinate matches this Coordinate's data */
	bool is(Coordinate const & xy) const ;

	/** re-initialize */
	void init(int x, int y);

	bool operator==(Coordinate const & v) const { return is(v); }
	bool operator!=(Coordinate const & v) const { return !is(v); }

	void print();
};
