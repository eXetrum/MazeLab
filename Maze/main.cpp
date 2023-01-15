#include "application.h"
#include <conio.h>
//#include "vld.h"

int main()
{
	Application app;
	app.init();
	while(app.getState() == Application::STATE_RUNNING)
	{
		app.draw();
		app.setUserInput(_getch());
		app.update();
	}
	if (app.getState() == Application::STATE_GAME_WIN) {
		printf("Congratulations you win the game ;)");
	}
	else if(app.getState() == Application::STATE_GAME_LOSE) {
		printf("Sorry you lose the game ;(");
	}
	app.release();
	
	return 0;
}

// INSTRUCTIONS
// ------------
// Compile this code. It should compile, but will crash immediately, 
// identifying that code is broken. Don't be alarmed, that is intentional.
// Prepare for a small taste of Test Driven Development!
//
// Read through this code! Try to understand it before starting the assignment.
// Once you feel comfortable with this code, accomplish each of the following,
// and make sure your code compiles and "runs" after each step is completed.
// 
// 1) Test Driven Development
//   a) Implement "Tile::load" so that it functions as the documentation above
//     its method signature implies. Be sure to compile and test that it works.
//     Expect asserts testing the next function should crash your program!
//   b) Implement "Tile::acceptAsLowerNeighbor" so it functions as documented.
//   c) Implement "Tile::acceptAsRightNeighbor" so it functions as documented.
//   d) Implement "Tile::flipHorizontal" so that it functions as documented.
//   e) Implement "Tile::flipVertical" so that it functions as documented.
//   f) Implement "Application::generateRandomTiles()" so it functions as 
//      documented.
//   g) Once the tiles generate, there should be a map of tiles, 8 wide and 5
//      high, with arrows pointing out incorrect tile neighbors. The code that
//      draws these arrows is at "Application::ensureAllTilesFit()", and is
//      there to be an example of logic for determining the fit of tiles.
// 2) Feel Good About Your Progress
//   a) Take a moment to appreciate that your code compiles and runs without
//      crashing!
//   b) Don't take too long though. There is plenty more left to do in this
//      assignment!
// 3) Fix the Tiles
//   a) Examine the code in "tileDoesntFit", which is called by
//      "Application::ensureAllTilesFit()". Notice that it marks tiles after
//      using the "acceptAsRightNeighbor" and "acceptAsLowerNeighbor" functions
//      to determine if the tile is placed properly. Modify "tileDoesntFit".
//   b) Tiles with incorrect left/right neighbors should flip horizontally and
//      be checked again.
//   c) Tiles with incorrect top/bottom neighbors should flip vertically and
//      be checked again.
//   d) If after flips, if the tile still does not fit, replace it with a
//      random tile. Make sure that the replacement tile is assigned to the
//      proper "tiles[row][col]" pointer!
// 4) Compile Into a Map
//   a) Paste all of the tiles into a single map. Use the stub function
//      "Application::pasteTilesIntoMap()" to implement the algorithm. The map
//      should be large enough that every tile can be pasted into it.
//      The goal is for all of the map to be in one CharSurface, so it can be
//      printed with one call (instead of a nested loop of calls), and ALSO so
//      that logic can be done on just one structure.
//   b) Notice that Application::draw is already set up to draw the map
//      CharSurface instead of every individual tile in the 2D array, once the
//      map has a valid height.
// 5) Flood Fill Test
//   a) Implement "Application::isMapTraversable(char traversableCharacter)".
//      A floodfill algorithm could be used to test if entire map is 
//      traversable. See the "floodFill" function in "application.cpp".
//   b) If the map is not fully traversable, re-generate the Tile table and map
//      until it is.
// 6) Make A Game!
//   a) Create an Entity class for the player to control. The player should
//      move around in clear spaces in the maze, using 'w', 'a', 's', and 'd'.
//   b) The player Entity should not be able to move out of the maze, or 
//      through walls.
//   c) Generate a 'coin' in each room. Reaching a coin with the player Entity
//      should make the coin disappear. When all coins have dissapeared, end
//      the game in victory.
//   d) At the beginning of the game, a maximumMoves variable should be
//      calculated, equal to the width*height of the map. If the player moves 
//      this many times, end the game in loss. Display the moves left as the
//      game is being played

// Achievements: (if you finished, and would like a challenge, try these)
//
// [Resource Manager] Use a list data structure instead of "tileResources" and
//     "numTileResources".
// [Data Driven Tilemaze] Load the list of strings in 
//     "Application::loadTileResources" from a file.
// [Page Flipping] Instead of having the map and player print directly to the
//     screen, the map and player print to a "screenBuffer" CharSurface large
//     enough to fill the screen, and only print that "screenBuffer".
// [More Map, Less Duplication] Place the 5x5 tiles so that they only take up
//     4x4 space by overlapping identical edges.
// [Your Map is So Big!] The map is much larger than the initial 5 x 8 tiles,
//     and when the player moves off of the displayed area of the map, the
//     displayed area offsets (scrolls) to show the player.
// [Less Function Calls, Still Filling] Replace the simple recursive floodfill
//     algorithm with a stack or queue based implementation of floodfill.
// [An Elegant Algorithm For a More Civilized Game] Use a different map
//     generation algorithm (still using tiles) other than the brute-force
//     implementation implied by this assignment.
