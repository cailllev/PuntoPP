#include <iostream>
#include "game.h"
#include "board.h"
//#include "heuristics.h"

Game::Game(std::list<std::string> names) {
	std::cout << "here\n";
	this->board = new Board();
	this->players = std::list<Player*>();
	int id = 1; // id 0 is null_player
	for (auto& name : names) {
		this->players.push_back(new Player(id++, name));
	}
}

void Game::play_round() {
	std::cout << this->print();
}

Board * Game::get_board() {
	return this->board;
}

std::list<Player*> Game::get_players() {
	return this->players;
}

void Game::advance_round() {
	Player *p = this->players.front();
	this->players.pop_front();
	this->players.push_back(p);
}

void Game::previous_round() {
	Player *p = this->players.back();
	this->players.pop_back();
	this->players.push_front(p);
}

std::string Game::print() {
	std::string s = this->board->print();
	s += "[*] Current Player: " + this->players.front()->print() + ".\n";
	return s;
}