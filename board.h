#ifndef BOARD_H
#define BOARD_H
#include <array>
#include "card.h"
#include "player.h"


struct Point{
    int x, y;
    Point(int x_, int y_) : x(x_), y(y_) {};
};

struct Borders {
    int ob_x_min, ob_x_max, ob_y_min, ob_y_max; // outer borders
    int ib_x_min, ib_x_max, ib_y_min, ib_y_max; // inner borders
    Borders(
        int ob_x_min_, int ob_x_max_, int ob_y_min_, int ob_y_max_, 
        int ib_x_min_, int ib_x_max_, int ib_y_min_, int ib_y_max_) : 
            ob_x_min(ob_x_min_), ob_x_max(ob_x_max_), ob_y_min(ob_y_min_), ob_y_max(ob_y_max_),
            ib_x_min(ib_x_min_), ib_x_max(ib_x_max_), ib_y_min(ib_y_min_), ib_y_max(ib_y_max_) {};
};


class Board {
    public:
        static const int max_x = 11;
        static const int max_y = 11;
        static const int max_size = 6;
        static const int x_mid = (max_x - 1) / 2;
        static const int y_mid = (max_y - 1) / 2;
        static Card * const null_card;     // const pointer
        static Player * const null_player; // const pointer

        Board();
        Card* get_card(int x, int y);
        std::list<Point*> get_valid_plays(Card *c);
        bool is_valid_play(Card *c, int x, int y);
        Borders* get_borders();
        void update_borders(int x, int y);
        bool play_card(Card *c, int x, int y);
        void play_card_no_checks(Card *c, int x, int y);
        void set_card(Card *c, int x, int y);
        bool check_row(int player_id, int x, int y, int xs, int ys, int xe, int ye, int dx, int dy);
        bool check_winner(int player_id, int x, int y);
        std::string print();

    private:
        Card *spots[Board::max_y][Board::max_x];
        int played_cards;

        // borders
        int ob_x_min;
        int ob_x_max;
        int ob_y_min;
        int ob_y_max;
        int ib_x_min;
        int ib_x_max;
        int ib_y_min;
        int ib_y_max;
};

#endif
