#ifndef CHECKERSAI_MOVE_H
#define CHECKERSAI_MOVE_H
#include <ostream>

struct Position{
    int x, y;//współrzędne pionka
    Position();
    Position(int x, int y);
};
std::ostream& operator<<(std::ostream& os, Position const& pos);

struct Move{
    bool player1; /// true jezeli zagral ten ruch gracz 1
    Position from;///pole początkowe
    Position to;//pole końcowe ruchu
    /// potrzebujemy ponizszych informacji aby moc cofnac ruch
    bool pawn_promoted; /// czy ten ruch zmienil piona w krolowa
    bool is_beating; /// true jezeli ten ruch bije jakiegos pionka
    Position pos_beaten; /// pozycja zbitego pionka
    bool was_beaten_queen; /// czy zbity pionek byl krolową
    int capture_number; /// ktore z kolei bicie tą figurą w tej turze (numeracja od 1)
    Move();
    Move(Position from, Position to, bool player1, bool pawn_promoted);
    Move(Position from, Position to, bool player1, bool pawn_promoted, Position pos_beated, bool was_beated_queen,
         int capture_number);
};
std::ostream& operator<<(std::ostream& os, Move const& move);



#endif //CHECKERSAI_MOVE_H
