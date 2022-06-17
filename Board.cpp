
#include <cstring>
#include <iostream>
#include "Board.h"
#include "Helpers.h"

/*w zależności od tego jaki i czyj pionek jestna polu, zwrócenie odpowiedniego znaku*/
char board_field_to_char(BoardField boardField) {
    switch (boardField) {
        case BoardField::NOTHING://jesli na polu nie ma nic to pole jest oznaczone "."
            return '.';
        case BoardField::PAWN1://jesli na polu jest pionek gracza 1 pole jest oznaczone "f"
            return 'f';
        case BoardField::PAWN2://jesli na polu jest pionek gracza 2 pole jest oznaczone "F"
            return 'F';
        case BoardField::QUEEN1://jesli na polu jest damka gracza 1 pole jest oznaczone "q"
            return 'q';
        case BoardField::QUEEN2://jesli na polu jest damka gracza 2 pole jest oznaczone "Q"
            return 'Q';
    }
    return 0; /// zeby nie bylo warningow
}
/*sprawdzenie czy na danym polu znajdujesię figura gracza 1*/
inline bool get_board_field_color(BoardField boardField) {
    return boardField == BoardField::PAWN1 || boardField == BoardField::QUEEN1;//jesli na polu znajduje się figura gracza 1 to zwraca prawde;
}
/*sprawdzenie czy na danym polu znajdujesię damka*/
inline bool is_board_field_queen(BoardField boardField) {
    return boardField == BoardField::QUEEN1 || boardField == BoardField::QUEEN2;
}
/*zwraca odpowiednia figure*/
BoardField create_board_field(bool color, bool is_queen) {
    return (color
        ? (is_queen ? QUEEN1 : PAWN1)//jesli na polu znajduje się figura gracza 1 to sprawdzamy czy jest to damka, jesli tak zwracamy damke gracza 1 jesli nie pionka
        : (is_queen ? QUEEN2 : PAWN2)
    );
}



Board::Board(bool is_player1_on_move) {
    this->_reset_board(is_player1_on_move);
}
/*ustawienie planszy w pozycji początkowej*/
void Board::_reset_board(bool is_player1_on_move) {
    _queens_cnt[0] = _queens_cnt[1] = 0;//brak damek u obu graczy
    _pawns_cnt[0] = _pawns_cnt[1] = 12;//12 pionkow u obu graczy
    _capture_number = 0;//ilosc bić figury w turze=0
    _is_player1_on_move = is_player1_on_move;
    memset(_board_table, BoardField::NOTHING, sizeof(BoardField)*8*8);//ustawienie wszystkich pól tablicy na NOTHING

    for(int i = 0; i<8; i++){//i-numer wiersza od góry/
        if(i == 3 || i == 4)//na wierszach 3 i 4 nie ma figur
            continue;
        for(int j = (i+1)%2; j<8; j+=2)//j-numer kolumny od lewej
            _board_table[j][i] = (i>4 ? BoardField::PAWN2 : BoardField::PAWN1);//przypisanie odpowiedniej figury w pole planszy
    }
}
/*Narysowanie planszy*/
void Board::print() const {
    
    for(int j = 0; j<8; j++){
        std::cout <<8-j<<" ";
        for(int i = 0; i<8; i++){
            std::cout << board_field_to_char(_board_table[i][j]) << " ";//Wypisanie odpowiedniego symbolu oznaczającego odpowiednią figurę
        }
        std::cout<< "\n";
    }
    std::cout<<"  "<<"A "<<"B "<<"C "<<"D "<<"E "<<"F "<<"G "<<"H "<<"\n";
    std::cout << "pionki1: " << get_amount_of_pawns(true) << " " << "damki1: " << get_amount_of_queens(true)//Wypisanie ilości pionków i damek gracza 1
            << std::endl << "pionki2: " << get_amount_of_pawns(false) << " " << "damki2: " << get_amount_of_queens(false) << std::endl<< std::endl;//Wypisanie ilości pionków i damek gracza 2
    std::cout << "GRACZ " <<  2 - get_color_on_move()  << " na ruchu" << std::endl;//Wypisanie który gracz jest na ruchu

   std::cout << std::flush;
}
/*Sprawdzenie czy pionek stał się damka*/
bool Board::_should_be_promoted(Position position, BoardField piece) const {
    if(piece == NOTHING)
        return false;
    return (piece == PAWN1 && position.y == 7)//jesli pionek jest gracza 1 i znajduje się w 7 rzędzi to jest damką i zwracamy prawde
           || (piece == PAWN2 && position.y == 0);//analogicznie dla pionka 2 gracza
}

