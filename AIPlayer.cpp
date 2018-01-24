#include "AIPlayer.hpp"
#include "Grid.hpp"
#include "Board.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>

//AIPlayer returns a grid or move by calculating a heuristicScore

//the fuction which returns the input
int AIPlayer::getInput(int grid, Board board) {

	//possible moves of the given grid
	std::vector<int> possibleMoves = board.grid[grid / 3][grid % 3 - 1].getEmptyPositions();

	//input, score and depth initialized
	int input;
	int bestScore = -1000000;
	int depth = 1;

	//The board we are playing on
	Board currentBoard = board;

	//if no subgrids are available and it has to choose a grid
	if (possibleMoves.size() <= 0) {
		//makes a vector array of the availible grids
		std::vector<int> possibleGrids;

		//fills the array by checking what is available
		for (int i = 1; i <= 9; i++) {
			if (!currentBoard.grid[i / 3][i % 3 - 1].checkFull()) {
				possibleGrids.push_back(i);
			}
		}

		//if there is only one possibility we can imidiatly return the set
		if (possibleGrids.size() == 1) {
			return possibleGrids[0];
		}

		//for each grid it will check the possible moves
		for (int i = 0; i < possibleGrids.size(); i++) {
			//array of possible moves in grid i
			std::vector<int> newMoves = currentBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].getEmptyPositions();

			//loops through all the possible moves
			for (int j = 0; j < newMoves.size(); j++) {

				//makes new board which we can use
				Board newBoard = currentBoard;

				//set the move and afterwards get the score
				newBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].set(newMoves[j] / 3, newMoves[j] % 3 - 1, 'o');
				int moveScore = -getHeuristicsScore(newBoard, newMoves[j], otherPlayer('o'), depth);

				//sees if the score is better than the previous ones
				if (moveScore > bestScore) {
					input = i;
					bestScore = moveScore;
				}
			}
		}
		//return the best grid
		return possibleGrids[input];
	}

	//if there are moves availible then we can loop through those possible moves
	else {
		for (int i = 0; i < possibleMoves.size(); i++) {
			//new board we can play on
			Board newBoard = currentBoard;

			//set the set and calculates the score afterwards
			newBoard.grid[grid / 3][grid % 3 - 1].set(possibleMoves[i] / 3, possibleMoves[i] % 3 - 1, 'o');
			int moveScore = -getHeuristicsScore(newBoard, possibleMoves[i], otherPlayer('o'), depth);

			//sees if the score is better than the previous scores
			if (moveScore > bestScore) {
				input = i;
				bestScore = moveScore;
			}
		}
		//returns the best score
		return possibleMoves[input];
	}
}

//the heuristics player which is called till max depth is reached
int AIPlayer::getHeuristicsScore(Board currentBoard, int grid, char currentPlayer, int depth) {

	//Check if there is a winner, loser or a tie
	if (currentBoard.checkWin(currentPlayer)) {
		return 10000;
	}

	if (currentBoard.checkWin(otherPlayer(currentPlayer))) {
		return -10000;
	}

	if (currentBoard.isFull()) {
		return 0;
	}

	//all the possible moves
	std::vector<int> possibleMoves = currentBoard.grid[grid / 3][grid % 3 - 1].getEmptyPositions();

	//initializing best score and maxDepth
	int bestScore = -1000;
	int maxDepth = 5;

	//if no subgrids are available and it has to choose a grid
	if (possibleMoves.size() <= 0) {

		//vector array of the possibleGrids
		std::vector<int> possibleGrids;

		//fills the array with the possible grids
		for (int i = 1; i <= 9; i++) {
			if (!currentBoard.grid[i / 3][i % 3 - 1].checkFull()) {
				possibleGrids.push_back(i);
			}
		}

		//Loops thought the possible grids
		for (int i = 0; i < possibleGrids.size(); i++) {

			//possible moves of grid i
			std::vector<int> newMoves = currentBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].getEmptyPositions();

			//loops though those possible moves
			for (int j = 0; j < newMoves.size(); j++) {

				//board we are playing on
				Board newBoard = currentBoard;

				//make a set
				newBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].set(newMoves[j] / 3, newMoves[j] % 3 - 1, currentPlayer);
				int score;

				//if max depth is reached calculate the heuristicsscore
				if (depth >= maxDepth) {
					score = calculateScore(newBoard, currentPlayer);
				}

				//otherwise let the other player play
				else {
					int newdepth = depth + 1;
					score = -getHeuristicsScore(newBoard, newMoves[j], otherPlayer(currentPlayer), newdepth);
				}

				//see if the score is better than the previous one
				if (score > bestScore) {
					bestScore = score;
				}
			}
		}

		//return the best score
		return bestScore;
	}

	//if there are moves on the given grid we will loop though these
	else {
		for (int i = 0; i < possibleMoves.size(); i++) {

			//board we are playing on
			Board newBoard = currentBoard;

			//make a set
			newBoard.grid[grid / 3][grid % 3 - 1].set(possibleMoves[i] / 3, possibleMoves[i] % 3 - 1, currentPlayer);
			int score = -10000;

			//if max depth is reached calculate the score
			if (depth >= maxDepth) {
				score = calculateScore(newBoard, currentPlayer);
			}

			//otherwise let the other player play a set
			else {
				int newdepth = depth + 1;
				score = -getHeuristicsScore(newBoard, possibleMoves[i], otherPlayer(currentPlayer), newdepth);
			}

			//see if the calculated score is better than the previous ones
			if (score > bestScore) {
				bestScore = score;
			}
		}

		//return the best score
		return bestScore;
	}
}

//calculates the score for the entire board
int AIPlayer::calculateScore(Board currentBoard, char currentPlayer) {
	int totalScore = 0;

	//for each of the 9 grids it will calculate a score
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {

			//calling the fuction which calculates the score
			int score = currentBoard.grid[i][j].heuristicScoreGrid(currentPlayer, otherPlayer(currentPlayer));

			//if the grid is one of the cornergrids we want to double the score
			if ((i == 0 && j == 0) || (i == 0 && j == 2) || (i == 0 && j == 2) || (i == 2 && j == 2)) {
				totalScore = totalScore + 2 * score;
			}

			//if the grid is the middle one we want to tripple the score
			else if (i == 1 && j == 1) {
				totalScore = totalScore + 3 * score;
			}

			//otherwise we will have the normal score
			else {
				totalScore = totalScore + score;
			}
		}
	}

	//adds the score calculated for the big grid and doubles it
	int score = currentBoard.heuristicScoreBoard(currentPlayer, otherPlayer(currentPlayer));
	totalScore = totalScore + 2 * score;

	//return the score calculated
	return totalScore;
}

//switches from one player to the other
char AIPlayer::otherPlayer(char currentplayer) {
	if (currentplayer == 'o') {
		return 'x';
	}

	if (currentplayer == 'x') {
		return 'o';
	}
}