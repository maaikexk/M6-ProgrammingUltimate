#include "Board.hpp"
#include "GameManager.hpp"
#include "AIPlayer.hpp"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>


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

// trim trailing spaces

string ofxTrimStringRight(string str) {

	size_t endpos = str.find_last_not_of(" \t\r\n");

	return (string::npos != endpos) ? str.substr(0, endpos + 1) : str;

}



// trim trailing spaces

string ofxTrimStringLeft(string str) {

	size_t startpos = str.find_first_not_of(" \t\r\n");

	return (string::npos != startpos) ? str.substr(startpos) : str;

}



string ofxTrimString(string str) {

	return ofxTrimStringLeft(ofxTrimStringRight(str));;

}

string ofxGetSerialString(ofSerial &serial, char until) {

	static string str;
	stringstream ss;

	char ch;
	int ttl = 1000;

	while ((ch = serial.readByte()) > 0 && ttl > 0 && ch != until) {
		ss << ch;
	}

	str += ss.str();

	if (ch == until) {
		string tmp = str;
		str = "";
		return ofxTrimString(tmp);
	}
	else {
		return "";
	}

}

//the constructor
GameManager::GameManager() {

	//for each big grid[i][j]
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {

			//and each supgrid[k][l]
			for (size_t k = 0; k < 3; ++k) {
				for (size_t l = 0; l < 3; ++l) {

					//set the subgrid to empty
					currentBoard.grid[i][j].set(k, l, '.');
				}
			}
		}
	}

	//the start player
	player = 'x';

	//the players playing
	playerAI = AIPlayer();
	cur = 0;

	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	int baud = 9600;
	serial.setup("COM5", baud);

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

		cur = grid;
		input(grid);
		player = player == 'x' ? 'o' : 'x';
	}
}

void GameManager::getArduinoData() {
	string str;

	do {
		str = ofxGetSerialString(serial, '\n'); //read until end of line

		if (str == "") continue;

		if (str.length() == 2) {
			nextGrid = str[0] - 48;
			nextSubgrid = str[1] - 48;
		}

	} while (str != "");
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

		input = nextGrid;
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
		input = nextSubgrid;

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

		if (player == 'x') {
			nextGrid = -1;
			nextSubgrid = -1;

			while (nextGrid == -1 && nextSubgrid == -1) {
				getArduinoData();
			}

			std::cout << "subgrid" << nextGrid << "light" << nextSubgrid << "\n";
		}

		if (currentBoard.grid[X(grid)][Y(grid)].checkFull()) {
			grid = pickNewGrid(grid);
		}

		input = getFinalInput(grid);

		if (player == 'o') {
			unsigned char buf[2] = { grid + 48, input + 48 };
			serial.writeBytes(&buf[0], 2);

			std::cout << "grid" << grid << "input" << input << "\n";
		}

		if (currentBoard.grid[X(grid)][Y(grid)].get(X(input), Y(input)) != '.') {
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

	/*int test = 0;
	if (currentBoard.grid[X(grid)][Y(grid)].winningCel(player)) {
		int playerNum;
		if (player == 'x') {
			playerNum = 1;
		}
		else {
			playerNum = 2;
		}
		unsigned char buf[2] = { grid + 48, playerNum + 48 };
		serial.writeBytes(&buf[0], 2);

		currentBoard.grid[X(grid)][Y(grid)].makeFull(player);

	}
	else {
		int playerNum;
		if (player == 'x') {
			playerNum = 1;
		}
		else {
			playerNum = 2;
		}
		unsigned char buf[2] = { 0 + 48, playerNum + 48 };
		serial.writeBytes(&buf[0], 2);

		std::cout << "grid" << 0 << "light" << playerNum << "\n";
	}
	
	//Sleep(10000);

	string str;

	do {
		str = ofxGetSerialString(serial, '\n'); //read until end of line

		if (str == "") continue;

		if (str.length() == 1) {
			test = str[0] - 48;
		}

	} while (str != "");
	*/

	grid = input;
}