// zwrca możliwe ruchy pionka o danym kolorze z danej pozycji
DynamicArray<Move> Board::_get_pawn_moves(Position from, bool color) const {
    DynamicArray<Move> res;//wektor

    int direction = (color ? 1 : -1);//kierunek ruchu pionka zalezy od  gracza, jesli color to prawda to gra gracz 1
    Position to_list[2] = {
        Position(from.x - 1, from.y + direction),//możliwe pozycje końcowe
        Position(from.x + 1, from.y + direction)
    };
    for(auto & to : to_list){
        if(to.x >= 0 && to.x < 8 && to.y >= 0 && to.y<8 && _board_table[to.x][to.y] == NOTHING)//sprawdzenie czy pionek nie wyjdzie za planszę i czy pole końcowe jest puste
            res.push(Move(from, to, color,
                          _should_be_promoted(to, _board_table[from.x][from.y])));//jesli tak to dodajemy możliwy ruch do wektora
    }

    return res;//zwracamy wektor
}



/*zwraca możliwe zbicia dla danego pionka*/
DynamicArray<Move> Board::_get_pawn_beats(Position from, bool color) const {
    DynamicArray<Move> res;
    int offset_x = -1, offset_y = -1;  /// zeby nie robić 4 dlugich ifow i nie powtarzac kodu,zmieniając na 1, możemy rozważyć bicia w wszytskich 4 kierunkach
    for(int i = 0; i<4; i++){
        int offset_x2 = 2*offset_x,
            offset_y2 = 2*offset_y;

        Position to = Position(from.x + offset_x2, from.y + offset_y2);//pozycja po zbiciu
        Position beaten_pos = Position(from.x + offset_x, from.y + offset_y);//pozycja bitego pionka
///Sprawdzenie czy pionek po bicu zmieści się w planszy,czy trafi na puste pole,czy jest figura do bicia i czy bity pionek jest przeciwnika
        if(to.x >= 0 && to.y >= 0
                && to.x < 8 && to.y < 8
                && _board_table[to.x][to.y] == BoardField::NOTHING
                && _board_table[beaten_pos.x][beaten_pos.y] != BoardField::NOTHING
                && get_board_field_color(_board_table[beaten_pos.x][beaten_pos.y]) != color){

            bool was_beaten_queen = is_board_field_queen(_board_table[beaten_pos.x][beaten_pos.y]);//sprawdzenie czy bita była damka
            res.push(Move(
                    from,
                    to,
                    color,
                    _should_be_promoted(to, _board_table[from.x][from.y]),
                    beaten_pos,
                    was_beaten_queen,
                    _capture_number+1));//dodanie elementu do wektora
        }

        offset_y *= -1;
        if(i == 1)
            offset_x *= -1;
    }
    return res;//zwrócenie wketora możliwych bić
}
/* zwrca możliwe ruchy damki o danym kolorze z danej pozycji*/
DynamicArray<Move> Board::_get_queen_moves(Position from, bool color) const {
    DynamicArray<Move> res;

    int offset_x = -1, offset_y = -1;//zmieniając offesty na 1 pozwala nam to sparwdzać ruchy w 4 kierunkach
    for(int i = 0; i<4; i++){
        Position new_pos(from.x + offset_x, from.y+offset_y);//pozycja końcowa
        while(new_pos.x >= 0 && new_pos.x < 8 && new_pos.y >=0 && new_pos.y<8){//dopuki damka nie "wyjdzie" za planszę
            if(_board_table[new_pos.x][new_pos.y] != NOTHING){//sorawdzenie czy pole końcowe jest zajęte
                break;
            }
            res.push(Move(from, new_pos, color,
                          _should_be_promoted(new_pos, _board_table[from.x][from.y])));//dodanie ruchu
            new_pos.x += offset_x;//zwiekszamy "długość" ruchu w pionie i poziomie
            new_pos.y += offset_y;
        }

        offset_y *= -1;
        if(i == 1)
            offset_x *= -1;
    }
    return res;//zwracamy możliwe ruchy damki
}
/* zwrca możliwe zbicia krolowej o danym kolorze z danej pozycji*/
DynamicArray<Move> Board::_get_queen_beats(Position from, bool color) const {
    DynamicArray<Move> res;

    int offset_x = -1, offset_y = -1;
    for(int i = 0; i<4; i++){
        Position new_pos(from.x + offset_x, from.y+offset_y);//nowa pozycja
        bool was_beaten_piece = false; /// czy juz cokolwiek bylo zbite
        bool was_beaten_queen = false; /// czy zbita figura to krolowa
        Position beaten_pos;
        while(new_pos.x >= 0 && new_pos.x < 8 && new_pos.y >=0 && new_pos.y<8){//dopuki damka pozostane na planszy
            if (_board_table[new_pos.x][new_pos.y] != NOTHING) {//sprawdzenie czy pole na którym jest bita figura nie jest puste
                if(was_beaten_piece) /// nie mozemy zbic dwoch figur naraz
                    break;
                if (get_board_field_color(_board_table[new_pos.x][new_pos.y]) == color) /// nie mozemy zbic swojej figury
                    break;
                was_beaten_piece = true;
                beaten_pos = new_pos;//pozycja zbitej figury
                was_beaten_queen = is_board_field_queen(_board_table[new_pos.x][new_pos.y]);//sprawdzenie czy bitą figurą była damka
            }
            else{
                if(was_beaten_piece) {
                    res.push(Move(from,
                                  new_pos,
                                  color,
                                  _should_be_promoted(new_pos, _board_table[from.x][from.y]),
                                  beaten_pos,
                                  was_beaten_queen,
                                  _capture_number+1));
                }
            }//jesli nastapiło zbicie dodajemy ruch zbicia do wektora
            new_pos.x += offset_x;//damka kończy bicie "za" zbitą figurą lub dalej
            new_pos.y += offset_y;
        }

        offset_y *= -1;
        if(i == 1)
            offset_x *= -1;
    }
    return res;//zwracamy wektor
}
/*zwrca możliwe ruchy z danej pozycji*/
DynamicArray<Move> Board::_get_moves(Position from) const {
    bool color = get_color_on_move();
    //sprawdzenie czy pole które sprawdzamy nie jest puste i czy figura na tym jest koloru obecnie grającego gracza
    if(_board_table[from.x][from.y] != NOTHING
            && get_board_field_color(_board_table[from.x][from.y]) == color){
        if(is_board_field_queen(_board_table[from.x][from.y]))//sprawdzenie czy na polu jest damka
            return _get_queen_moves(from, color);//możliwe ruchy danej damki
        return _get_pawn_moves(from, color);//możliwye ruchy danego pionka
    }
    return DynamicArray<Move>();//zwrócenie wektora ruchów
}
/* zwrca możliwe zbicia z danej pozycji*/
DynamicArray<Move> Board::_get_beats(Position from) const {
    bool color = get_color_on_move();
    //sprawdzenie czy pole które sprawdzamy nie jest puste i czy figura na tym jest koloru obecnie grającego gracza
    if(_board_table[from.x][from.y] != NOTHING
       && get_board_field_color(_board_table[from.x][from.y]) == color){
        if(is_board_field_queen(_board_table[from.x][from.y]))//sprawdzenie czy na polu jest damka
            return _get_queen_beats(from, color);//możliwe ruchy damki 
         return _get_pawn_beats(from, color);   //możliwe ruchy pionka
    }

    return DynamicArray<Move>();//zwrócenie wektora ruchów
}
/* zwraca wszystkie mozliwe ruchy gracza aktualnie na ruchu, wliczajac bicia*/
DynamicArray<Move> Board::get_all_possible_moves() const {
    if(this->_capture_number > 0){  /// jezeli jest to drugie lub kolejne bicie w tej turze
        return _get_beats(this->_last_capture_piece_pos);//zwraca bicia dla pola na którym jest figura która ostatnio zbiła
    }
    DynamicArray<Move> beat_moves;//wektor zbic
    DynamicArray<Move> normal_moves;//wektor ruchów bez zbić

    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            Position pos(i, j);

            beat_moves.push_many(_get_beats(pos));//dodaje do wektora możliwe zbicia z kolejnych możliwych pozcycji
            normal_moves.push_many(_get_moves(pos));//dodaje możliwe ruchy z kolejnych pozycji
        }
    }

    if(beat_moves.size() > 0) /// bicie jest przymusowe
        return beat_moves;
    return normal_moves;
}
/*zwraca prawde jesli gra gracz 1*/
bool Board::get_color_on_move() const {
    return _is_player1_on_move;
}

