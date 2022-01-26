#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include <list>
#include <string>

class Player {
    private:
        int player_id;
        std::string player_name;
        std::list<Card*> cards;

    public:
        Player(int id, std::string name);
        ~Player();
        
        int get_id();
        std::string get_name();
        Card* get_next_card();
        Card* peek_next_card();
        Card* get_avg_draw();
        int get_cards_count();
        std::string print();
};

#endif
