#pragma once

#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Board.hpp"

class GameManager {

private:
	int Gridsize = 3;

public:
	GameManager();
	void display();
	void play();
	void input(int& g);
	int pickNewGrid(int& g);
	int getFinalInput(int& g);
	char player;
	size_t cur;
	AIPlayer playerAI;
	HumanPlayer playerHuman;
	Board currentBoard;
};
