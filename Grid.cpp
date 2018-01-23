#include "Grid.hpp"
#include <cassert>

void Grid::set(int x, int y, char cell) {
	subgrid[x][y] = cell;
}

char Grid::get(int x, int y) {
	return subgrid[x][y];
}

bool Grid::checkFull() {
	for (int x = 0; x < Gridsize; x++) {
		for (int y = 0; y < Gridsize; y++) {
			if (subgrid[x][y] == '.') {
				return false;
			}
		}
	}
	return true;
}

bool Grid::winningCel(char cell) {
	for (int y = 0; y < Gridsize; y++) {
		if (subgrid[0][y] == cell && subgrid[1][y] == cell && subgrid[2][y] == cell) {
			makeFull(cell);
			return true;
		}
	}

	for (int x = 0; x < Gridsize; x++) {
		if (subgrid[x][0] == cell && subgrid[x][1] == cell && subgrid[x][2] == cell) {
			makeFull(cell);
			return true;
		}
	}

	// check diagonal
	if (subgrid[0][0] == cell && subgrid[1][1] == cell && subgrid[2][2] == cell) {
		makeFull(cell);
		return true;
	}
	if (subgrid[0][2] == cell && subgrid[1][1] == cell && subgrid[2][0] == cell) {
		makeFull(cell);
		return true;
	}
	return false;
}

void Grid::makeFull(char cell) {
	for (int i = 0; i < Gridsize; i++) {
		for (int j = 0; j < Gridsize; j++) {
			subgrid[i][j] = cell;
		}
	}
}

int Grid::heuristicScoreGrid(char currentPlayer, char otherPlayer) {

	if (winningCel(currentPlayer)) {
		return 200;
	}

	if (winningCel(otherPlayer)) {
		return -200;
	}

	if (checkFull()) {
		return 0;
	}
	int score = 0;
	int current = 0;
	int other = 0;

	oneCurrent = 0;
	oneOther = 0;
	twoCurrent = 0;
	twoOther = 0;

	for (int j = 0; j < Gridsize; j++) {
		current = 0;
		other = 0;
		for (int k = 0; k < Gridsize; k++) {

			//current player
			if (subgrid[j][k] == currentPlayer) {
				current++;
			}

			//other player
			if (subgrid[j][k] == otherPlayer) {
				other++;
			}
		}
		//call the function to bump the total score
		check(current, other);
	}

	for (int k = 0; k < Gridsize; k++) {
		current = 0;
		other = 0;

		for (int j = 0; j < Gridsize; j++) {
			//curent player
			if (subgrid[j][k] == currentPlayer) {
				current++;
			}

			//other palyer
			if (subgrid[j][k] == otherPlayer) {
				other++;
			}
		}

		//call the function to bump the total score
		check(current, other);
	}

	current = 0;
	other = 0;

	//check diagonal from left top till bottom right
	for (int j = 0; j < Gridsize; j++) {
		//current field
		if (subgrid[j][j] == currentPlayer) {
			current++;
		}

		//other field
		if (subgrid[j][j] == otherPlayer) {
			other++;
		}
	}
	//call the function to bump the total score
	check(current, other);

	current = 0;
	other = 0;

	//check diagonal from right top till bottom left
	for (int j = 0; j < Gridsize; j++) {

		//current player
		if (subgrid[j][2 - j] == currentPlayer) {
			current++;
		}

		//other player
		if (subgrid[j][2 - j] == otherPlayer) {
			other++;
		}
	}
	//call the function to bump the total score
	check(current, other);

	//calculate the final score and return it
	score = (3 * twoCurrent + oneCurrent) - (3 * twoOther + oneOther);
	return score;

}

int Grid::check(int current, int other) {
	if (current == 2) {
		twoCurrent++;
	}
	if (current == 1) {
		oneCurrent++;
	}
	if (other == 2) {
		twoOther++;
	}
	if (other == 1) {
		oneOther++;
	}

	//returns factors used for the calculation.
	return (oneCurrent, twoCurrent, oneOther, twoOther);
}

std::vector<int> Grid::getEmptyPositions() const {
	std::vector<int> empty_positions;

	for (int i = 1; i <= 9; i++) {
		if (subgrid[i / 3][i % 3 - 1] == '.') {
			empty_positions.push_back(i);
		}
	}
	return empty_positions;
}