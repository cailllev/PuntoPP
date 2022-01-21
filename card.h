#ifndef CARD_H
#define CARD_H
#include <string>


class Card {
    private:
        int player_id;
        int value;

    public:
        static int const min_val = 1;
        static int const max_val = 9;

        Card(int player_id, int value);
        int get_value();
        int get_player_id();
        std::string print();
};

#endif
