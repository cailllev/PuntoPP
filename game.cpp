#include "game.h"
#include "console.h"
#include <iostream>
#include <cstring>
#include <chrono>
#include <thread>

Game::Game(std::list<std::string> names) {
	this->board = new Board();
	this->players = std::list<Player*>();
	int id = 1; // id 0 is null_player
	for (auto& name : names) {
		this->players.push_back(new Player(id++, name));
	}
}

Game::~Game() {
	players.clear(); // delete players
}

void Game::play_round() {

	// first round
	Player *first_p = this->players.front();
	std::cout << this->print();
	std::cout << "[*] " + first_p->print() + ", press Enter to place " + first_p->peek_next_card()->print();
	std::getchar();
	Point *mid = this->board->get_mid();
	this->board->play_card_no_checks(
		first_p->get_next_card(), mid->x, mid->y
	);
	delete mid;
	this->advance_round();

	Player *p;
	bool done = false;
	while (!done) {
		std::cout << this->print();
		
		p = this->players.front();
		int x, y;
		
		if (p->get_name().compare("AI") == 0) {
			std::cout << "[*] " + p->print() + " calculating next move...\n";
			Card *c = p->get_next_card();
			Point *pt = maximax((*this), c);
			x = pt->x;
			y = pt->y;
			this->board->play_card(c, x, y);
			std::cout << this->print();
			std::cout << "[*] AI played " + c->print() + "\n";
			delete pt;
			std::this_thread::sleep_for(std::chrono::seconds(2));
		
		} else {
			Card *c = p->get_next_card();

			while (true) {
				std::cout << "[*] " + p->print() + ", enter the position to play " + c->print() + ": ";
				std::string input = "";
				std::getline(std::cin, input);
				
				std::string xs, ys;
				bool first_num = true;
				for (auto& c : input) {
					if (isspace(c)) {
						first_num = false;
						continue;
					}
					if (first_num) {
						xs.push_back(c);
					} else {
						ys.push_back(c);
					}
				}
				try {
					x = std::stoi(xs);
					y = std::stoi(ys);
				} catch (const std::invalid_argument &ex) {
					std::cout << "[!] Invalid position, enter it like this: '5 5'\n";
					continue;
				}

				if (this->board->play_card(c, x, y)) {
					break;
				}
				std::cout << "[!] Invalid play, try again.\n";
			}
		}

		done = this->board->check_winner(p->get_id(), x, y);
		this->advance_round();
	}
	std::cout << this->print();
	std::cout << "[*] Congrats " + p->print() + ", you won!\n";
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
	return clear_screen() + this->board->print();
}