#pragma once

#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include "Board.hpp"
#include <ofSerial.h>

class GameManager {

private:
	int Gridsize = 3;

public:
	GameManager();
	void display();
	void play();
	void input(int& g);
	void getArduinoData();
	int pickNewGrid(int& g);
	int getFinalInput(int& g);
	char player;

	int nextGrid;
	int nextSubgrid;

	size_t cur;
	AIPlayer playerAI;
	//HumanPlayer playerHuman;
	Board currentBoard;
	ofSerial serial;

};
