#pragma once
#include "entity.h"

class Player : public Entity {
	int maximumMoves;
	int currentMoveNumber;
	int score;
public:
	Player(int x, int y, char icon = 1, int maximumMoves = 100)
		: Entity(x, y, icon) {
		init(x, y, icon, maximumMoves);
	}

	void init(int x, int y, char icon = 1, int maximumMoves = 100) {
		setMaxMoves(maximumMoves);
		resetMoveCounter();
		resetScore();
	}

	int getMaxMoves() const { return maximumMoves; };
	int getScore() const { return score; }
	void setMaxMoves(int maximumMoves) { this->maximumMoves = maximumMoves; }
	void setScore(int score) { this->score = score; }

	bool updateMoveCounter() { 
		if (currentMoveNumber == maximumMoves) return false;
		++currentMoveNumber; 
		return true;
	}
	int getMovesLeft() const { return maximumMoves - currentMoveNumber; }
	void resetMoveCounter() { currentMoveNumber = 0; }
	
	void updateScore() { ++score; }
	void resetScore() { score = 0; }


};
