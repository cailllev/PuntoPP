#ifndef PLAYER_H
#define PLAYER_H

#include <list>
#include "card.h"

class Player {
    private:
        int player_id;
        std::string player_name;
        std::list<Card> cards;

    public:
        Player(int id, std::string name);

        int get_id();
        std::string get_name();
        Card* get_next_card();
        Card* peek_next_card();
        Card* get_avg_draw();
        std::list<Card> get_remaining_cards();
        int get_cards_count();
};

#endif
