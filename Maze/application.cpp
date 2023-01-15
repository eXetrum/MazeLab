#include "application.h"
#include <iostream>
using namespace std;
#include "consoleutil.h"
#include <conio.h>
#include "myassert.h"

#include "tiletestcode.h"

#include <time.h>


const char fillCharacter = '~';
const char emptyCharacter = ' ';
const char obstacleCharacter = '#';
const char playerCharacter = 1;
const char goalCharacter = 4;

char moveCommand[] = { "wasd" };

Tile * Application::getNewRandomTile() {
	//int whichTileToGet = rand() % numTileResources;
	int whichTileToGet = rand() % tileResources.size();
	return new Tile( *tileResources[whichTileToGet] );
}

// create a static counter to keep track of loops, break if too many happen
#define PREVENT_INFINITE_LOOP(MAX_LOOPS) { \
    static int timesLooped = 0; \
    assert(timesLooped++ < MAX_LOOPS && \
        "this brute force algorithm may be infinite-looping"); \
}

void Application::init()
{
	srand((unsigned)time);
	// Create player entity
	entities.push_back(new Entity(0, 0, playerCharacter));

	loadTileResources();
	// note: only use this test code with standard tile0 through tile6
	testAcceptNeighborAndFlipFunctions(&tileResources[0], numTileResources);

	bool mapIsGood = true;
	do{
		generateRandomTiles();
		testThatTilesWereGenerated(tiles, TILE_ROWS, TILE_COLS);
		ensureAllTilesFit();
		pasteTilesIntoMap();
		mapIsGood = isMapTraversable(' ');
		if(!mapIsGood){
			releaseTiles();
			// because this is a brute force algorithm, fail after 4096 tries
			PREVENT_INFINITE_LOOP( (1 << 12) ) // 2 toThe 12th power = 4096
		}
	} while(!mapIsGood); // re-generate if the map is not good

	state = Application::STATE_RUNNING;
}

/** loads tile resources from file data */
void Application::loadTileResources()
{
	char * tilemaps[] = { 
		"tile0.txt",
		"tile1.txt",
		"tile2.txt",
		"tile3.txt",
		"tile4.txt",
		"tile5.txt",
		"tile6.txt",
	};
	numTileResources = sizeof(tilemaps)/sizeof(tilemaps[0]);
	assert(numTileResources > 0 && "there is at least one tile");
	for(int i = 0; i < numTileResources; ++i) {
		tileResources.push_back(new Tile);
		tileResources[i]->load(tilemaps[i]);
		testLoadedTile(tileResources[i]);
	}
}

/**
 * Generates a two dimensional array of Tile pointers for this->tiles, (NOT
 * tileResources, that is already created). Assigns each pointer to a randomly
 * generated Tile from "Application::getNewRandomTile".
 *
 * The two-dimensional array is TILE_ROWS high (tiles = new Tile**[TILE_ROWS];)
 * and TILE_COLS wide (tiles[row] = new Tile*[TILE_COLS];).
 *
 * Each random Tile is given an x/y position according to its col/row.
 */
void Application::generateRandomTiles()
{
	tiles = new Tile**[TILE_ROWS];
	for (int row = 0; row < TILE_ROWS; ++row) {
		tiles[row] = new Tile*[TILE_COLS];
	}
	for (int row = 0; row < TILE_ROWS; ++row) {
		for (int col = 0; col < TILE_COLS; ++col) {
			tiles[row][col] = getNewRandomTile();
		}
	}
}

/** de-allocate all allocated tiles */
void Application::releaseTiles()
{
	for(int row = 0; row < TILE_ROWS; ++row)
	{
		for(int col = 0; col < TILE_COLS; ++col)
		{
			delete tiles[row][col];
		}
		delete [] tiles[row];
	}
	delete [] tiles;
}

void Application::releaseEntities() {
	for (size_t i = 0; i < entities.size(); ++i) {
		delete entities[i];
		entities[i] = NULL;
	}
	entities.clear();
}

/**
 * called when tiles[row][col] does not match its neighbors to the left/above
 */
