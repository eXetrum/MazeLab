#pragma once

#include "charsurface.h"

class Tile
{
	CharSurface surface;
public:
	const char * source;
	Coordinate location;

	void load(const char * filename);

	void draw(){ surface.draw(location); }

	Coordinate getSize(){ return surface.getSize(); }
	int getWidth()   { return surface.getWidth(); }
	int getHeight()  { return surface.getHeight(); }

	char get(int row, int col)             { return surface.get(row, col); }
	void set(int row, int col, char value) { return surface.set(row, col, value); }
	char * operator[](int row)             { return surface[row]; }

	Tile():source(0){}

	virtual ~Tile(){
		source=0;
		surface.release();
	}

	Tile & operator=(Tile const & other) {
		surface = other.surface; // calls overloaded operator
		source = other.source;
		location = other.location;
		return *this;
	}

	Tile(Tile & other){
		operator=(other);
	}

	CharSurface * getSurface() { return &surface; }

	int lastCol() { return surface.getWidth() -1; }
	int lastRow() { return surface.getHeight() -1; }

	/**
	 * @return true if all characters on last column of this->surface are the
	 *         same as the characters on the first column of n.surface
	 */
	bool acceptAsRightNeighbor(Tile & n);
	/**
	 * @return true if all characters on the last row of this->surface are the
	 *         same as the characters on the first row of n.surface
	 */
	bool acceptAsLowerNeighbor(Tile & n);

	/**
	 * swap characters horizontally in this Tile's surface so that
	 *
	 * ## ##                ## ##
	 * #   #                #   #
	 * #      would become      #
	 * #   #                #   #
	 * #####                #####
	 */
	void flipHorizontal();

	/**
	 * swap characters vertically in this Tile's surface so that
	 *
	 * ## ##                #####
	 * #   #                #   #
	 * #      would become  #    
	 * #   #                #   #
	 * #####                ## ##
	 */
	void flipVertical();
};
