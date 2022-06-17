#ifndef CHECKERSAI_GAME_H
#define CHECKERSAI_GAME_H


#include "Agent.h"



class Game {
public:
    Agent *agent1, *agent2;
    Board board;
    Game(Agent *agent1, Agent *agent2, bool is_player1_starting);
    BoardState play_next_move(); /// zwraca informacje o stanie gry
    bool is_finished() const;
};


#endif //CHECKERSAI_GAME_H
