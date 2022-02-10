#include "test.h"
#include "heuristics.h"
#include "board.h"
#include "card.h"
#include <iostream>

bool test_init() {
	Board *b = new Board();
	return true;
}

bool test_place_valid_card() {
	return false;
}

bool test_place_smaller_card() {
	return false;
}
bool test_place_card_out_of_bounds() {
	return false;
}

bool test_place_card_non_adjacent() {
	return false;
}

// TODO all 4 directions
bool test_check_winner() {
	std::cout << "[*] test_check_row\n";
	bool ret = true;

	int player_id = 1;
	Board *b = new Board();
	Card *c1 = new Card(player_id, 1);
	Card *c2 = new Card(player_id, 2);
	Card *c3 = new Card(player_id, 3);

	b->play_card_no_checks(c1, 5, 5); // first card
	b->play_card(c2, 6, 4);
	b->play_card(c2, 7, 3);
	bool winner = b->check_winner(player_id, 7, 3);
	std::cout << b->print();
	std::cout << "  [#] winner: " + std::to_string(winner) + "\n\n";
	ret = ret && !winner; // not won yet

	b->play_card(c2, 8, 2);

	std::cout << b->print();
	winner = b->check_winner(player_id, 7, 3);
	ret = ret && winner; // now winner
	std::cout << "  [#] winner: " + std::to_string(winner) + "\n\n";
	return ret;
}

bool test_check_row() {
	return false;
}

bool test_complete_round() {
	return false;
}

// TODO
bool test_get_lines_from_pos() {
	std::cout << "[*] test_get_lines_from_pos\n";
	bool ret = true;
	Board *b = new Board();
	Card *c1 = new Card(1, 1);
	Card *c2 = new Card(1, 2);

	b->play_card_no_checks(c1, 5, 5); // first card
	b->play_card(c2, 5, 4);

	return false;
}

bool test_get_best_spot() {
	std::cout << "[*] test_get_best_spot\n";
	bool ret = true;

	int pid = 1;
	Player *p = new Player(pid, "dummy");
	Board *b = new Board();
	Card *c1 = new Card(pid, 1);
	Card *c2 = new Card(pid, 2);
	Card *c3 = new Card(pid, 3);
	Card *c4 = new Card(pid, 4);
	Card *c5 = new Card(pid, 5);

	b->play_card_no_checks(c1, 5, 5); // first card
	b->play_card(c2, 5, 4);
	b->play_card(c3, 6, 3);
	b->play_card(c4, 5, 2);

	// test top 4
	std::list<ScorePos*> scores = get_best_spots(b, b->get_borders(), c5, 4, 9);
	ret = ret && (scores.size() == 4);

	std::cout << "  [#] Board:\n" + b->print();
	for (auto& s : scores) {
		std::cout << "  [#] " + std::to_string(s->x) + " " 
			+ std::to_string(s->y) + ": " + std::to_string(s->score) + "\n";
	}
	std::cout << "\n\n";

	ScorePos* best = scores.front();
	ret = ret && (best->x == 5 && best->y == 3);

	// test not more top spots than actual playing spots (24)
	scores = get_best_spots(b, b->get_borders(), c5, 24, 9);
	std::cout << "Here\n";
	ret = ret && (scores.size() == 24);
	scores = get_best_spots(b, b->get_borders(), c5, 25, 9);
	std::cout << "Here\n";
	ret = ret && (scores.size() == 24);

	return ret;
}

bool test_maximax() {
	return false;
}

bool tests() {
	test_get_best_spot();
	return (
		test_init() &&
		test_place_valid_card() &&
		test_place_smaller_card() &&
		test_place_card_out_of_bounds() &&
		test_place_card_non_adjacent() &&
		test_check_winner() &&
		test_check_row() &&
		test_complete_round() &&
		test_get_lines_from_pos() &&
		test_get_best_spot() &&
		test_maximax()
	);
}
