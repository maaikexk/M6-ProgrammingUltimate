#include "Grid.hpp"
#include <cassert>

//sets a subgrid to a certain player
void Grid::set(int x, int y, char cell) {
	subgrid[x][y] = cell;
}

//gets the subgrid
char Grid::get(int x, int y) {
	return subgrid[x][y];
}

//check if the subgrid is full
bool Grid::checkFull() {

	//for each grid check if one is empty, if so return false
	for (int x = 0; x < Gridsize; x++) {
		for (int y = 0; y < Gridsize; y++) {
			if (subgrid[x][y] == '.') {
				return false;
			}
		}
	}

	//else return true
	return true;
}

//check if the grid is won and makes the cell full if so
bool Grid::winningCel(char cell) {

	//check vertical
	for (int y = 0; y < Gridsize; y++) {
		if (subgrid[0][y] == cell && subgrid[1][y] == cell && subgrid[2][y] == cell) {
			makeFull(cell);
			return true;
		}
	}

	//check horizontal
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

	//if not won yet return false
	return false;
}

//fills the entire grid with the player given to show that it is won by that player
void Grid::makeFull(char player) {
	for (int i = 0; i < Gridsize; i++) {
		for (int j = 0; j < Gridsize; j++) {
			subgrid[i][j] = player;
		}
	}
}

//calculates the heuristics scores
int Grid::heuristicScoreGrid(char currentPlayer, char otherPlayer) {

	//check of there is a winner, loser or a tie
	if (winningCel(currentPlayer)) {
		return 200;
	}

	if (winningCel(otherPlayer)) {
		return -200;
	}

	if (checkFull()) {
		return 0;
	}

	//initialising the variables
	int score = 0;
	int current = 0;
	int other = 0;

	oneCurrent = 0;
	oneOther = 0;
	twoCurrent = 0;
	twoOther = 0;

	//check horizontal
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
		if (current == 0 || other == 0) {
			check(current, other);
		}
	}

	//check vertical
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
		if (current == 0 || other == 0) {
			check(current, other);
		}
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
	if (current == 0 || other == 0) {
		check(current, other);
	}

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
	if (current == 0 || other == 0) {
		check(current, other);
	}

	//calculate the final score and return it
	score = (3 * twoCurrent + oneCurrent) - (3 * twoOther + oneOther);
	return score;

}

// function which checks and returns the variable used in the heuristics value
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

//returning a vector array with emptyPositions
std::vector<int> Grid::getEmptyPositions() const {

	//making an array
	std::vector<int> empty_positions;

	//for each subgrid check if its empty, if zo push into the array
	for (int i = 1; i <= 9; i++) {
		if (subgrid[i / 3][i % 3 - 1] == '.') {
			empty_positions.push_back(i);
		}
	}

	//return the vector array
	return empty_positions;
}