#include "Board.hpp"
#include "GameManager.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>

//calling the namespace so we don't have to call it each time
using namespace std;

string rule(80, '_');

//returns the x value
int X(int pos) {
	return pos / 3;
}

//returns the y value
int Y(int pos) {
	return pos % 3 - 1;
}

//the constructor
GameManager::GameManager() {

	//for each big grid[i][j]
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)

			//and each supgrid[k][l]
			for (size_t k = 0; k < 3; ++k)
				for (size_t l = 0; l < 3; ++l){

					//set the subgrid to empty
					currentBoard.grid[i][j].set(k, l, '.');
				}

	//the start player
	player = 'x';

	//the players playing
	playerAI = AIPlayer();
	playerHuman = HumanPlayer();
	cur = 0;
}

void GameManager::play() {
	int grid, s;
	//display();

	grid = 1;

	while (1) {
		//display();
		if (currentBoard.checkWin(player)) {
			//display();
			break;
		}

		player = player == 'x' ? 'o' : 'x';
		cur = grid;
		input(grid);
	}
}

/*void GameManager::display(){
	for (size_t i = 0; i < 3; ++i){
		for (size_t k = 0; k < 3; ++k){
			cout << "\n";
			char left, right;
			left = right = ' ';

			for (size_t j = 0; j < 3; ++j)
			{
				if (k == 1)
				{
					if (3 * i + j + 1 == cur) {
						left = '>';
						right = '<';
					}
					else {
						left = right = ' ';
					}
				}
				cout << "  " << left << " ";
				for (size_t l = 0; l < 3; ++l) {
					cout << currentBoard.grid[i][j].get(k, l) << " ";
				}
				cout << right;
			}
		}
		cout << "\n\n";
	}
	cout << "\n";
}*/

int GameManager::pickNewGrid(int& grid) {
	int input;
	if (player == 'x') {
		input = playerHuman.getNewGrid();
		if (currentBoard.grid[X(input)][Y(input)].checkFull()) {
			return 0;
		}

		else {
			//display();
			return input;
		}
	}

	if (player == 'o') {
		input = playerAI.getInput(grid, currentBoard);
		return input;
	}

	return false;
}

int GameManager::getFinalInput(int& grid) {
	int input;
	if (player == 'x') {
		input = playerHuman.getInput();
		
		if (input > 0 && input < 10)
		{
			if (currentBoard.grid[X(grid)][Y(grid)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
		//display();
		}

	if (player == 'o') {
		input = playerAI.getInput(grid, currentBoard);
		if (input > 0 && input < 10)
		{
			if (currentBoard.grid[X(grid)][Y(grid)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
		//display();
	}
	return false;
}

void GameManager::input(int& grid) {

	int input;
	while (1) {
		//display();

		if (currentBoard.grid[X(grid)][Y(grid)].checkFull()) {
			grid = pickNewGrid(grid);
		}

		input = getFinalInput(grid);
		if (currentBoard.grid[X(grid)][Y(grid)].get(X(input),Y(input)) != '.') {
			continue;
		}
		
		if (input < 10) {
			break;
		}
	}

	currentBoard.grid[X(grid)][Y(grid)].set(X(input), Y(input), player);

	if (currentBoard.grid[X(grid)][Y(grid)].winningCel(player)) {
		currentBoard.grid[X(grid)][Y(grid)].makeFull(player);
	}

	grid = input;
}

