#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include "Grid.hpp"

//important to include the class otherwise we cannot use it
class Board;

class AIPlayer {
private:
	int gridSize = 3;

public:
	void getInput(int, Board, char*);
	int getHeuristicsScore(Board, int, char, int);
	int calculateScore(Board, char);
	char otherPlayer(char);
};

#endif



