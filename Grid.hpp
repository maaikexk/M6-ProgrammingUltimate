#pragma once

#include <vector>

class Grid
{
private:
	int Gridsize = 3;
	char subgrid[3][3];
	char left, right;

public:
	Grid() {}
	void set(int x, int y, char cell);
	char get(int x, int y);
	//bool isFull;
	bool checkFull();
	bool won;
	bool winningCel(char);
	void makeFull(char);
	std::vector<int> getEmptyPositions() const;
//+	int possibleMoves[];
};

