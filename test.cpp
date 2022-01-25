#include <iostream>
#include "test.h"
#include "heuristics.h"
#include "board.h"
#include "card.h"

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

bool test_check_winner() {
	return false;
}

bool test_check_row() {
	return false;
}

bool test_complete_round() {
	return false;
}

bool test_get_lines_from_pos() {
	return false;
}

bool test_get_best_spot() {
	Board *b = new Board();
	Card *c1 = new Card(1, 1);
	Card *c2 = new Card(1, 2);
	Card *c3 = new Card(1, 3);
	Card *c4 = new Card(1, 4);
	Card *c5 = new Card(1, 5);

	b->play_card_no_checks(c1, 5, 5); // first card
	b->play_card(c2, 5, 4);
	b->play_card(c3, 6, 3);
	b->play_card(c4, 5, 2);

	// board:
	//   2 3 4 5
	// 4 . . . .
	// 5 x . x x
	// 6 . x . .

	std::list<ScorePos*> scores = get_best_spots(b, c5, 3, b->get_borders());
	ScorePos* best = scores.front();
	return best->x == 5 && best->y == 3;
}

bool test_maximax() {
	return false;
}


bool tests() {
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
