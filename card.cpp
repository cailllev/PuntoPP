#include <array>
#include "console.h"
#include "card.h"

Card::Card(int p_id, int val) : player_id(p_id), value(val) {};

int Card::get_value() {
	return this->value;
}

int Card::get_player_id() {
	return this->player_id;
}

std::string Card::print() {
    return to_colour[this->player_id](std::to_string(this->value));
}
