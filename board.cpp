#include <string>
#include <list>
#include "board.h"
#include "card.h"
#include "console.h"

Card * const Board::null_card = new Card(0, 0);
Player * const Board::null_player = new Player(0, "");

Board::Board() {

    // init all spots with null cards
    this->spots[max_y][max_x] = {};
    for (int y = 0; y < this->max_y; y++) {
        for (int x = 0; x < this->max_x; x++) {
            this->spots[y][x] = Board::null_card;
        }
    }

    // init boarders
    this->ob_x_min = 0;
    this->ob_x_max = this->max_x;
    this->ob_y_min = 0;
    this->ob_y_max = this->max_y;
    this->ib_x_min = this->x_mid - 1;
    this->ib_x_max = this->x_mid - 1;
    this->ib_y_min = this->y_mid + 1;
    this->ib_y_max = this->y_mid + 1;
}

Card * Board::get_card(int x, int y) {
    return this->spots[y][x];
}

std::list<Point*> Board::get_valid_plays(Card *c) {
    std::list<Point*> plays = std::list<Point*>();
    for (int y = this->ib_y_min; y <= this->ib_y_max; y++) {
        for (int x = this->ib_x_min; x <= this->ib_x_max; x++) {
            Point *p = new Point(x, y);
            if (Board::is_valid_play(c, x, y))
                plays.push_back(p);
        }
    }
    return plays;
}

bool Board::is_valid_play(Card *c, int x, int y) {

    // new card must be bigger then old card
    if (c->get_value() <= (this->spots[y][x])->get_value())
        return false;

    // new card must be in inner border
    if (x < this->ib_x_min || x > this->ib_x_max || y < this->ib_y_min || y < this->ib_y_max)
        return false;

    // check if any cards are adjacent
    int x_minus = 0 ? x == 0 : 1;
    int x_plus  = 0 ? x == this->max_x : 1;
    int y_minus = 0 ? y == 0 : 1;
    int y_plus  = 0 ? y == this->max_y : 1;
    bool any_neighbour = false;
    for (int yi = y - y_minus; yi <= y + y_plus; yi++) {
        for (int xi = x - x_minus; xi <= x + x_plus; xi++) {
            if ((this->spots[yi][xi])->get_value() != 0) {
                return true;
            }
        }
    }
    return false;
}

// current sizes (11 and 6) let the "outer border" automatically shrink 
// when card is played on the "inner border" and expand "inner border" when placed on it
void Board::update_borders(int x, int y) {
    if      (x == this->ib_x_min) {this->ob_x_max = x - 1 + this->max_size; this->ib_x_min -= 1;}
    else if (x == this->ib_x_max) {this->ob_x_min = x + 1 - this->max_size; this->ib_x_max += 1;}
    if      (y == this->ib_y_min) {this->ob_y_max = y - 1 + this->max_size; this->ib_y_min -= 1;}
    else if (y == this->ib_y_max) {this->ob_y_min = y + 1 - this->max_size; this->ib_y_max += 1;}
}

// checks if card to play is valid, then places it and updates borders
bool Board::play_card(Card *c, int x, int y) {
    if (Board::is_valid_play(c, x, y)) {
        Board::play_card_no_checks(c, x, y);
        return true;
    }
    return false;
}

// places a card and updates borders
void Board::play_card_no_checks(Card *c, int x, int y) {
    this->played_cards += 1;
    this->spots[y][x] = c;
    Board::update_borders(x, y);
}

void Board::set_card(Card *c, int x, int y) {
    this->spots[y][x] = c;
}

// traverse row (from xs, ys to xe, ye with deltas dx and dy per step), returns true if connected 4
bool Board::check_row(int player_id, int x, int y, int xs, int ys, int xe, int ye, int dx, int dy) {
    int streak = 0;
    for (int xi = xs, yi = ys; xi <= xe && yi <= ye; yi += dy, xi += dx) {
        (this->spots[yi][xi])->get_player_id() == player_id ? streak += 1 : streak = 0;
        if (streak == 4)
            return true;
    }
    return false;
}

// traverse all possible rows from given spot, returns true if player_id connected 4
bool Board::check_winner(int player_id, int x, int y) {
    int x_minus = std::min(x, 3);
    int y_minus = std::min(y, 3);
    int x_plus = std::min(this->max_x - 1 - x, 3);
    int y_plus = std::min(this->max_y - 1 - y, 3);
    int xs, ys, xe, ye;

    // top to bottom
    xs = x;
    ys = y - y_minus;
    xe = x;
    ye = y + y_plus;
    if (Board::check_row(player_id, x, y, xs, ys, xe, ye, 0, 1))
        return true;

    // top left to bottom right
    xs = x - x_minus;
    ys = y - y_minus;
    xe = x + x_plus;
    ye = y + y_plus;
    if (Board::check_row(player_id, x, y, xs, ys, xe, ye, 1, 1))
        return true;

    // left to right
    xs = x - x_minus;
    ys = y;
    xe = x + x_plus;
    ye = y;
    if (Board::check_row(player_id, x, y, xs, ys, xe, ye, 1, 0))
        return true;

    // bottom left to top right
    xs = x - x_minus;
    ys = y + y_plus;
    xe = x + x_plus;
    ye = y - y_minus;
    if (Board::check_row(player_id, x, y, xs, ys, xe, ye, 1, -1))
        return true;

    return false;
}

std::string Board::print() {
    std::string s = "    ";
    std::string pad = "   ";

    // indices above board, mark indices outside border as black
    for (int xi = 0; xi < Board::max_x; xi++) {
        if (xi >= this->ob_x_min && xi <= this->ob_x_max) {
            s += white(std::to_string(xi));
        } else {
            s += black(std::to_string(xi));
        }
        s += pad;
    }
    s += "\n\n";

    for (int y = 0; y < Board::max_y; y++) {

        // indices next to board
        if (y >= this->ob_x_min && y <= this->ob_x_max) {
            s += white(std::to_string(y));
        } else {
            s += black(std::to_string(y));
        }

        // padding between index and board
        int index_length = std::to_string(y).length();
        for (int p = 0; p < 4 - index_length; p++) {
            s += " ";
        }

        // cards in each row
        for (int x = 0; x < Board::max_x; x++) {
            s += (this->spots[y][x])->print();
            s += pad;
        }
        s += "\n";
    }
    return s;
}