void tileDoesntFit(Tile *** tiles, int row, int col, 
				   bool doesntMatchLeft, bool doesntMatchTop) {
	if(doesntMatchLeft){
		//tiles[row][col]->set(2, 1, '<');
		tiles[row][col]->flipHorizontal();
		if (col != 0) { // if not left-most column, check tile to left
			Tile* tileToLeft = tiles[row][col - 1];
			// After filp still does not fit
			if (tileToLeft->acceptAsRightNeighbor(*tiles[row][col]) == false) {
				// Replace with random tile
				delete tiles[row][col];
				tiles[row][col] = NULL;
				return; // Stop execution to prevent null pointer dereference exception
			}
		}
	}
	if(doesntMatchTop){
		//tiles[row][col]->set(1, 2, '^');
		tiles[row][col]->flipVertical();
		if (row != 0) { // if not top row, check tile above
			Tile* tileAbove = tiles[row - 1][col];
			// After flip still does not fit
			if (tileAbove->acceptAsLowerNeighbor(*tiles[row][col]) == false) {
				// Replace with random tile
				delete tiles[row][col];
				tiles[row][col] = NULL;
			}
		}
	}
}

/** logic that adjusts tiles so that they all connect correctly */
void Application::ensureAllTilesFit()
{
	int corner[] = { 1, 3, 5 };
	int topOrBotEdge[] = { 1, 3, 4, 5 };
	int leftOrRightEdge[] = { 1, 2, 3, 5, 6 };
	int inner[] = { 0, 1, 2, 3, 4, 5, 6 };

	// TOP LEFT CORNER
	int topLeftCorner = corner[rand() % (sizeof(corner) / sizeof(int))];
	tiles[0][0] = new Tile(*tileResources[topLeftCorner]);
	if (topLeftCorner == 1) tiles[0][0]->flipHorizontal();
	else if (topLeftCorner == 3) {
		tiles[0][0]->flipHorizontal();
		tiles[0][0]->flipVertical();
	}
	else if (topLeftCorner == 5) tiles[0][0]->flipVertical();

	// TOP RIGHT CORNER
	int topRightCorner = corner[rand() % (sizeof(corner) / sizeof(int))];
	tiles[0][TILE_COLS - 1] = new Tile(*tileResources[topRightCorner]);
	if (topRightCorner == 3) tiles[0][TILE_COLS - 1]->flipVertical();
	else if (topRightCorner == 5) tiles[0][TILE_COLS - 1]->flipVertical();

	// BOT LEFT CORNER
	int botLeftCorner = corner[rand() % (sizeof(corner) / sizeof(int))];
	tiles[TILE_ROWS - 1][0] = new Tile(*tileResources[botLeftCorner]);
	if (botLeftCorner == 1) tiles[TILE_ROWS - 1][0]->flipHorizontal();
	else if (botLeftCorner == 3) tiles[TILE_ROWS - 1][0]->flipHorizontal();

	// BOT RIGHT CORNER
	int botRightCorner = corner[rand() % (sizeof(corner) / sizeof(int))];
	tiles[TILE_ROWS - 1][TILE_COLS - 1] = new Tile(*tileResources[botRightCorner]);


	// Left and right borders
	for (int row = 1; row < TILE_ROWS - 1; ++row){
		int edge = leftOrRightEdge[rand() % (sizeof(leftOrRightEdge) / sizeof(int))]; // { 1, 2, 3, 5, 6 };
		tiles[row][0] = new Tile(*tileResources[edge]);
		if (edge == 1 || edge == 3) {
			tiles[row][0]->flipHorizontal();
		} 

		edge = leftOrRightEdge[rand() % (sizeof(leftOrRightEdge) / sizeof(int))]; // { 1, 2, 3, 5, 6 };
		tiles[row][TILE_COLS - 1] = new Tile(*tileResources[edge]);
		if (edge == 6) {
			tiles[row][TILE_COLS - 1]->flipHorizontal();
		}
	}


	// Top and bottom borders
	for (int col = 1; col < TILE_COLS - 1; ++col) {
		int edge = topOrBotEdge[rand() % (sizeof(topOrBotEdge) / sizeof(int))]; // { 1, 3, 4, 5 };
		tiles[0][col] = new Tile(*tileResources[edge]);
		if (edge == 3 || edge == 4 || edge == 5) {
			tiles[0][col]->flipVertical();
		}

		edge = topOrBotEdge[rand() % (sizeof(topOrBotEdge) / sizeof(int))]; // { 1, 3, 4, 5 };
		tiles[TILE_ROWS - 1][col] = new Tile(*tileResources[edge]);
	}

	// check if neighbors are valid. mark invalid neighbors
	Tile * t, * tileToLeft, * tileAbove;
	
	for(int row = 1; row < TILE_ROWS - 1; ++row)
	{
		bool isFlipped = false;
		for(int col = 1; col < TILE_COLS - 1; ++col)
		{
			// check if tile fits according to left and upper neighbors
			bool isGoodRightN = true, isGoodLowerN = true;
			t = tiles[row][col];
			t->draw();
			if(col != 0) { // if not left-most column, check tile to left
				tileToLeft = tiles[row][col-1];
				isGoodRightN = tileToLeft->acceptAsRightNeighbor(*t);
			}
			if(row != 0) { // if not top row, check tile above
				tileAbove = tiles[row-1][col];
				isGoodLowerN = tileAbove->acceptAsLowerNeighbor(*t);
			}
			// if this tile needs to change (flip, or be replaced)
			if(!isGoodRightN || !isGoodLowerN){
				// tell the function that is supposed to fix it
				tileDoesntFit(tiles, row, col, !isGoodRightN, !isGoodLowerN);
				// if this tile was replaced
				if(t != tiles[row][col]){
					tiles[row][col] = getNewRandomTile();
					col--; // make sure the replacement tile gets checked again
				}
			}
		}
	}
}

