#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "game.h"
#include "board.h"
#include "card.h"


struct CardPos {
    int x, y;
    Card *c;
    CardPos(int x_, int y_, Card *c_) : x(x_), y(y_), c(c_) {};
    ~CardPos() { delete c; };
};

struct ScorePos {
    int x, y, score;
    ScorePos(int x_, int y_, int score_) : x(x_), y(y_), score(score_) {};
    ~ScorePos() {};
};

struct AlgoData {
	int me, play_nr;
	double current_round, added_score, max_score;
	AlgoData(int me_, int p, double c, double a, double m) : 
		me(me_), play_nr(p), current_round(c),  added_score(a), max_score(m) {};
	~AlgoData() {};
};

std::list<std::list<CardPos*>> get_lines_from_pos(Board *b, int x, int y);
std::list<ScorePos*> get_best_spots(Board *b, Card *c, int count_spots, Borders* bo);
Point * maximax(Game g, Card *c);

#endif
