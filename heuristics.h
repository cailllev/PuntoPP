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
	int me, curr_depth, max_depth;
    long long added_score;
	AlgoData(int m, int cd, int md, long a) : 
		me(m), curr_depth(cd), max_depth(md), added_score(a) {};
	~AlgoData() {};
};

std::list<std::list<CardPos*>> get_lines_from_pos(Board *b, int x, int y);
std::list<ScorePos*> get_best_spots(Board *b, Borders *bo, Card *c, int count_spots, int own_highest);
Point * maximax(Game g, Card *c);

#endif
