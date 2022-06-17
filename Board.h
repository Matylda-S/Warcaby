#ifndef CHECKERSAI_BOARD_H
#define CHECKERSAI_BOARD_H
#include "Move.h"
#include "DynamicArray.h"

/*Pole planszy*/
enum BoardField{
    NOTHING,//puste pole
    PAWN1,// pionkek gracza 1
    PAWN2,//pionkek gracza 2
    QUEEN1,//damka gracza 1
    QUEEN2//damka gracza 2
};
char board_field_to_char(BoardField boardField);
bool get_board_field_color(BoardField boardField);
bool is_board_field_queen(BoardField boardField);
BoardField create_board_field(bool color, bool is_queen);

enum BoardState{
    WIN_1, WIN_2, IN_PROGRESS
};

class Board {
    BoardField _board_table[8][8];
    DynamicArray<Move> _moves_history; /// pierwszy wymiar to x drugi to y
    bool _is_player1_on_move;
    int _pawns_cnt[2], _queens_cnt[2];//ilosc pionków i damek obu graczy
    void _change_pieces_cnt(BoardField piece, int change);

    int _capture_number; /// ile razy w tej turze figura juz zbila
    Position _last_capture_piece_pos; /// obecna pozycja figury ktora ostatnio zbila

    bool _should_be_promoted(Position position, BoardField piece) const;


    DynamicArray<Move> _get_pawn_moves(Position from, bool color) const; /// zwrca możliwe ruchy pionka o danym kolorze z danej pozycji
    DynamicArray<Move> _get_pawn_beats(Position from, bool color) const; /// zwrca możliwe zbicia pionka o danym kolorze z danej pozycji
    DynamicArray<Move> _get_queen_moves(Position from, bool color) const; /// zwrca możliwe ruchy damki o danym kolorze z danej pozycji
    DynamicArray<Move> _get_queen_beats(Position from, bool color) const; /// zwrca możliwe zbicia damki o danym kolorze z danej pozycji

    DynamicArray<Move> _get_moves(Position from) const; /// zwrca możliwe ruchy z danej pozycji
    DynamicArray<Move> _get_beats(Position from) const; /// zwrca możliwe zbicia z danej pozycji

    void _reset_board(bool is_player1_on_move); /// ustawia board do początkowej pozycji
public:
    Board(bool is_player1_on_move = false);
    DynamicArray<Move> get_all_possible_moves() const; /// zwraca wszystkie mozliwe ruchy gracza aktualnie na ruchu, wliczajac bicia
    void play_move(Move move); /// jezeli chcemy wykonac np. podwojne bicie to wykonujemy te funkcje 2 razy, kolor sie wtedy nie zmienia
    void revert_move(); /// cofa ostatnio zagrany ruch
    void print() const; /// rysuje plansze
    bool get_color_on_move() const; /// zwracza czy gracz 1 jest na ruchu
    int get_amount_of_pieces(bool player) const; /// zwraca ilosc bierek danego gracza  (pieces = pawns + queens)
    int get_amount_of_pawns(bool player) const; /// zwraca pionkow
    int get_amount_of_queens(bool player) const; /// zwraca damek

    BoardState get_state(const DynamicArray<Move> &all_possible_moves) const;
};


#endif //CHECKERSAI_BOARD_H
