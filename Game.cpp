#include "Game.h"
/*Inicjalizacja gry*/
Game::Game(Agent *agent1, Agent *agent2, bool is_player1_starting) : board(is_player1_starting){
    this->agent1 = agent1;
    this->agent2 = agent2;
    agent1->init(&board, true);
    agent2->init(&board, false);
}
/*granie następnego ruchu*/
BoardState Game::play_next_move() {
    Agent *agent = (board.get_color_on_move() ? agent1 : agent2);//który gracz gra
    Move move = agent->get_move();//wybór ruchu
    board.play_move(move);//zagranie ruchu
    return board.get_state(board.get_all_possible_moves());//zwrócenie stanu gry
}
/*sprawdzenie czy gra się skończyła*/
bool Game::is_finished() const {
    return board.get_state(board.get_all_possible_moves()) != IN_PROGRESS;
}
