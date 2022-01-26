#include "player.h"
#include "console.h"
#include <iostream>
#include <random>


Player::Player(int id, std::string name) {
    player_id = id;
    player_name = name;

    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());

    // create a shuffled list of cards (1, 1, 2, 2, 3, 3, ...)
    for (int j = 0; j < 2; j ++) {
        for (int i = Card::min_val; i <= Card::max_val; i++) {
            // create random position for new card
            std::uniform_int_distribution<int> distr(0, i + j*Card::max_val + 1);
            int pos = distr(generator);

            // create and advance iterator to position
            std::list<Card*>::iterator it = this->cards.begin();
            advance(it, pos);

            this->cards.insert(it, new Card(this->player_id, i));
        }
    }
}

Player::~Player() {
    this->cards.clear(); // delete cards
}

int Player::get_id() { 
    return this->player_id; 
}

std::string Player::get_name() { 
    return this->player_name; 
}

Card * Player::peek_next_card() {
    return this->cards.front(); 
}

Card * Player::get_next_card() {
    Card *c = this->cards.front();
    this->cards.pop_front();
    return c;
}

// returns average card drawn
// TODO, implement top 66% / top x% draw
Card * Player::get_avg_draw() {
    int sum = 0;
    for (auto& card : this->cards) {
        sum += card->get_value();
    }
    return new Card(this->player_id, (int) (sum / this->cards.size()));
}

int Player::get_cards_count() { 
    return this->cards.size();
}

std::string Player::print() {
    return to_colour[this->player_id](this->player_name);
}