void Application::pasteTilesIntoMap(){
	// Resize map. Assuming that each tile have same dimensions
	int TILE_WIDTH = tiles[0][0]->getWidth();
	int TILE_HEIGHT = tiles[0][0]->getHeight();
	Coordinate dims(TILE_COLS * TILE_WIDTH, TILE_ROWS * TILE_HEIGHT);
	map.setSize(dims);
	// Fill map with tiles
	for (int row = 0; row < TILE_ROWS; ++row) {
		for (int col = 0; col < TILE_COLS; ++col) {
			CharSurface* s = tiles[row][col]->getSurface();
			
			Coordinate pos;
			if (row != 0 || col != 0) {
				if (col != 0) {
					pos = Coordinate(col * TILE_WIDTH - col, row * TILE_HEIGHT - row);
				}
				else {
					pos = Coordinate(col * TILE_WIDTH, row * TILE_HEIGHT - row);
				}				
			}
			else {
				// Shift by tile size
				pos = Coordinate(col * TILE_WIDTH, row * TILE_HEIGHT);
			}

			// Put new tile into map
			map.paste(*s, pos);

			// Uncomment for debug
			//system("cls");
			//map.draw(Coordinate());
			//system("pause");
		}
	}
}

/**
 * recursive floodfill implementation
 * @param map a CharSurface to apply a floodfill algorithm to
 * @param x column
 * @param y row
 * @param oldColor which character to replace
 * @param newColor what character to replace the oldColor character with
 */
void floodFill(CharSurface * map, int x, int y, char oldColor, char newColor){
	if(x >= 0 && x < map->getWidth()
	&& y >= 0 && y < map->getHeight()
	&& (*map)[y][x] == oldColor
	&& (*map)[y][x] != newColor)
	{
		(*map)[y][x] = newColor; // mark this "pixel", and move on
		floodFill(map, x-1, y  , oldColor, newColor);	// left
		floodFill(map, x  , y-1, oldColor, newColor);	// up
		floodFill(map, x+1, y  , oldColor, newColor);	// right
		floodFill(map, x  , y+1, oldColor, newColor);	// down
	}
}

bool Application::isMapTraversable(char traversableCharacter)
{
	// TODO this method could use floodFill to test if the map is traversable
	bool isTraversable = true;

	// Find first empty cell
	Coordinate start(-1, -1);
	
	for (int row = 0; row < map.getHeight() && start == Coordinate(-1, -1); ++row) {
		for (int col = 0; col < map.getWidth() && start == Coordinate(-1, -1); ++col) {
			if (map.get(row, col) == emptyCharacter) start = Coordinate(col, row);
		}
	}
	floodFill(&map, start.x, start.y, emptyCharacter, fillCharacter);

	// Move throught map border and execute floodfill for all empty cells at the edge of the map
	/*for (int row = 0; row < map.getHeight(); ++row) {
		if (map.get(row, 0) == emptyCharacter) {
			floodFill(&map, 0, row, emptyCharacter, fillCharacter);
		}
		if (map.get(row, map.getWidth() - 1) == emptyCharacter) {
			floodFill(&map, map.getWidth() - 1, row, emptyCharacter, fillCharacter);
		}
	}

	for (int col = 0; col < map.getWidth(); ++col) {
		if (map.get(0, col) == emptyCharacter) {
			floodFill(&map, col, 0, emptyCharacter, fillCharacter);
		}
		if (map.get(map.getHeight() - 1, col) == emptyCharacter) {
			floodFill(&map, col, map.getHeight() - 1, emptyCharacter, fillCharacter);
		}
	}*/
	
	
	// Debug
	map.draw(Coordinate(0, 0));	
	//system("pause");
	// Check if entire map is traversable 
	for (int row = 0; row < map.getHeight() && isTraversable; ++row) {
		for (int col = 0; col < map.getWidth() && isTraversable; ++col) {
			if (map.get(row, col) == emptyCharacter) {
				isTraversable = false;
			}
		}
	}
	
	// Revert fill
	for (int row = 0; row < map.getHeight(); ++row) {
		for (int col = 0; col < map.getWidth(); ++col) {
			if (map.get(row, col) == fillCharacter) {
				floodFill(&map, col, row, fillCharacter, emptyCharacter);
			}
		}
	}
	
	// Debug
	map.draw(Coordinate(0, 0));
	//system("pause");


	Player* player = (entities.size() > 0 ? (Player*)entities[0] : NULL);
	if (isTraversable && player != NULL) {

		// Find empty place for player
		Coordinate playerPos(-1, -1);

		for (int row = 0; row < map.getHeight() && playerPos == Coordinate(-1, -1); ++row) {
			for (int col = 0; col < map.getWidth() && playerPos == Coordinate(-1, -1); ++col) {
				if (map.get(row, col) == emptyCharacter) playerPos = Coordinate(col, row);
			}
		}
		// Put player to the start location
		player->setX(playerPos.x);
		player->setY(playerPos.y);	
		player->init(playerPos.x, playerPos.y, 1, map.getWidth() * map.getHeight());

		// Create goal objects
		for (int row = 0; row < TILE_ROWS; ++row) {
			for (int col = 0; col < TILE_COLS; ++col) {
				CharSurface* s = tiles[row][col]->getSurface();
				Coordinate pos(col * s->getWidth() + s->getWidth() / 2 - col, row * s->getHeight() + s->getHeight() / 2 - row);
				// Create new goal object
				entities.push_back(new Entity(pos.x, pos.y, goalCharacter));
				//map.set(pos.y, pos.x, goalCharacter);
			}
		}

	}

	return isTraversable;
}

