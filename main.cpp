/*Matylda Szadkowska 259334*/
#include <iostream>
#include "Game.h"

int main() {

    /// mozna zmienic Agenta na innego np. z UserAgent na MinMaxAgent aby grały dwa boty lub odwrotnie aby grali ludzie
    //MinMaxAgent player1(8); /// im wieksza glebokosc tym dluzej mysli 
    //MinMaxAgent player2(4);
//    aby gral czlowiek z komputerem
    MinMaxAgent player1(4); 
   UserAgent player2;
    Game game(&player1, &player2, false); /// pierwszy gracz to ten na gorze

    while(!game.is_finished()){//sprawdzamy czy gra nie jest zakończona
        game.board.print();

        std::cout << "Kliknij ENTER aby kontynuwac..." << std::flush;
        getchar();
        std::cout << "\n";

        auto state = game.play_next_move();
        //Sprawdzamy stan gry po wykonanym ruchu
        switch (state) {
            case IN_PROGRESS:
                break;
            case WIN_1:
                game.board.print();
                std::cout << "Wygrywa player 1";
                break;
            case WIN_2:
                game.board.print();
                std::cout << "Wygrywa player 2";
                break;
        }
    }



    return 0;
}
