#include "charsurface.h"

#include <stdio.h>   // putchar
#include "consoleutil.h"


// define some utility macros, limited to this cpp file. ----------------------
/** resolve to the smaller of the 2 given values */
#define GETMIN(a, b) ((a < b)?a:b)
/** resolve to the larger of the 2 given values */
#define GETMAX(a, b) ((a > b)?a:b)
/** 
 * used for printing a CharSurface to something. Terminate with
 * ITERATE_SURFACE_OVER_2D_END
 */
#define ITERATE_SURFACE_OVER_2D_BEGIN(HEIGHT, WIDTH, SURF) \
	int minx = GETMAX(0, location.x); \
	int miny = GETMAX(0, location.y); \
	int maxx = GETMIN(WIDTH , location.x+SURF.getWidth()); \
	int maxy = GETMIN(HEIGHT, location.y+SURF.getHeight()); \
	for(int row = miny; row < maxy; ++row) { \
		for(int col = minx; col < maxx; ++col) { \
			int x = (col-location.x); \
			int y = (row-location.y);
/** terminates ITERATE_SURFACE_OVER_2D_BEGIN */
#define ITERATE_SURFACE_OVER_2D_END \
		} \
	}
// ----------------------------------------------------------------------------

/**
 * @param surface what surface to paste into this one
 * @param offset where in this surface to paste the given surface
 */
void CharSurface::paste(CharSurface const & surface, Coordinate location) {
	//int minx = GETMAX(0, location.x);
	//int miny = GETMAX(0, location.y);
	//int maxx = GETMIN(size.x, location.x+surface.getWidth());
	//int maxy = GETMIN(size.y, location.y+surface.getHeight());
	//for(int row = miny; row < maxy; ++row) {
	//	for(int col = minx; col < maxx; ++col) {
	//		int x = (col-location.x);
	//		int y = (row-location.y);
	ITERATE_SURFACE_OVER_2D_BEGIN(size.y, size.x, surface)
			map[row][col] = surface.map[y][x];
	ITERATE_SURFACE_OVER_2D_END
	//	}
	//}
}
	
void CharSurface::draw(Coordinate location){
	//int minx = GETMAX(0, location.x);
	//int miny = GETMAX(0, location.y);
	//int maxx = GETMIN(SCREEN_WIDTH,  location.x+getWidth());
	//int maxy = GETMIN(SCREEN_HEIGHT, location.y+getHeight());
	//for(int row = miny; row < maxy; ++row) {
	//	for(int col = minx; col < maxx; ++col) {
	//		int x = (col-location.x);
	//		int y = (row-location.y);
	ITERATE_SURFACE_OVER_2D_BEGIN(SCREEN_HEIGHT, SCREEN_WIDTH, (*this) )
			if(map[y][x] != 0) {
				moveCursor(col, row);
				putchar(map[y][x]);
			}
	ITERATE_SURFACE_OVER_2D_END
	//	}
	//}
}

#undef ITERATE_THIS_SURFACE_OVER_2D_END
#undef ITERATE_THIS_SURFACE_OVER_2D_BEGIN
#undef GETMAX
#undef GETMIN

/**
 * @return a location containing the given letter. (-1,-1) if letter does
 *         not exist in this map
 */
Coordinate CharSurface::getLocationOf(char letter) {
	for(int row = 0; row < getHeight(); ++row) {
		for(int col = 0; col < getWidth(); ++col) {
			if(map[row][col] == letter){
				return Coordinate(col, row);
			}
		}
	}
	return Coordinate(-1,-1);
}
