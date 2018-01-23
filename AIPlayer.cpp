#include "AIPlayer.hpp"
#include "Grid.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include "Board.hpp"

using namespace std;

int AIPlayer::getInput(int grid, Board board) {
	std::vector<int> possibleMoves = board.grid[grid / 3][grid % 3 - 1].getEmptyPositions();
	//std::vector<int> possibleGrids = ;
	int input;
	int bestScore = -1000000;
	int depth = 1;

	Board currentBoard = board;

	//if no subgrids are available and it has to choose a grid
	if (possibleMoves.size() <= 0) {
		std::vector<int> possibleGrids;

		for (int i = 1; i <= 9; i++) {
			if (!currentBoard.grid[i / 3][i % 3 - 1].checkFull()) {
				possibleGrids.push_back(i);
			}
		}

		for (int i = 0; i < possibleGrids.size(); i++) {
			std::vector<int> newMoves = currentBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].getEmptyPositions();
			for (int j = 0; j < newMoves.size(); j++) {
				Board newBoard = currentBoard;
				newBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].set(newMoves[j] / 3, newMoves[j] % 3 - 1, 'o');

				int moveScore = -getHeuristicsScore(newBoard, newMoves[j], otherPlayer('o'), depth);

				if (moveScore > bestScore) {
					input = i;
					bestScore = moveScore;
				}
			}
		}
		return possibleGrids[input];
	}


	else {
		for (int i = 0; i < possibleMoves.size(); i++) {
			Board newBoard = currentBoard;
			newBoard.grid[grid / 3][grid % 3 - 1].set(possibleMoves[i] / 3, possibleMoves[i] % 3 - 1, 'o');

			int moveScore = -getHeuristicsScore(newBoard, possibleMoves[i], otherPlayer('o'), depth);

			if (moveScore > bestScore) {
				input = i;
				bestScore = moveScore;
			}
		}
		return possibleMoves[input];
	}
}

int AIPlayer::getHeuristicsScore(Board currentBoard, int grid, char currentPlayer, int depth) {

	if (currentBoard.checkWin(currentPlayer)) {
		return 10000;
	}

	if (currentBoard.checkWin(otherPlayer(currentPlayer))) {
		return -10000;
	}

	if (currentBoard.isFull()) {
		return 0;
	}

	std::vector<int> possibleMoves = currentBoard.grid[grid / 3][grid % 3 - 1].getEmptyPositions();
	//std::vector<int> possibleGrids = ;
	//int input;
	int bestScore = -1000;
	int maxDepth = 4;

	//Board currentBoard = board;

	//if no subgrids are available and it has to choose a grid
	if (possibleMoves.size() <= 0) {
		std::vector<int> possibleGrids;

		for (int i = 1; i <= 9; i++) {
			if (!currentBoard.grid[i / 3][i % 3 - 1].checkFull()) {
				possibleGrids.push_back(i);
			}
		}

		for (int i = 0; i < possibleGrids.size(); i++) {
			std::vector<int> newMoves = currentBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].getEmptyPositions();
			for (int j = 0; j < newMoves.size(); j++) {
				Board newBoard = currentBoard;
				newBoard.grid[possibleGrids[i] / 3][possibleGrids[i] % 3 - 1].set(newMoves[j] / 3, newMoves[j] % 3 - 1, currentPlayer);

				int score;

				if (depth >= maxDepth) {
					score = calculateScore(newBoard, currentPlayer);
				}

				else {
					int newdepth = depth + 1;
					score = -getHeuristicsScore(newBoard, newMoves[j], otherPlayer(currentPlayer), newdepth);
				}

				if (score > bestScore) {
					bestScore = score;
				}
			}
		}

		return bestScore;
	}


	else {
		for (int i = 0; i < possibleMoves.size(); i++) {
			Board newBoard = currentBoard;
			newBoard.grid[grid / 3][grid % 3 - 1].set(possibleMoves[i] / 3, possibleMoves[i] % 3 - 1, currentPlayer);

			int score = -10000;

			if (depth >= maxDepth) {
				score = calculateScore(newBoard, currentPlayer);
			}

			else {
				int newdepth = depth + 1;
				score = -getHeuristicsScore(newBoard, possibleMoves[i], otherPlayer(currentPlayer), newdepth);
			}

			if (score > bestScore) {
				bestScore = score;
			}
		}

		return bestScore;
	}
}

int AIPlayer::calculateScore(Board currentBoard, char currentPlayer) {
	int totalScore = 0;

	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			int score = currentBoard.grid[i][j].heuristicScoreGrid(currentPlayer, otherPlayer(currentPlayer));
			totalScore = totalScore + score;
		}
	}

	int score = currentBoard.heuristicScoreBoard(currentPlayer, otherPlayer(currentPlayer));
	totalScore = totalScore + 3 * score;

	return totalScore;
}

char AIPlayer::otherPlayer(char currentplayer) {
	if (currentplayer == 'o') {
		return 'x';
	}

	if (currentplayer == 'x') {
		return 'o';
	}
}