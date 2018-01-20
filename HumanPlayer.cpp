#include "HumanPlayer.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

int HumanPlayer::getInput() {
	int g;
	std::cin >> g;
	return g;
}

int HumanPlayer::getNewGrid() {
	int g;
	std::cin >> g;
	return g;
}