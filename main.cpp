#include <iostream>
#include "board.h"
#include "test.h"


bool tests() {
	return (
		test_init() &&
		test_place_valid_card() &&
		test_place_smaller_card() &&
		test_place_card_out_of_bounds() &&
		test_place_card_non_adjacent() &&
		test_check_winner() &&
		test_check_row() &&
		test_complete_round()
	);
}


int main() {
	if (!tests())
		std::cout << "Tests failed!\n";
	else
		std::cout << "Tests successful!\n";
	return 0;
}

