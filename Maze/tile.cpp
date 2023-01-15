#include "tile.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/**
 * Loads a 2 dimensional array from a file. The width and height of the array
 * are written in human-readable plain-text, followed by a new line, and then
 * the 2D char array data to be loaded. The loaded 2D array is stored in the
 * CharSurface member named surface.
 *
 * @param filename name/path of the file to read
 */
void Tile::load(const char * filename) {
	source = filename;
	// TODO load the surface with a 2D array loaded from the file
	ifstream input(source);
	if (!input.is_open()) {
		cout << "Unable to open file: " << source << endl;
		return;
	}
	int x, y;
	input >> x >> y;
	surface.setSize(Coordinate(x, y));
	string line;
	size_t row = 0, col = 0;
	while (getline(input, line) && row < y) {
		if (line == "" || line == "\n") continue; // skip empty lines
  		for (size_t col = 0; col < x && col < line.size(); ++col) {
			surface.set(row, col, line.at(col));
		}
		row++;
	}
	input.close();

}

/**
 * @return true if all characters on last column of this->surface are the
 *         same as the characters on the first column of n.surface
 */
bool Tile::acceptAsRightNeighbor(Tile & n)
{
	bool isGoodNeighbor = true;
	
	for (int row = 0; row < surface.getHeight() && row < n.surface.getHeight() && isGoodNeighbor; ++row) {
		if (surface.get(row, lastCol()) != n.surface.get(row, 0)) {
			isGoodNeighbor = false;
		}
	}

	return isGoodNeighbor;
}
/**
 * @return true if all characters on the last row of this->surface are the
 *         same as the characters on the first row of n.surface
 */
bool Tile::acceptAsLowerNeighbor(Tile & n)
{
	bool isGoodNeighbor = true;
	
	for (int col = 0; col < surface.getWidth() && col < n.surface.getWidth() && isGoodNeighbor; ++col) {
		if (surface.get(lastRow(), col) != n.surface.get(0, col)) {
			isGoodNeighbor = false;
		}
	}

	return isGoodNeighbor;
}

/**
 * swap characters horizontally in this Tile's surface so that
 *
 * ## ##                ## ##
 * #   #                #   #
 * #      would become      #
 * #   #                #   #
 * #####                #####
 */
void Tile::flipHorizontal()
{
	for (int i = 0; i < surface.getHeight(); ++i) {
		for (int j = 0; j < surface.getWidth() / 2; ++j) {
			int back = lastCol() - j;
			char temp = surface.get(i, j);
			surface.set(i, j, surface.get(i, back));
			surface.set(i, back, temp);
		}
	}
}

/**
 * swap characters vertically in this Tile's surface so that
 *
 * ## ##                #####
 * #   #                #   #
 * #      would become  #    
 * #   #                #   #
 * #####                ## ##
 */
void Tile::flipVertical()
{
	for (int i = 0; i < surface.getWidth(); ++i) {
		for (int j = 0; j < surface.getHeight() / 2; ++j) {
			int back = lastRow() - j;
			char temp = surface.get(j, i);
			surface.set(j, i, surface.get(back, i));
			surface.set(back, i, temp);
		}
	}
}
