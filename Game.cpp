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
		/*if (player = 'x') {
			g = player1.getInput();
		}
		if (player = 'o') {
			g = player2.getInput();
		}*/
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

int Game::pickNewGrid() {
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
	return 100;
}

std::vector<int> Game::getEmptyGrids() const {
	std::vector<int> emptyGrids;

	for (int i = 0; i < 9; i++) {
		//if (grid[i/3][i%3+1].checkFull()) {

		//}
	}
	return emptyGrids;
}

bool Game::gridFull(int i) {
	if (grid[X(i)][Y(i)].checkFull()) {
		return true;
	}
	else {
		return false;
	}
}

void Game::input(int& g) {

	int input;
	while (1) {
		display();
		cout << "\n" << player;

		if (grid[X(g)][Y(g)].checkFull()) {
			g=pickNewGrid();
		}

		input = getFinalInput(g);

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
	/*char p = player;
	int row = 1, col = 1, main_diag = 1, anti_diag = 1;
	for (size_t i = 0; i < 3; ++i)
	{
		row = col = 1;
		if (grid.get(i, 3 - 1 - i) != p) {
			anti_diag = 0;
		}
		if (grid.get(i, i) != p) {
			row = col = main_diag = 0;
		}

		else {
			for (size_t j = 0; j < 3; ++j)
			{
				if (grid.get(i, j) != p) {
					row = 0;
				}
				if (grid.get(j, i) != p) {
					col = 0;
				}
			}
		}

		if (row || col) {
			return 1;
		}
	}

	if (main_diag || anti_diag) {
		return 1;
	}
	return 0;*/
}

int main()
{
	Game game;
	game.display();
	cout << "\n  Welcome to Ultimate Tic Tac Toe." <<
		"\n  Press Enter to start.";
	cin.get();

	int input, error = 0;
	enum menu { play = 1, scores, quit };
	do {
		game.display();
		if (error) {
			cout << "  Invalid option. Try again.\n";
			error = 0;
		}
		else {
			cout << "  Select an option: \n";
		}
		cout << "    1) Play\n    2) Scores\n    3) Quit\n" << "\n> ";
		cin >> input;
		switch (input) {
		case play:
			game.play(); break;
		case scores:
			//showScores();
			break;
		case quit:
			std::exit(0);
		default:
			error = 1;
		}
		system("cls");
	} while (error);

	system("pause");
	return 0;
}

