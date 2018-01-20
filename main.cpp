#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include "Game.hpp";

using namespace std;

int main(){
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
			game.play(); 
			break;
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









