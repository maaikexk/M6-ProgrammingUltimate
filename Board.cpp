#include "Board.hpp"
#include "Grid.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>

//Board is the big grid and has an array of subgrid with which it uses

//fuction to check if there is a winner
bool Board::checkWin(char player) {

	//the current player
	char cell = player;

	//check the vertical grids
	for (int y = 0; y < Gridsize; y++) {
		if (grid[0][y].winningCel(cell) && grid[1][y].winningCel(cell) && grid[2][y].winningCel(cell)) {
			return true;
		}
	}

	//check the horizontal grids
	for (int x = 0; x < Gridsize; x++) {
		if (grid[x][0].winningCel(cell) && grid[x][1].winningCel(cell) && grid[x][2].winningCel(cell)) {
			return true;
		}
	}

	// check diagonals
	if (grid[0][0].winningCel(cell) && grid[1][1].winningCel(cell) && grid[2][2].winningCel(cell)) {
		return true;
	}
	if (grid[0][2].winningCel(cell) && grid[1][1].winningCel(cell) && grid[2][0].winningCel(cell)) {
		return true;
	}

	// if none is true, return false
	return false;
}

//check if the board is full
bool Board::isFull() {

	//check each grid, if one is not full retun false
	for (int i = 0; i < Gridsize; i++) {
		for (int j = 0; j < Gridsize; j++) {
			if (!grid[i][j].checkFull()) {
				return false;
			}
		}
	}

	//else return true
	return true;
}

//this fuction calculates the heuristicScore for the board (big grid)
int Board::heuristicScoreBoard(char currentPlayer, char otherPlayer) {

	//initializing the variables
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
			if (grid[j][k].winningCel(currentPlayer)) {
				current++;
			}

			//other player
			if (grid[j][k].winningCel(otherPlayer)) {
				other++;
			}
		}
		//call the function to bump the total score if only one player has moves in that row
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
			if (grid[j][k].winningCel(currentPlayer)) {
				current++;
			}

			//other palyer
			if (grid[j][k].winningCel(otherPlayer)) {
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
		if (grid[j][j].winningCel(currentPlayer)) {
			current++;
		}

		//other field
		if (grid[j][j].winningCel(otherPlayer)) {
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
		if (grid[j][2 - j].winningCel(currentPlayer)) {
			current++;
		}

		//other player
		if (grid[j][2 - j].winningCel(otherPlayer)) {
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

//function which checks and returns the variable used in the heuristics value
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

