#ifndef GAME_H
#define GAME_H
#include "board.h"

class Game {
	public:
		Game(std::list<std::string> names);
		void play_round();
		Board * get_board();
		std::list<Player*> get_players();
		void advance_round();
		void previous_round();
		std::string print();

	private:
		Board *board;
		std::list<Player*> players;
};

#endif
