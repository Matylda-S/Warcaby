
#include "Agent.h"
#include "Helpers.h"
/*inicjowanie gracza*/
void Agent::init(Board *board, bool color) {
    this->board = board;
    this->color = color;
}
/*inicjowanie gracza opartego o algorytm minmax*/
void MinMaxAgent::init(Board *board, bool color) {
    Agent::init(board, color);
}

/*inicjalizowanie Minmaxa*/
MinMaxRes::MinMaxRes(int eval, int best_move_id) {
    this->eval = eval;
    this->best_move_id = best_move_id;
}
/*maksymalna głębokość*/
MinMaxAgent::MinMaxAgent(int max_depth) {
    _max_depth = max_depth;
}
/*Wybór ruchu */
Move MinMaxAgent::get_move() {
    auto res= _minmax(0);
    return board->get_all_possible_moves()[res.best_move_id];//zwrócenie ruchu o janwiększej korzyści dla gracza
}
/*algorym MinMax*/
MinMaxRes MinMaxAgent::_minmax(int depth, int alpha, int beta) {
    auto moves = board->get_all_possible_moves();//wszytski możliwe ruchy gracza na ruchu
    auto state0 = board->get_state(moves);//status gry

    if (depth == _max_depth || state0 != IN_PROGRESS) {//sprawdzamy czy doszliśmy do maksymalnej głębokości lub czy gra się zakończyła
        return MinMaxRes(_evaluate_board(moves), -1);//jeśli tak zwracamy MinMaxRes z obecną punktacją
    }

    bool player1 = board->get_color_on_move();

    MinMaxRes best(-1, -1);
    for (int i = 0; i<moves.size() && alpha < beta; i++) {//pokolei idizemy po możliwych ruchach i sprawdzamy czy alfa jest mniejsza od bety
        const Move &move = moves[i];

        board->play_move(move);//gramy ruch
        MinMaxRes resi = _minmax(depth+1, alpha, beta);//Wywołujemy minmaxa z większą głębokością
        if(best.best_move_id == -1){ /// jezeli best niebylo ustawione to ustawiamy
            best.best_move_id = i;// id ruchu
            best.eval = resi.eval;//wartość ruchu
        }
        if (player1) {  /// player1 chce zmaksymalizowac eval
            if(resi.eval > best.eval){
                best.eval = resi.eval;//jesli obecnie sprawdzany ruch ma lepszą wartość to zamieniamy wartość minmaxa i id najlepszego ruchu
                best.best_move_id = i;
            }
            alpha = max(alpha, resi.eval);//przypisujemy alfie większą z wartości
        }
        else{//player 2 chce zminimalizowac cel
            if(resi.eval < best.eval){
                best.eval = resi.eval;
                best.best_move_id = i;
            }
            beta = min(beta, resi.eval);//przypisujemy Becie mneijsząz wartości
        }
        board->revert_move();//cofnięcie ruchu
    }
    return best;//zwrócenie najlepszego ruchu
}
/*zwraca punktacje*/
int MinMaxAgent::_evaluate_board(const DynamicArray<Move> &possible_moves) {
    auto state = board->get_state(possible_moves);//stan gry
    if (state == WIN_1)
        return INF;//jesli wygrał gracz 1 to zwracamy nieskończoność
    if (state == WIN_2)
        return -INF;//jeśli wygrał gracz 2, zwracamy minus nieskończonosć
    return board->get_amount_of_queens(true) * 3 + board->get_amount_of_pawns(true)//w przeciwnym razie punktacja to 3* ilość damek + ilość pionków gracza 1 pomniejszona o 3*ilośćdamek + ilośc pionków gracza 2
           - (board->get_amount_of_queens(false) * 3 + board->get_amount_of_pawns(false));
}
/*zaincjalizowania gracza UserAgent*/
void UserAgent::init(Board *board, bool color) {
    Agent::init(board, color);
}
/*Wybór ruchu przez gracza*/
Move UserAgent::get_move() {
    auto moves = board->get_all_possible_moves();//wszysttkie możliwe ruchy gracza
    Move move = _pick_move(moves);//wybranie ruchu
    return move;//zwrócenie ruchu
}
/*Wypisanie możliwych ruchów*/
void UserAgent::_print_moves(DynamicArray<Move> &moves) {
    std::cout << "(GRACZ " << 2 - (int) color << ") Mozliwe ruchy:" << std::endl;
    for (int i = 0; i < moves.size(); i++) {
        std::cout << i + 1 << ") " << moves[i] << std::endl;//Wypisanie możliwych ruchów
    }
}
/*wybranie ruchu przez użytkownika*/
Move UserAgent::_pick_move(DynamicArray<Move> &moves) {
    _print_moves(moves);//Wypisanie możliwych ruchów
    std::cout << "Podaj numer ruchu\n>";
    while (true) {
        int move_id;
        std::cin >> move_id;//wczytanie numeracji wybranego ruchu
        //std::cin.ignore(256, '\n');
        move_id--;//rzeczywiste id ruchów są liczone od 0
        if (std::cin.fail() || move_id < 0 || move_id >= moves.size()) {//sprawdzenie czy wybór został zrobiony popranwie
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cout << "Bledny numer ruchu" << std::endl;
            std::cout << "Podaj poprawny numer ruchu\n>";
            continue;
        }
        else
        std::cin.ignore(256, '\n');
        return moves[move_id];//zwrócenie ruchu o opdowiednim id
    }
}


