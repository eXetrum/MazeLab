#include "tile.h"
#include "myassert.h"
#include <iostream>
using namespace std;

// NOTE: Good test code is often more complex than the code it is testing.
//       In this academic example, you can read the test code to get an idea
//       of how to implement the functions being tested.

/**
 * runs a suite of tests to determine if the given tile loaded correctly
 */
inline void testLoadedTile(Tile * t){
	assert(t->source != NULL && "tile knows it's own source");
	const char * fileName = "tile#.txt";
	int len = strlen(fileName);
	bool validSourceFileName = true;
	for(int i = 0; i < len; ++i){
		if(fileName[i] != '#' && fileName[i] != t->source[i]){
			validSourceFileName = false;
		}
	}
	assert(validSourceFileName && "are tile#.txt files being used?"
		" If intentionally using different data, comment-out this test");
	// getSize() returns by value, so operator!= must be const complete
	bool loadedSize = t->getSize() != Coordinate::ZERO;
	assert(loadedSize && "loaded size. Is Tile::load working?");
	bool correctSize = t->getSize() == Coordinate(5,5);
	assert(correctSize && "size is correct. Is tile#.txt file modified?");
	// check to see if data acually loaded
	bool loadedData = false;
	for(int r = 0; r < t->getSize().y; ++r) {
		for(int c = 0; c < t->getSize().y; ++c) {
			// de-reference the tile, otherwise [] uses poitner arithmetic
			if( (*t)[r][c] != 0)
				loadedData = true;
		}
	}
	assert(loadedData && "tile surface data loaded, and is not empty");
	// check to see if the files are exactly what is expected as test data
	char * files[7][5] = {
		{// tile0.txt
			"## ##",
			"#   #",
			"     ",
			"#   #",
			"## ##",
		},{// tile1.txt
			"#####",
			"#   #",
			"    #",
			"#   #",
			"#####",
		},{// tile2.txt
			"## ##",
			"#   #",
			"#   #",
			"#   #",
			"## ##",
		},{// tile3.txt
			"## ##",
			"#   #",
			"    #",
			"#   #",
			"#####",
		},{// tile4.txt
			"## ##",
			"#   #",
			"     ",
			"#   #",
			"#####",
		},{// tile5.txt
			"## ##",
			"#   #",
			"#   #",
			"#   #",
			"#####",
		},{// tile6.txt
			"## ##",
			"#   #",
			"#    ",
			"#   #",
			"## ##",
		},
	};
	int whichFile = (t->source[4]-'0');
	char ** expectedFile = files[whichFile];
	for(int row = 0; row < 5; ++row){
		for(int col = 0; col < 5; ++col){
			assert(expectedFile[row][col] == (*t->getSurface())[row][col] &&
				"file is exactly the same as expected");
		}
	}
}

void testFlipFunctions(Tile ** tileRs, int count)
{
	Tile * t, * tflip;
	for(int tileIndex = 0; tileIndex < count; ++tileIndex)
	{
		t = tileRs[tileIndex];
		tflip = new Tile(*t);
		tflip->flipHorizontal();
		CharSurface * s = t->getSurface(), * flips = tflip->getSurface();
		bool allCharactersFlippedCorrectly = true;
		///////////////////////////////////////////////////////////////////////
		for(int row = 0; row < t->getHeight(); ++row) {
			for(int col = 0; col < t->getWidth(); ++col) {
				int reversedCol = t->getWidth()-1-col;
				if((*s)[row][reversedCol] != (*flips)[row][col]) {
					allCharactersFlippedCorrectly = false;
					cout << "flipHorizontal failed on "<< tflip->source <<endl;
					assert(allCharactersFlippedCorrectly);
				}
			}
		}
		tflip->flipVertical();
		// mostly copy/pasted test code ///////////////////////////////////////
		for(int row = 0; row < t->getHeight(); ++row) {
			for(int col = 0; col < t->getWidth(); ++col) {
				int reversedCol = t->getWidth()-1-col;
				int reversedRow = t->getHeight()-1-row;
				if((*s)[reversedRow][reversedCol] != (*flips)[row][col]) {
					allCharactersFlippedCorrectly = false;
					cout << "flipVertical failed on " << tflip->source << endl;
					assert(allCharactersFlippedCorrectly);
				}
			}
		}
		delete tflip;
	}
}