/*zwraca ilość pionków danego gracza*/
int Board::get_amount_of_pawns(bool player) const {
    return _pawns_cnt[(player ? 0 : 1)];
}
/*zwraca ilość damek gracza*/
int Board::get_amount_of_queens(bool player) const {
    return _queens_cnt[(player ? 0 : 1)];
}
/*zwraca ilość figur danego gracza (pionków+damek)*/
int Board::get_amount_of_pieces(bool player) const {
    return get_amount_of_pawns(player) + get_amount_of_queens(player);
}
/*Zamina liczby figur*/
void Board::_change_pieces_cnt(BoardField piece, int change) {
    bool color = get_board_field_color(piece);//sprawdzenie czy figura jest gracza 1
    bool is_queen = is_board_field_queen(piece);//sprawdzenie czy jest damką
    if(is_queen)
        _queens_cnt[(color ? 0 : 1)] += change;
    else
        _pawns_cnt[(color ? 0 : 1)] += change;
}
/*cofa ostatnio zagrany ruch*/
void Board::revert_move() {
    Move move = _moves_history.pop();//zdejmuje ostatni ruch z wektora

    _is_player1_on_move = move.player1;//gracz ktory wykonal ostatni ruch
    if(move.pawn_promoted){//sprawdza czy pionek zamienil sie w damke
        _change_pieces_cnt(_board_table[move.to.x][move.to.y], -1);//zmniejsza liczbe damek o 1
        _board_table[move.to.x][move.to.y] = (move.player1 ? PAWN1 : PAWN2);
        _change_pieces_cnt(_board_table[move.to.x][move.to.y], 1);//zwieksza liczbe pionkow o 1
    }
//sprawdza czy nie bylo bicia
    if(!move.is_beating){
        _capture_number = 0;
    }
    else{
        _capture_number = move.capture_number-1;//zmniejsza ilość bić o 1
        _last_capture_piece_pos = move.from;//zmienienie polozenia figury ktora zbila na wczesniejsza pozycje 

        BoardField beaten_piece = create_board_field(!move.player1, move.was_beaten_queen);//tworzymy zbitą figurę
        _board_table[move.pos_beaten.x][move.pos_beaten.y] = beaten_piece;//zbita figura wraca na pole
        _change_pieces_cnt(beaten_piece, 1);//zwiekszenie liczby figur przeciwnika o 1

    }


    swap(_board_table[move.from.x][move.from.y], _board_table[move.to.x][move.to.y]);//zamieniamy pola
}

