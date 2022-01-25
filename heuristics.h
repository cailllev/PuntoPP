#ifndef HEURISTICS_H
#define HEURISTICS_H
#include "game.h"
#include "board.h"


struct CardPos {
    int x, y;
    Card *c;
    CardPos(int x_, int y_, Card *c_) : x(x_), y(y_), c(c_) {};
};

struct ScorePos {
    int x, y, score;
    ScorePos(int x_, int y_, int score_) : x(x_), y(y_), score(score_) {};
};

struct AlgoData {
	int me, play_nr;
	double current_round, added_score, max_score;
	AlgoData(int me_, int p, double c, double a, double m) : 
		me(me_), play_nr(p), current_round(c),  added_score(a), max_score(m) {};
};

std::list<std::list<CardPos*>> get_lines_from_pos(Board *b, int x, int y);
std::list<ScorePos*> get_best_spots(Board *b, Card *c, int count_spots, Borders* bo);
void maximax_algo(AlgoData *a, Game *g);
Point * maximax(Board *b, Card *c);

#endif
