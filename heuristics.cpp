#include "heuristics.h"
#include <limits>


// TODO: refine coefs
const int H_WIN = 10000;
const int H_IN_ROW = 10;

const int H_OVERLAY_OPPONENT = 3;
const int H_OVERLAY_OWN = -1;

const int H_OWN_SUM = 1;
const int H_OPPONENT_SUM = 2;

const int H_OUT_OF_SOFT_BOUNDS = -100;
const int H_UNWINNABLE_LINE = -100;
const int H_INVALID_PLAY = -1000;
const int H_LOOSE = -H_WIN;

const int DIRS[4][2]  = {
    {+1, 0},   // top to bottom
    {+1, +1},  // top left to bottom right
    {0, +1},   // left to right
    {+1, -1},  // top right to bottom left
};

const int TOP_PLAYS_FIRST_ROUND = 8; // how many top plays to inspect for first round
const int TOP_N_PLAYS = 2;           // how many top plays to inspect for following rounds
const int ROUNDS_FORWARD = 2;        // how many rounds forward for maximax


std::list<std::list<CardPos*>> get_lines_from_pos(Board *b, int x, int y, Borders *bo) {
    std::list<std::list<CardPos*>> lines = std::list<std::list<CardPos*>>();
    
    for (int d = 0; d < 4; d++) {
        int dy = DIRS[d][0];
        int dx = DIRS[d][1];

        // get each line of 4 cards in a given direction, i.e.:
        //  x x x x . . .;    . x x x x . .;    . . x x x x .;    . . . x x x x
        // -3-2-1 0 1 2 3    -3-2-1 0 1 2 3    -3-2-1 0 1 2 3    -3-2-1 0 1 2 3
        for (int s = -3; s <= 0; s++) {
            std::list<CardPos*> line = std::list<CardPos*>();
            for (int i = 0; i < 4; i++) {
                int xi = x + (s + i) * dx;
                int yi = y + (s + i) * dy;
                if (xi >= bo->ob_x_min && xi <= bo->ob_x_max && 
                    yi >= bo->ob_y_min && yi <= bo->ob_y_max) {
                    line.push_back(new CardPos(xi, yi, b->get_card(xi, yi)));
                }
            }
            // add line to lines if all 4 cards are in outer borders
            if (line.size() == 4) {
                lines.push_back(line);
            }
        }
    }
    return lines;
}

bool sorted_insert(std::list<ScorePos*> *l, ScorePos *s, int count_spots) {
    count_spots += 1; // do not count dummy node

    bool inserted = false;
    int i = 0; 
    std::list<ScorePos*>::iterator it = l->begin();

    // advance pointer until at end of list; or new score > score at iterator; or score < top x scores
    while (it != l->end() && s->score <= (*it)->score && i < count_spots) {
        it++; i++;
    };

    // insert new score if in top count_spots
    if (i < count_spots) {
        l->insert(it, s);
        inserted = true;
    }

    // remove last item if list is now too big
    if (l->size() > count_spots) {
        l->pop_back();
    }
    return inserted;
}

std::list<ScorePos*> get_best_spots(Board *b, Borders *bo, Card *c, int count_spots, int own_highest) {

    int x_min = bo->ib_x_min;
    int x_max = bo->ib_x_max;
    int y_min = bo->ib_y_min;
    int y_max = bo->ib_y_max;

    int x_diff = x_max - x_min;
    int y_diff = y_max - y_min;

    int player_id = c->get_player_id();

    // create scores and in_row arrays (but only for playable board)
    int scores[y_diff + 1][x_diff + 1] = {};

    // iterate only playable board, xi & yi -> coords on shrinked, playable board
    for (int yi = 0; yi <= y_diff; yi++) {
        int y = yi + y_min; // actual y coord on actual board
        for (int xi = 0; xi <= x_diff; xi++) {
            int x = xi + x_min; // actual x coord actual on board

            // mark invalid plays and continue
            if (!b->is_valid_play(c, x, y)) {
                scores[yi][xi] = H_INVALID_PLAY;
                continue;
            }

            // temporarily set new card to calc heuristics
            Card *original_card = b->get_card(x, y);
            b->set_card(c, x, y);

            // test for overlay bonus, the higher the other card, the better
            int overlay_bonus;
            if (original_card->get_player_id() != player_id) {
                overlay_bonus = original_card->get_value() * H_OVERLAY_OPPONENT;
            } else {
                overlay_bonus = original_card->get_value() * H_OVERLAY_OWN;
            }

            std::list<std::list<CardPos*>> lines = get_lines_from_pos(b, x, y, bo);
            for(auto& line : lines) {

                // heuristics of spot when placing card in current row
                // all cards in current row are within outer borders at the moment
                int cards_in_line = 0; // count of own cards in line
                int own_sum = 0;       // sum of own cards in line
                int opponent_sum = 0;  // sum of other cards in line
                bool out_of_soft_bounds = false;

                int max_card_opponent = 0;

                for (auto& cardpos : line) {
                    Card *card = cardpos->c;
                    int xc = cardpos->x;
                    int yc = cardpos->y;
                    int card_val = card->get_value();

                    if (card->get_player_id() == player_id) {
                        cards_in_line += 1;
                        own_sum += card_val;
                    }
                    else {
                        opponent_sum += card_val;
                        if (card_val > max_card_opponent) {
                            max_card_opponent = card_val;
                        }
                    }

                    // check if card is out of soft bound (inner borders)
                    if (xc < x_min || xc > x_max || yc < y_min || yc > y_max) {
                        out_of_soft_bounds = true;
                    }
                }
                line.clear(); // delete line

                // impossible to win on current line -> ignore rest on current line
                if (max_card_opponent >= own_highest) {
                    scores[yi][xi] += H_UNWINNABLE_LINE;
                    continue;
                }

                int score = H_OWN_SUM * own_sum + H_OPPONENT_SUM * opponent_sum;
                if (out_of_soft_bounds) {
                    score += H_OUT_OF_SOFT_BOUNDS;
                }

                // add cards in line to score
                if (cards_in_line > 1) {
                    score += H_IN_ROW * cards_in_line * cards_in_line;
                }

                score += overlay_bonus;
                if (cards_in_line == 4) {
                    score += H_WIN;
                }
                
                // save score and in row for current line at pos x, y
                scores[yi][xi] += score;
            }
            lines.clear(); // delete lines
            
            // revert board
            b->set_card(original_card, x, y);
        }
    }
    delete bo;

    std::list<ScorePos*> sorted_scores = std::list<ScorePos*>();
    // insert dummy element, removes empty checks
    sorted_scores.push_back(new ScorePos(-1, -1, std::numeric_limits<int>::min()));

    ScorePos *s;
    for (int yi = 0; yi <= y_diff; yi++) {
        for (int xi = 0; xi <= x_diff; xi++) {
            int score = scores[yi][xi];
            
            // only create ScorePos obj if score should be inserted
            if (score > sorted_scores.back()->score) {
                // add min to correct the offset
                s = new ScorePos(xi + x_min, yi + y_min, score); 
                sorted_insert(&sorted_scores, s, count_spots);
            } 
        }
    }
    // remove trailing dummy node
    sorted_scores.pop_back();

    // no need to delete scores, see board.cpp line 30
    return sorted_scores;
}


