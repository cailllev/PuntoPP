#include <string>
#include "player.h"
#include "card.h"


Player::Player(int id, std::string name) {
    player_id = id;
    player_name = name;

    // create array of cards (1, 1, 2, 2, 3, 3, ...)
    for (int i = Card::min_val; i <= Card::max_val; i += 1) {
        this->cards.push_back(Card(this->player_id, i));
        this->cards.push_back(Card(this->player_id, i));
    }
}

int Player::get_id() { return this->player_id; }
std::string Player::get_name() { return this->player_name; }
Card* Player::peek_next_card() { return &(this->cards.front()); }

Card* Player::get_next_card() {
    Card* c = &(this->cards.front());
    this->cards.pop_front();
    return c;
}

Card* Player::get_avg_draw() {
            int idx = this->cards.size();
            std::list<Card>::iterator it = this->cards.begin();
            std:advance(it, idx);
            return &(*it);
}

// std::list<Card> Player::get_remaining_cards() {}
int Player::get_cards_count() { return this->cards.size(); }

