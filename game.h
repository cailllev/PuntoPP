#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include <list>
#include <string>


class Game {
	private:
		Board *board;
		std::list<Player*> players;

	public:
		Game(std::list<std::string> names);
		~Game();

		void play_round();
		Board * get_board();
		std::list<Player*> get_players();
		void advance_round();
		void previous_round();
		std::string print();
};

// forward declaration of maximax from heuristics
Point * maximax(Game g, Card *c);

#endif
