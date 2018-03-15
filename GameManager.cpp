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


//trim the string
string ofxTrimString(string str) {

	return ofxTrimStringLeft(ofxTrimStringRight(str));;

}

//get the serial
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

	// sets it to the correct com and baudrate
	int baud = 9600;
	serial.setup("COM5", baud);
}

//cal the play fuction
void GameManager::play() {

	//grid and subgrid
	int grid, s;
	grid = 0;

	//play until someboday has won
	while (1) {
		if (currentBoard.checkWin(player)) {
			break;
		}

		//ask for input and change the player
		cur = grid;
		input(grid);
		player = player == 'x' ? 'o' : 'x';
	}
}

//get data from the arduino
void GameManager::getArduinoData() {
	string str;

	do {
		//read until end of line
		str = ofxGetSerialString(serial, '\n');

		if (str == "") continue;

		//of length of the string is 2 save the values in the variables
		if (str.length() == 2) {
			nextGrid = str[0] - 48;
			nextSubgrid = str[1] - 48;
		}

	} while (str != "");
}

void GameManager::checkIncommingData() {
	string str;

	int returnGrid = -1, playerReturn = -1;
	while (returnGrid == -1 && playerReturn == -1) {
		do {
			//read until end of line
			str = ofxGetSerialString(serial, '\n');

			if (str == "") continue;


			//of length of the string is 2 save the values in the variables
			if (str.length() == 2) {
				returnGrid = str[0] - 48;
				playerReturn = str[1] - 48;
			}

		} while (str != "");
	}

	std::cout << "gridCheckAI" << returnGrid << "vakje" << playerReturn << "\n";
}

//lets te current player pick a new grid
int GameManager::pickNewGrid(int& grid) {
	int input;

	//the human player
	if (player == 'x') {

		//gets the input
		input = nextGrid;

		//checks if its a legit move
		if (currentBoard.grid[X(input)][Y(input)].checkFull()) {
			return 0;
		}

		//returns the score
		else {
			return input;
		}
	}

	//if player AI-input is given by the ai player
	if (player == 'o') {
		input = inputs[0];
		//input = playerAI.getInput(grid, currentBoard);
		return input;
	}

	return false;
}

//get the subgrid input
int GameManager::getFinalInput(int& grid) {
	int input;

	//if player human, get new input
	if (player == 'x') {
		input = nextSubgrid;

		//sees if the set is legit
		if (input > 0 && input < 10)
		{
			if (currentBoard.grid[X(grid)][Y(grid)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
	}

	//player AI, get new input
	if (player == 'o') {
		input = inputs[1];
		//gets input from AI player
		//input = playerAI.getInput(grid, currentBoard);

		//checks if the move is legit
		if (input > 0 && input < 10)
		{
			if (currentBoard.grid[X(grid)][Y(grid)].get(X(input), Y(input)) == '.') {
				return input;
			}
		}
	}
	return false;
}

//gets input and cals get final input and picknewgrid
void GameManager::input(int& grid) {
	int input;

	//until otherwise stay in loop
	while (1) {
		if (player == 'o') {
			playerAI.getInput(grid, currentBoard, inputs);
		}
		//player human
		if (player == 'x') {

			//used to see if there is new data
			nextGrid = -1;
			nextSubgrid = -1;

			//untill there is new data keep reveiving
			while (nextGrid == -1 && nextSubgrid == -1) {
				getArduinoData();
			}
			std::cout << "subgridHum" << nextGrid << "light" << nextSubgrid << "\n";
		}


		//lets current player pick new grid
		if (currentBoard.grid[X(grid)][Y(grid)].checkFull() || grid == 0) {
			grid = pickNewGrid(grid);
		}

		//gets the input
		input = getFinalInput(grid);

		//player ai
		if (player == 'o') {

			checkIncommingData();

			unsigned char buf[2] = { grid + 48, input + 48 };
			serial.writeBytes(&buf[0], 2);

			std::cout << "gridSendAI" << grid << "input" << input << "\n";

			checkIncommingData();
		}

		//checks if the set is legit
		if (currentBoard.grid[X(grid)][Y(grid)].get(X(input), Y(input)) != '.') {
			continue;
		}

		if (input < 10) {
			break;
		}
	}

	//sets the input
	currentBoard.grid[X(grid)][Y(grid)].set(X(input), Y(input), player);

	int playerInt;

	if (player == 'x') {
		playerInt = 1;
	}
	else {
		playerInt = 2;
	}

	//checks if there is a winner
	if (currentBoard.grid[X(grid)][Y(grid)].winningCel(player)) {
		currentBoard.grid[X(grid)][Y(grid)].makeFull(player);

		unsigned char full[2] = { grid + 48, playerInt + 48 };
		serial.writeBytes(&full[0], 2);
		std::cout << "gridISFULL" << grid << "player" << playerInt << "\n";
	}
	else {
		unsigned char full[2] = { 0 + 48, playerInt + 48 };
		serial.writeBytes(&full[0], 2);

		std::cout << "gridNotFull" << 0 << "player" << playerInt << "\n";
	}

	//the last in put should be the new grid
	grid = input;
}

