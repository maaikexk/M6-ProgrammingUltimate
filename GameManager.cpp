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
	int g, s;
	display();
	while (1) {
		display();
		cout << "\n  Player " << player << " - Enter Grid: ";
		cin >> g;
		if (g > 0 && g < 10) {
			break;
		}
		display();
		cout << "\n  Try again.";
		cin.get();
	}

	s = g;
	while (1) {
		display();
		if (currentBoard.checkWin(player)) {

			display();
			cout << "\n  Player " << player << " won!";
			cin.get();
			cin.ignore();
			break;
		}

		player = player == 'x' ? 'o' : 'x';
		cur = g;
		input(g);
	}
}

void GameManager::display()
{
	system("cls");
	cout << "\n  ULTIMATE TIC TAC TOE\n" << rule;

	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t k = 0; k < 3; ++k)
		{
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
}

int GameManager::pickNewGrid(int& g) {
	int input;
	if (player == 'x') {
		cout << "\n Player" << player << "- Enter grid";
		input = playerHuman.getNewGrid();
		if (currentBoard.grid[X(input)][Y(input)].checkFull()) {
			return 0;
		}

		else {
			display();
			cin.ignore(); cin.get();
			return input;
		}
	}

	if (player == 'o') {
		input = playerAI.getInput(g, currentBoard);
		return input;
	}

	return false;

	//if (player == 'o'
}

int GameManager::getFinalInput(int& g) {
	int input;
	if (player == 'x') {
		cout << "\n  Player " << player << " - Enter subgrid: ";
		input = playerHuman.getInput();
		//cin >> s;
		if (input > 0 && input < 10)
		{
			if (currentBoard.grid[X(g)][Y(g)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
		display();
		cout << "\n  Try again.";
		cin.ignore(); cin.get();
	}

	if (player == 'o') {
		input = playerAI.getInput(g, currentBoard);
		if (input > 0 && input < 10)
		{
			if (currentBoard.grid[X(g)][Y(g)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
		display();
	}
	return false;
}

void GameManager::input(int& g) {

	int input;
	while (1) {
		display();
		//cout << "\n" << player;

		if (currentBoard.grid[X(g)][Y(g)].checkFull()) {
			g = pickNewGrid(g);
		}
		//cin.ignore();
		input = getFinalInput(g);
		if (currentBoard.grid[X(g)][Y(g)].get(X(input),Y(input)) != '.') {
			continue;
		}
		cout << input;

		if (input < 10) {
			break;
		}
	}

	currentBoard.grid[X(g)][Y(g)].set(X(input), Y(input), player);

	if (currentBoard.grid[X(g)][Y(g)].winningCel(player)) {
		currentBoard.grid[X(g)][Y(g)].makeFull(player);
	}

	g = input;
}

