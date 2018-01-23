#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include "Grid.hpp"


//class Game;
class Board;
class AIPlayer {
private:
	int gridSize = 3;

public:
	int getInput(int, Board);
	int getHeuristicsScore(Board, int, char, int);
	int calculateScore(Board, char);
	char otherPlayer(char);
};

#endif

