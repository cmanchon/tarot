#include "include/game.hpp"

int main(int argc, char** argv){

    if (argc == 2 && !strcmp(argv[1], "clear"))
        std::cout << CLEAR;

    Game G;
    G.start_game();    
    G.game();



    return 0;
}