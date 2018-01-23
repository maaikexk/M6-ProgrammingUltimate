#include "Board.hpp"
#include "Grid.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>

using namespace std;

bool Board::checkWin(char player) {
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

bool Board::isFull() {
	for (int i = 0; i < Gridsize; i++) {
		for (int j = 0; j < Gridsize; j++) {
			if (!grid[i][j].checkFull()) {
				return false;
			}
		}
	}
	return true;
}

int Board::heuristicScoreBoard(char currentPlayer, char otherPlayer) {
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
			if (grid[j][k].winningCel(currentPlayer)) {
				current++;
			}

			//other player
			if (grid[j][k].winningCel(otherPlayer)) {
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
			if (grid[j][k].winningCel(currentPlayer)) {
				current++;
			}

			//other palyer
			if (grid[j][k].winningCel(otherPlayer)) {
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
		if (grid[j][j].winningCel(currentPlayer)) {
			current++;
		}

		//other field
		if (grid[j][j].winningCel(otherPlayer)) {
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
		if (grid[j][2 - j].winningCel(currentPlayer)) {
			current++;
		}

		//other player
		if (grid[j][2 - j].winningCel(otherPlayer)) {
			other++;
		}
	}
	//call the function to bump the total score
	check(current, other);

	//calculate the final score and return it
	score = (3 * twoCurrent + oneCurrent) - (3 * twoOther + oneOther);
	return score;

}

int Board::check(int current, int other) {
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

