#pragma once

#include "charsurface.h"
#include "tile.h"
#include "entity.h"
#include "player.h"
#include <vector>
using namespace std;

class Application
{
private:
	int state;
	int userKeyPress;

	/** list of tile resources */
	//Tile ** tileResources;
	int numTileResources;
	vector<Tile*> tileResources;
	

	/** loads tile resources from file data */
	void loadTileResources();

	static const int TILE_ROWS = 5, TILE_COLS = 8;
	/**
 	 * 2 dimensional array of pointers - all the tiles in the game
	 * Again, this is not a 3D array, it's a 2D array of pointers.
	 */
	Tile *** tiles;

	vector<Entity*> entities; // Entity at index=0 will be player entity

	/** make tiles a 2D array of pointers, pointing at random tiles */
	void generateRandomTiles();
	/** de-allocate all allocated tiles */
	void releaseTiles();
	/** de-allocate all entities objects */
	void releaseEntities();
	/** logic that adjusts tiles so that they all connect correctly */
	void ensureAllTilesFit();

	CharSurface map;

	/** put all tiles into the map */
	void pasteTilesIntoMap();

	/**
	 * check if the map is traversable using floodfill. 
	 * @param traversableCharacter which character is traversable
	 * @return true if every traversableCharacter is adjacent to another
	 *         traversableCharacter
	 */
	bool isMapTraversable(char traversableCharacter);

public:
	static const int
		STATE_INIT = 0,    // an application not yet fully initialized
		STATE_RUNNING = 1, // running
		STATE_QUIT = 2,    // user requested quit
		STATE_GAME_LOSE = 3,
		STATE_GAME_WIN = 4;


	/** @return Application::STATE_* */
	int getState(){ return state; }

	Application():state(STATE_INIT){}

	void init();
	void release();

	void draw();
	void setUserInput(int input);
	void update();

	Tile * getNewRandomTile();
};
