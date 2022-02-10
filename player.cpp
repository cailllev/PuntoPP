#include "player.h"
#include "console.h"
#include <random>


Player::Player(int id, std::string name) {
    this->highest_card_value = Card::max_val;
    this->player_id = id;
    this->player_name = name;

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
    
    int max = 0;
    for (auto& card : this->cards) {
        if (card->get_value() > max) {
            max = card->get_value();
        }
    }
    this->highest_card_value = max;

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

int Player::get_highest_draw() {
    return this->highest_card_value;
}

std::array<int, Card::max_val+1> Player::get_remaining_cards() {
    std::array<int, Card::max_val+1> distribution = std::array<int, Card::max_val+1>();
    for (auto& card : this->cards) {
        distribution[card->get_value()] += 1;
    }
    return distribution;
}

int Player::get_cards_count() { 
    return this->cards.size();
}

std::string Player::print() {
    return to_colour[this->player_id](this->player_name);
}