void Application::release()
{
	releaseTiles();
	releaseEntities();
}

void Application::draw()
{
	if(map.getHeight() != 0) {
		map.draw(Coordinate::ZERO);
		for (size_t i = 0; i < entities.size(); ++i) {
			entities[i]->draw();
		}
	}
	else
	{
		for(int row = 0; row < TILE_ROWS; ++row)
		{
			for(int col = 0; col < TILE_COLS; ++col)
			{
				tiles[row][col]->draw();
			}
		}
	}
	if (entities.size() > 0) {
		Player* player = (Player*)entities[0];
		moveCursor(map.getWidth() + 2, 2);
		printf("%15s %10d\n", "Score: ", player->getScore());
		moveCursor(map.getWidth() + 2, 3);
		int left = player->getMovesLeft();
		printf("%15s %10d\n", "Moves left: ", left);
		moveCursor(0, map.getHeight());
	}

}

void Application::setUserInput(int input)
{
	userKeyPress = input;
}

void Application::update()
{
	Player* player = (entities.size() > 0 ? (Player*)entities[0] : NULL);
	switch(userKeyPress)
	{
	case 'w':
	case 's':
	case 'a':
	case 'd':

	{
		if (player != NULL) {
			// move the players, based on what key was pressed
			int whatMove = Entity::MOVE_NONE;
			// Determine whos move
			for (int i = 0; moveCommand[i] != '\0'; ++i) {
				if (moveCommand[i] == userKeyPress) {
					whatMove = i;
					break;
				}
			}
			// Proceed move operation
			if (whatMove != Entity::MOVE_NONE) {
				Coordinate prevPos = player->getPosition();
				player->move(whatMove);
				player->update();
				// Prevent move throught obstacles and out of the maze box
				if (
					(player->getY() < 0 || player->getY() >= map.getHeight() || player->getX() < 0 || player->getX() >= map.getWidth())
					|| (map.get(player->getY(), player->getX()) == obstacleCharacter)
					) {
					// Move back
					player->setX(prevPos.x);
					player->setY(prevPos.y);
				}
				else { // Find entity with same pos
					player->updateMoveCounter();
					for (size_t i = 1; i < entities.size(); ++i) {
						if (entities[i]->getPosition() == player->getPosition()) {
							player->updateScore();
							//map.set(entities[i]->getY(), entities[i]->getX(), emptyCharacter);
							delete entities[i];
							entities.erase(entities.begin() + i);
							break;
						}
					}
					if (entities.size() == 1) {
						state = STATE_GAME_WIN;
					}
					else if (player->getMovesLeft() == 0) {
						state = STATE_GAME_LOSE;
					}

				}

			}
		}
	}
		break;
	case 27:
		state = Application::STATE_QUIT;
		break;

	}

	/*for (size_t i = 1; i < entities.size(); ++i) {
		entities[i]->update();
	}*/
	


	userKeyPress = 0;
}
