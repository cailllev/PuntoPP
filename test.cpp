#include "test.h"
#include "board.h"

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

