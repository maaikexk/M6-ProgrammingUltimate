#include "AIPlayer.hpp"
#include "Grid.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Game.hpp"

using namespace std;

int AIPlayer::getInput(Grid board) {
	std::vector<int> possibleMoves = board.getEmptyPositions();

	/*if (possibleMoves.size = 0) {
		std::vector<int> possibleGrids = getEmptyGrids();
	}*/

	int input= rand() % possibleMoves.size() + 0;
	return input;
}