// pow by squarring
long long myPow (int base, int exp) {
    long long res = 1;
    while (exp >= 1) {
        if (exp & 1)
            res *= base;
        exp >>= 1;
        base *= base;
    }
    return res;
}


// TODO: check functionality
void maximax_algo(AlgoData *a, Game *g) {
    Board *b = g->get_board();
    Player *player = g->get_players().front();
    int player_id = player->get_id();

    // when in leaf node: add final scores to total
    if (a->curr_depth == a->max_depth) {

        for (auto& card_val : player->get_remaining_cards()) {
            Card *card = new Card(player_id, card_val);

            std::list<ScorePos*> plays = get_best_spots(
                b, b->get_borders(), card, TOP_N_PLAYS, player->get_highest_draw()
            );

            for (auto& p : plays) {
                a->added_score += p->score;
            }

            plays.clear(); // delete plays
            delete card;
        }
        return;
    }

    // takes the current player, call to maximax always advances players
    // TODO: implement actual card removal @ player
    for (auto& card_val : player->get_remaining_cards()) {
        Card *card = new Card(player_id, card_val);
        std::list<ScorePos*> plays = get_best_spots(
            b, b->get_borders(), card, TOP_N_PLAYS, player->get_highest_draw()
        );

        for(auto& p : plays) {
            /*// only change score on own player
            if (player_id == a->me) {
                a->added_score += p->score
            }*/

            // store old card, set new card and evaluate new board
            Card *tmp_card = b->get_card(p->x, p->y);
            b->set_card(card, p->x, p->y);

            bool winner = b->check_winner(player_id, p->x, p->y);

            // no further calc needed on win, add to added score
            if (winner) {
                int base_score = player_id == a->me ? H_WIN : H_LOOSE;
                long long missed_plays = myPow(
                    TOP_N_PLAYS * Card::max_val, (a->max_depth - a->curr_depth)
                );
                a->added_score += base_score * missed_plays;
            }
            else {
                g->advance_round();
                a->curr_depth++;
                // TODO: add print and manually check boards
                maximax_algo(a, g);
                a->curr_depth--;
                g->previous_round();
            }

            // revert played card
            b->set_card(tmp_card, p->x, p->y);
        }

        plays.clear(); // delete plays
        delete card;
    }
}

Point * maximax(Game g, Card *c) {
    Board *b = g.get_board();
    Player *me = g.get_players().front();
    int max_depth = g.get_players().size() * ROUNDS_FORWARD;

    // get top n valid plays, known top card
    std::list<ScorePos*> original_plays = get_best_spots(
            b, b->get_borders(), c, TOP_N_PLAYS, me->get_highest_draw()
    );

    long long max_score = std::numeric_limits<long long>::min();
    int xm, ym;

    // calculate leaf score of each play
    for (auto& play : original_plays) {
        int x0 = play->x;
        int y0 = play->y;

        AlgoData *a = new AlgoData(me->get_id(), 0, max_depth, 0);
        Card *old_card = b->get_card(x0, y0);

        b->set_card(c, x0, y0);
        maximax_algo(a, &g);
        b->set_card(old_card, x0, y0);

        if (a->added_score > max_score) {
            max_score = a->added_score;
            xm = x0;
            ym = y0;
        }
        delete a;
    }
    original_plays.clear(); // delete original_plays

    // return x,y of highest score
    return new Point(xm, ym);
}
