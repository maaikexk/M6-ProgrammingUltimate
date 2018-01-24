#pragma once

#include "Grid.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include <vector>

class Board{
private:
	int Gridsize = 3;

public:
	Grid grid[3][3];
	bool checkWin(char);
	bool isFull();
	int heuristicScoreBoard(char, char);
	int check(int, int);
	int oneCurrent, twoCurrent, oneOther, twoOther;
};