inline void testAcceptAsXXXneighborMethods(Tile ** tileRs, int count,
											bool hasOpeningOnTop[],
											bool hasOpeningOnLeft[],
											bool hasOpeningOnBottom[],
											bool hasOpeningOnRight[])
{
	// now that tiles are known to be correct, test acceptAsXXXXNeighbor
	const char 
		* TEST1 = "acceptAsRightNeighbor", 
		* TEST2 = "acceptAsLowerNeighbor";
	// test every tile against every other tile
	for(int a = 0; a < count; ++a)
	{
		Tile * ta = tileRs[a];
		for(int b = 0; b < count; ++b)
		{
			Tile * tb = tileRs[b];
			const char * testingF;
			bool shouldWork, calculated;
			///////////////////////////////////////////////////////////////////
				testingF = TEST1;
				shouldWork = hasOpeningOnRight[a] == hasOpeningOnLeft[b];
				calculated = ta->acceptAsRightNeighbor(*tb);
			if(shouldWork == calculated) {
			// mostly copy/pasted test code ///////////////////////////////////
				testingF = TEST2;
				shouldWork = hasOpeningOnBottom[a] == hasOpeningOnTop[b];
				calculated = ta->acceptAsLowerNeighbor(*tb);
			}
			if(shouldWork != calculated) {
				ta->getSurface()->draw(Coordinate(1,1));
				tb->getSurface()->draw(
					(testingF == TEST1)
						?Coordinate(7,1)
						:Coordinate(1,7)
					);
				cout << endl << endl << "tile" << a 
					 << "." << testingF << "(tile" << b <<") should return";
				cout << (shouldWork?" true":" false") << endl << endl;
				cout << "incorrect code in Tile::" << testingF << endl
					<< endl;
				assert(calculated == shouldWork);
			}
		}
	}
}

inline void testTilesAreAsExpected(Tile ** tileRs, int count,
								   bool hasOpeningOnTop[],
								   bool hasOpeningOnLeft[],
								   bool hasOpeningOnBottom[],
								   bool hasOpeningOnRight[])
{
	const char * shouldMatch;
	Tile * t = tileRs[0];
	int WIDTH = t->getWidth();
	int HEIGHT = t->getHeight();
	const char * TESTABLE_OPEN =   "## ##";
	const char * TESTABLE_CLOSED = "#####";
// utility macro, so code doesn't get *manually* copy/pasted //////////////////
#define TEST_TILE_SIDE(testName, size, tileAccess)                        \
shouldMatch = testName[tileIndex]?TESTABLE_OPEN:TESTABLE_CLOSED;          \
for(int i = 0; i < size; ++i)                                             \
{                                                                         \
	bool testName##EdgeAsExpected = (*t) tileAccess == shouldMatch[ i ];  \
	assert(testName##EdgeAsExpected && "data doesn't conform to test");   \
}
///////////////////////////////////////////////////////////////////////////////
	for(int tileIndex = 0; tileIndex < count; ++tileIndex)
	{
		t = tileRs[tileIndex];
		// test top
		TEST_TILE_SIDE( hasOpeningOnTop, WIDTH, [0][i] )
		// test left
		TEST_TILE_SIDE( hasOpeningOnLeft, HEIGHT, [i][0] )
		// test bottom
		TEST_TILE_SIDE( hasOpeningOnBottom, WIDTH, [HEIGHT-1][i] )
		// test right
		TEST_TILE_SIDE( hasOpeningOnRight, HEIGHT, [i][WIDTH-1] )
	}
#undef TEST_TILE_SIDE
}

inline void testAcceptNeighborAndFlipFunctions(Tile ** tileRs, int count) {
	const int NUM_TESTABLE_TILES = 7;
	assert(count == NUM_TESTABLE_TILES && "expecting tile0.txt to tile6.txt");
	/* summary of tiles, all 5 wide and 5 high
		0		1		2		3		4		5		6
		## ##	#####	## ##	## ##	## ##	## ##	## ##
		#   #	#   #	#   #	#   #	#   #	#   #	#   #
					#	#   #	    #	     	#   #	#    
		#   #	#   #	#   #	#   #	#   #	#   #	#   #
		## ##	#####	## ##	#####	#####	#####	## ##
	*/
	bool hasOpeningOnTop[] = 
	{	true,	false,	true,	true,	true,	true,	true};
	bool hasOpeningOnLeft[] = 
	{	true,	true,	false,	true,	true,	false,	false};
	bool hasOpeningOnBottom[] = 
	{	true,	false,	true,	false,	false,	false,	true};
	bool hasOpeningOnRight[] = 
	{	true,	false,	false,	false,	true,	false,	true};
	// make sure tile edges are as expected by tests
	testTilesAreAsExpected(tileRs, count, hasOpeningOnTop, 
		hasOpeningOnLeft, hasOpeningOnBottom, hasOpeningOnRight);
	// check the neighbor methods
	testAcceptAsXXXneighborMethods(tileRs, count, hasOpeningOnTop, 
		hasOpeningOnLeft, hasOpeningOnBottom, hasOpeningOnRight);
	// check flip methods
	testFlipFunctions(tileRs, count);
}

inline void testThatTilesWereGenerated(Tile *** tiles, int a_rows, int a_cols)
{
	Tile * t;
	assert(tiles && "something was allocated for tiles by"
		"Application::generateRandomTiles()");
	for(int row = 0; row < a_rows; ++row) {
		assert(tiles && "something was allocated for each row of tiles by"
			"Application::generateRandomTiles()");
		for(int col = 0; col < a_cols; ++col) {
			t = tiles[row][col];
			assert(t != NULL && "all tile pointers are not null");
			assert(t->getWidth() > 0 && "all tiles have a valid width");
			assert(t->getHeight() > 0 && "all tiles have a valid height");
		}
	}
}
