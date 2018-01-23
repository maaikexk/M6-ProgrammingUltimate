#pragma once

#include "Grid.hpp"
#include "HumanPlayer.hpp"
#include "AIPlayer.hpp"
#include <vector>

class Board{
private:
	int Gridsize = 3;

public:
	//Game();
	Grid grid[3][3];
	//void display();
	//void play();
	//void input(int& g);
	//int pickNewGrid(int& g);
	//int getFinalInput(int& g);
	bool checkWin(char);
	bool isFull();
	int heuristicScoreBoard(char, char);
	int check(int, int);
	int oneCurrent, twoCurrent, oneOther, twoOther;
	//char player;
	//size_t cur;
	//AIPlayer playerAI;
	//HumanPlayer playerHuman;
	//std::vector<int> getEmptyGrids() const;
	//bool gridFull(int);
	//void showScore();
};