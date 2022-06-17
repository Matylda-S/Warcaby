
#ifndef CHECKERSAI_AGENT_H
#define CHECKERSAI_AGENT_H


#include "Board.h"

class Agent {
protected:
    bool color; /// true oznacza ze jestesmy player1
    Board *board;
public:
    virtual void init(Board *board, bool color);
    virtual Move get_move() = 0; /// wybiera ruch ktory chce zagrac
};
struct MinMaxRes{
    int eval;//punktacja
    int best_move_id;
    MinMaxRes(int eval, int best_move_id);
};

const int INF = 1e9;
class MinMaxAgent : public Agent{
    int _max_depth;//maksymalna głębokość
    MinMaxRes _minmax(int depth, int alpha = -INF, int beta = INF);

    int _evaluate_board(const DynamicArray<Move> &possible_moves);
public:
    MinMaxAgent(int max_depth);
    Move get_move() override;
    void init(Board *board, bool color) override;
};

class UserAgent : public Agent{
    void _print_moves(DynamicArray<Move> &moves);
    Move _pick_move(DynamicArray<Move> &moves);
public:
    Move get_move() override;
    void init(Board *board, bool color) override;
};


#endif //CHECKERSAI_AGENT_H
