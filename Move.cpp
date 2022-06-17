#include <ostream>
#include "Move.h"

/*kontruktor*/
Position::Position() {
    this->x = this->y = 0;
}
/*zainicjowanie pozycji*/
Position::Position(int x, int y) {
    this->x = x;
    this->y = y;
}
/*przeciążenie operatora <<*/
std::ostream& operator<<(std::ostream& os, Position const& pos)
{
    os << (char)('A'+pos.x) << 8-pos.y;//wypisanie pozycji
    return os;
}
/*Ruch bez bicia*/
Move::Move(Position from, Position to, bool player1, bool pawn_promoted) {
    this->from = from;//z jakiego pola jest ruch
    this->to = to;//do jakiego pola jest ruch
    this->player1 = player1;//czy graczem jest 1 gracz
    this->pawn_promoted = pawn_promoted;//czy pionek zamienil sie  w damke
    this->is_beating = false;//brak bicia
    this->capture_number = 0;//ktore bicie z kolei figurą (nie ma bicia czyli 0)
}
/*Ruch z biciem*/
Move::Move(Position from, Position to, bool player1, bool pawn_promoted, Position pos_beated, bool was_beated_queen,
           int capture_number)
        : Move(from, to, player1, pawn_promoted) {
    this->is_beating = true;//bicie
    this->pos_beaten = pos_beated;//pozycja zbitego pionka
    this->was_beaten_queen = was_beated_queen;//czy zbity pionek byl krolową
    this->capture_number = capture_number;//kote bicie z kolei tą figurą
}

Move::Move() {}

/*Przeciążenie operatora <<*/
std::ostream &operator<<(std::ostream &os, const Move &move) {
    os << move.from << " --> " << move.to;//zapis skąd do kąd byl ruch
    if(move.is_beating)//sprawdzenie czy bylo bicie
        os << " (BICIE)";
    if(move.pawn_promoted)//sprawdzenie czy pionek zotsał damką
        os << " (PROMOCJA W DAME)";
    return os;
}