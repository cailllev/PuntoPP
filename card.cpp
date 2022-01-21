#include <array>
#include <functional>
#include "console.h"
#include "card.h"

// null cards are black (player_id 0 => index 0)
std::array<std::function<std::string(std::string)>, 5> to_colour = {
    &black, &red, &green, &blue, &yellow
};

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
