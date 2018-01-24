#include "HumanPlayer.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>

//human player used to play the program on a computer, not used in the instalation

//using namespace std to read input
using namespace std;

//get a input
int HumanPlayer::getInput() {
	int g;
	std::cin >> g;
	return g;
}

//get a new grid
int HumanPlayer::getNewGrid() {
	int g;
	std::cin >> g;
	return g;
}