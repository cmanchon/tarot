#include "include/game.hpp"

int main(int argc, char** argv){

    if (argc == 2 && !strcmp(argv[1], "clear"))
        std::cout << CLEAR;
    
    // Deck D(DFL_DECK);

    // D.print();
    // std::cout << "\n\n";
    
    // D.shuffle();
    // D.print();
    // std::cout << "\n\n";

    // D.sort();
    // D.print();
    // std::cout << "\n\n";

    // Game G(3);

    Game G;
    G.start_game();    
    G.game();

    return 0;
}