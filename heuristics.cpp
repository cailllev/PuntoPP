#include "heuristics.h"
#include "board.h"
#include "card.h"
#include "game.h"
#include <iostream>

// TODO: refine
const int H_WIN = 1000;
const int H_IN_ROW = 10;

const int H_OVERLAY_OPPONENT = 3;
const int H_OVERLAY_OWN = -1;

const int H_TOTAL_SUM = 2;
const int H_OWN_SUM = 1;
const int H_CROSS_ROW_SUM = 1;
const int H_OPPONENT_SUM = 2;

const int H_OUT_OF_SOFT_BOUNDS = -1;
const int H_INVALID_PLAY = -1000;

const int dirs[4][2]  = {
    {+1, 0},   // top to bottom
    {+1, +1},  // top left to bottom right
    {0, +1},   // left to right
    {+1, -1},  // top right to bottom left
};

const int top_plays_first_round = 8;                   // how many top plays to inspect for first round
const int top_n_plays_per_round[] = {3, 3, 1};         // how many top plays to inspect for following rounds
const double score_coef_per_round[] = {0.8, 0.6, 0.5}; // smaller rewards for future plays
const int rounds_forward = 3;                          // length of top_n_plays_per_round


std::list<std::list<CardPos*>> get_lines_from_pos(Board *b, int x, int y, Borders *bo) {
    std::list<std::list<CardPos*>> lines = std::list<std::list<CardPos*>>();
    
    for (int d = 0; d < 4; d++) {
        int dy = dirs[d][0];
        int dx = dirs[d][1];

        // get each line of 4 cards in a given direction, i.e.:
        //  x x x x . . .; . x x x x . .; . . x x x x .; . . . x x x x
        // -3-2-1 0         -2-1 0 1         -1 0 1 2          0 1 2 3
        for (int s = -3; s <= 0; s++) {
            std::list<CardPos*> line = std::list<CardPos*>();
            for (int i = 0; i < 4; i++) {
                int xi = x + (s + i) * dx;
                int yi = y + (s + i) * dy;
                if (xi >= bo->ob_x_min && xi <= bo->ob_x_max && yi >= bo->ob_y_min && yi <= bo->ob_y_max)
                    line.push_back(new CardPos(xi, yi, b->get_card(xi, yi)));
            }
            // add line to lines if all 4 cards are in outer borders
            if (line.size() == 4)
                lines.push_back(line);
        }
    }
    return lines;
}

void sorted_insert(std::list<ScorePos*> *l, ScorePos *s, int count_spots) {
    if (l->empty()) {
        l->push_back(s);
        return;
    }

    int i = 0;
    std::list<ScorePos*>::iterator it = l->begin();

    // advance pointer until at end of list; or new score > score at iterator; or score < top x scores
    while (it != l->end() && s->score <= (*it)->score && i < count_spots) {
        it++; i++;
    };

    // insert new score if in top count_spots
    if (i < count_spots) {
        l->insert(it, s);
    }

    // remove last item if list is now too big
    if (l->size() > count_spots) {
        l->pop_back();
    }


}


