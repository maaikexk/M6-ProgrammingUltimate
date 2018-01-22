#include "Game.hpp"
#include "Grid.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

bool Game::checkWin(char player) {
	char cell = player;
	for (int y = 0; y < Gridsize; y++) {
		if (grid[0][y].winningCel(cell) && grid[1][y].winningCel(cell) && grid[2][y].winningCel(cell)) {
			return true;
		}
	}

	for (int x = 0; x < Gridsize; x++) {
		if (grid[x][0].winningCel(cell) && grid[x][1].winningCel(cell) && grid[x][2].winningCel(cell)) {
			return true;
		}
	}

	// check diagonal
	if (grid[0][0].winningCel(cell) && grid[1][1].winningCel(cell) && grid[2][2].winningCel(cell)) {
		return true;
	}
	if (grid[0][2].winningCel(cell) && grid[1][1].winningCel(cell) && grid[2][0].winningCel(cell)) {
		return true;
	}

	return false;
}

