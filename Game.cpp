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

int X(int pos) {
	return pos / 3;
}

int Y(int pos) {
	return pos % 3 - 1;
}

string rule(80, '_');

Game::Game() {
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
			for (size_t k = 0; k < 3; ++k)
				for (size_t l = 0; l < 3; ++l)
				{
					grid[i][j].set(k, l, '.');
				}
	player = 'x';
	playerAI = AIPlayer();
	playerHuman = HumanPlayer();
	cur = 0;
}

void Game::play() {
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
		if (checkWin()) {

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

void Game::display()
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
					cout << grid[i][j].get(k, l) << " ";
				}
				cout << right;
			}
		}
		cout << "\n\n";
	}
	cout << "\n";
}

int Game::pickNewGrid(int& g) {
	int input;
	if (player == 'x') {
		cout << "\n Player" << player << "- Enter grid";
		input = playerHuman.getNewGrid();
		if (grid[X(input)][Y(input)].checkFull()) {
			return 0;
		}

		else {
			display();
			cin.ignore(); cin.get();
			return input;
		}
	}

	if (player == 'o') {
		input = playerAI.getInput(grid[X(g)][Y(g)]);
		return input;
	}

	return false;

	//if (player == 'o'
}

int Game::getFinalInput(int& g) {
	int input;
	if (player == 'x') {
		cout << "\n  Player " << player << " - Enter subgrid: ";
		input = playerHuman.getInput();
		//cin >> s;
		if (input > 0 && input < 10)
		{
			if (grid[X(g)][Y(g)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
		display();
		cout << "\n  Try again.";
		cin.ignore(); cin.get();
	}

	if (player == 'o') {
		input = playerAI.getInput(grid[X(g)][Y(g)]);
		if (input > 0 && input < 10)
		{
			if (grid[X(g)][Y(g)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
		display();
	}
	return false;
}

void Game::input(int& g) {

	int input;
	while (1) {
		display();
		//cout << "\n" << player;

		if (grid[X(g)][Y(g)].checkFull()) {
			g=pickNewGrid(g);
		}
		//cin.ignore();
		input = getFinalInput(g);

		cout << input;

		if (input < 10) {
			break;

		}

	}

	grid[X(g)][Y(g)].set(X(input), Y(input), player);

	if (grid[X(g)][Y(g)].winningCel(player)) {
		grid[X(g)][Y(g)].makeFull(player);
	}

	g = input;
}

bool Game::checkWin() {
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