/*Wykonanie  ruch, jesli bylo zbicia to nie ma zmiany gracza */
void Board::play_move(Move move) {
    swap(_board_table[move.to.x][move.to.y], _board_table[move.from.x][move.from.y]);//zamieniamy miejsce figury wykonującej ruch

    bool has_next_beat = false;
    if(!move.is_beating){//sprawdzenie czy  ruch to nie jest bicie
        _is_player1_on_move = !_is_player1_on_move;//zmiana gracza
        _capture_number = 0;//ustawienei bić  wturze na 0;
    }
    else{
        _change_pieces_cnt(_board_table[move.pos_beaten.x][move.pos_beaten.y], -1);//zmniejszenie figur perezciwnika
        _board_table[move.pos_beaten.x][move.pos_beaten.y] = NOTHING;//pole na ktorym była figura staje się puste

        has_next_beat = _get_beats(move.to).size() > 0;
        if(has_next_beat){//jesli jest możliwość bicia
            _last_capture_piece_pos = move.to;//przesuwamy figurę ktora bije
            _capture_number++;//ziwekszamy liczbe bić
        }
        else{
            _is_player1_on_move = !_is_player1_on_move;//jesli nie ma bicia zmieniamy gracza
            _capture_number = 0;//zerujemy ilość zbić
        }
    }

    if(move.pawn_promoted && has_next_beat){ /// jezeli staje sie damka w wyniku bicia i ma kolejne bicie to nie jest damka
        move.pawn_promoted = false;
    }
    if(move.pawn_promoted){//jesli staje sie damką to zmieniamy ilość pionków i damek
        _change_pieces_cnt(_board_table[move.to.x][move.to.y], -1);
        _board_table[move.to.x][move.to.y] = (move.player1 ? QUEEN1 : QUEEN2);//na polu stawiamy damke
        _change_pieces_cnt(_board_table[move.to.x][move.to.y], 1);
    }
    _moves_history.push(move);//dodajemy ruch do wektora
}
/*zwraca stan gry*/
BoardState Board::get_state(const DynamicArray<Move> &all_possible_moves) const {
    if (get_amount_of_pieces(true) == 0)//jesli gracz 1 nie ma już figur to wygrywa gracza 2
        return WIN_2;
    if (get_amount_of_pieces(false) == 0)
        return WIN_1;
    if (all_possible_moves.size() == 0)//jesli nie ma możliwoścu ruchu wygrywa gracz przeciwny
        return (get_color_on_move() ? WIN_2 : WIN_1);
    return IN_PROGRESS;
}


