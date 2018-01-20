#include "Grid.hpp"
#include <cassert>

void Grid::set(int x, int y, char cell) {
	subgrid[x][y] = cell;
}

char Grid::get(int x, int y) {
	return subgrid[x][y];
}

bool Grid::checkFull() {
	for (int x = 0; x < Gridsize; x++) {
		for (int y = 0; y < Gridsize; y++) {
			if (subgrid[x][y] == '.') {
				return false;
			}
		}
	}
	return true;
}

bool Grid::winningCel(char cell) {
	for (int y = 0; y < Gridsize; y++) {
		if (subgrid[0][y] == cell && subgrid[1][y] == cell && subgrid[2][y] == cell) {
			makeFull(cell);
			return true;
		}
	}

	for (int x = 0; x < Gridsize; x++) {
		if (subgrid[x][0] == cell && subgrid[x][1] == cell && subgrid[x][2] == cell) {
			makeFull(cell);
			return true;
		}
	}

	// check diagonal
	if (subgrid[0][0] == cell && subgrid[1][1] == cell && subgrid[2][2] == cell) {
		makeFull(cell);
		return true;
	}
	if (subgrid[0][2] == cell && subgrid[1][1] == cell && subgrid[2][0] == cell) {
		makeFull(cell);
		return true;
	}
	return false;
}

void Grid::makeFull(char cell) {
	for (int i = 0; i < Gridsize; i++) {
		for (int j = 0; j < Gridsize; j++) {
			subgrid[i][j] = cell;
		}
	}
}

std::vector<int> Grid::getEmptyPositions() const {
	std::vector<int> empty_positions;

	for (int i = 0; i < Gridsize*Gridsize; i++) {
		if (subgrid[i / 3][i % 3 + 1] == '.') {
			empty_positions.push_back(i);
		}
	}
	return empty_positions;
}