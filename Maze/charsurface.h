#pragma once

#include "coordinate.h"

class CharSurface {
	Coordinate size;
	char ** map;
public:
	CharSurface():map(0){}
	int getHeight() const {return size.y; }
	int getWidth() const {return size.x; }
	Coordinate getSize(){ return size; }

	char get(int row, int col)             { return map[row][col]; }
	void set(int row, int col, char value) { map[row][col] = value; }
	char * operator[](int a_row)           { return map[a_row]; }

	char ** getMap(){ return map; }

	void release(){
		if(map) {
			for(int row = 0; row < size.y; ++row) {
				delete [] map[row];
			}
			delete [] map;
			map = 0; // same as map = NULL;
		}
	}
	/**
	 * @param size how big to make this Map2 (filled with 0)
	 */
	void setSize(Coordinate size){
		release();
		this->size = size;
		map = new char * [getHeight()];
		for(int row = 0; row < size.y; ++row) {
			map[row] = new char [getWidth()];
			for(int col = 0; col < size.x; ++col) {
				map[row][col] = 0;
			}
		}
	}

	/**
	 * @param surface what surface to paste into this one
	 * @param offset where in this surface to paste the given surface
	 */
	void paste(CharSurface const & surface, Coordinate offset);

	~CharSurface(){ release(); }
	CharSurface & operator=(CharSurface const & other){
		setSize(other.size);
		paste(other, Coordinate::ZERO);
		return *this;
	}
	CharSurface(CharSurface const & surface):map(0) {
		operator=(surface);
	}
	void draw(Coordinate location);

	/**
	 * @return a location containing the given letter. (-1,-1) if letter does
	 *         not exist in this map
	 */
	Coordinate getLocationOf(char letter);
};