std::list<ScorePos*> get_best_spots(Board *b, Card *c, int count_spots, Borders *bo) {
    int x_min = bo->ib_x_min;
    int x_max = bo->ib_x_max;
    int y_min = bo->ib_y_min;
    int y_max = bo->ib_y_max;

    int x_diff = x_max - x_min;
    int y_diff = y_max - y_min;
    
    int player_id = c->get_player_id();

    // create scores and in_row arrays
    int in_row[y_diff + 1][x_diff + 1] = {};
    int scores[y_diff + 1][x_diff + 1] = {};

    // iterate only playable board, map to actual coordinates later
    for (int yi = 0; yi <= y_diff; yi++) {
        int y = yi + y_min; // actual coord
        for (int xi = 0; xi <= x_diff; xi++) {
            int x = xi + x_min; // actual coord

            // mark invalid plays and continued
            if (! (b->is_valid_play(c, x, y))) {
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
            int line_c = 0;
            for(auto& line : lines) {

                // heuristics of spot when placing card in current row
                // all cards in current row are within outer borders at the moment
                int cards_in_row = 0; // count of own cards in line
                int own_sum = 0;      // sum of own cards in line
                int opponent_sum = 0; // sum of other cards in line
                bool out_of_soft_bounds = false;

                for (auto& cardpos : line) {
                    Card *card = cardpos->c;
                    int xc = cardpos->x;
                    int yc = cardpos->y;
                    if (card->get_player_id() == player_id) {
                        cards_in_row += 1;
                        own_sum += card->get_value();
                    }
                    else {
                        opponent_sum += card->get_value();
                    }

                    // check if card is out of soft bound (inner borders)
                    if (xc < x_min || xc > x_max || yc < y_min || yc > y_max) {
                        out_of_soft_bounds = true;
                    }
                }

 
                int score = H_OWN_SUM * own_sum + H_OPPONENT_SUM * opponent_sum;
                if (out_of_soft_bounds) {
                    score += H_OUT_OF_SOFT_BOUNDS;
                }

                // find crossing rows (and add score to them)
                int in_row_existing = in_row[yi][xi];
                int score_existing = scores[yi][xi];
                
                if (in_row_existing > cards_in_row) {
                    cards_in_row = in_row_existing;
                }
                
                if (cards_in_row == 4) {
                    score += H_WIN;
                }
                score += H_CROSS_ROW_SUM * score_existing + overlay_bonus;
                
                // save score and in row for current line at pos x, y
                in_row[yi][xi] = cards_in_row;
                scores[yi][xi] = score;
            }
            
            // revert board
            b->set_card(original_card, x, y);
        }
    }

    std::list<ScorePos*> sorted_scores = std::list<ScorePos*>();

    // add in_row and score to complete score
    int score, cards_in_row;
    ScorePos *s;
    for (int yi = 0; yi <= y_diff; yi++) {
        for (int xi = 0; xi < x_diff; xi++) {
            cards_in_row = in_row[yi][xi];
            score = H_TOTAL_SUM * scores[yi][xi] + H_IN_ROW * cards_in_row * cards_in_row;
            s = new ScorePos(xi + x_min, yi + y_min, score); // add min to correct the offset
            sorted_insert(&sorted_scores, s, count_spots);
        }
    }
    return sorted_scores;
}


void maximax_algo(AlgoData *a, Game *g) {

    // when in leaf node: check if accumulated score > previous max of leafs, then store in results
    if (a->current_round == rounds_forward) {
        if (a->added_score > a->max_score) {
            a->max_score = a->added_score;
        }
        return;
    }

    // get best play for next player
    g->advance_round();
    for (auto& player : g->get_players()) {

        Card *avg_card = player->get_avg_draw();
        int top_n_plays = top_n_plays_per_round[(int) (a->current_round)];
        std::list<ScorePos*> scores = get_best_spots(
            g->get_board(), avg_card, top_n_plays, g->get_board()->get_borders()
        );

        a->current_round += 1 / g->get_players().size(); // increase current round

        for(auto& s : scores) {
            // only change score on own player
            if (player->get_id() == a->me)
                a->added_score += s->score * score_coef_per_round[(int) (a->current_round)];

            // store old card, set new card and evaluate new board
            Card *tmp_card = g->get_board()->get_card(s->x, s->y);
            g->get_board()->set_card(avg_card, s->x, s->y);
            maximax_algo(a, g);

            // revert play
            g->get_board()->set_card(tmp_card, s->x, s->y);
        }
    }

    // revert player
    g->previous_round();
}


// TODO: implement shallow pruning, correct and sanitycheck algo
// minimax devolves to 1 v all in multiplayer settings
//   -> use maximax (maximize for all players individually)
Point * maximax(Game *g, Card *c) {
    int me = (g->get_players().front())->get_id();

    // get top n valid plays
    std::list<ScorePos*> original_plays = get_best_spots(
        g->get_board(), c, top_plays_first_round, g->get_board()->get_borders()
    );

    // calculate score of each play
    int i = 0;
    double max_max_score = 0;
    int xm, ym;

    for (auto& play : original_plays) {
        int x0 = play->x;
        int y0 = play->y;
        int score0 = play->score;

        AlgoData *a = new AlgoData(me, i++, 0.0, score0, 0);
        Card *old_card = g->get_board()->get_card(x0, y0);

        g->get_board()->set_card(c, x0, y0);
        maximax_algo(a, g);
        g->get_board()->set_card(old_card, x0, y0);

        if (a->max_score > max_max_score) {
            max_max_score = a->max_score;
            xm = x0;
            ym = y0;
        }
    }

    // return x,y of highest score
    return new Point(xm, ym);
}
