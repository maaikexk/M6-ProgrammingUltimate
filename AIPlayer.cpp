#include "AIPlayer.hpp"
#include "Grid.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Game.hpp"

using namespace std;

int AIPlayer::getInput(Grid board) {
	std::vector<int> possibleMoves = board.getEmptyPositions();
	//std::vector<int> possibleGrids = ;
	int input;

	if (possibleMoves.size() <= 0) {
		std::vector<int> possibleGrids;
		Game currentGame;
		
		for (int i = 1; i <= 9; i++) {
			if (!currentGame.grid[i / 3][i % 3 - 1].checkFull()) {
				possibleGrids.push_back(i);
			}
		}
		input = rand() % possibleGrids.size() + 0;
		return possibleGrids[input];
	}
	else {
		input = rand() % possibleMoves.size() + 0;
		return possibleMoves[input];
	}
}