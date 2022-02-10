#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <array>
#include <list>
#include <string>

class Player {
    private:
        int player_id;
        int highest_card_value;
        std::string player_name;
        std::list<Card*> cards;

    public:
        Player(int id, std::string name);
        ~Player();
        
        int get_id();
        std::string get_name();
        Card * get_next_card();
        Card * peek_next_card();
        Card * get_avg_draw();
        int get_highest_draw();
        std::array<int, Card::max_val+1> get_remaining_cards();
        int get_cards_count();
        std::string print();
};

#endif
