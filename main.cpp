#include "include/game.hpp"

int main(int argc, char** argv){

    if (argc == 2 && !strcmp(argv[1], "clear"))
        std::cout << CLEAR;

    Game G;
    G.start_game();    
    G.game();

    // std::vector<int> T = {1};
    // std::vector<int> empty;
    // std::vector<int> *test = &empty;

    // std::cout << "\n" << test->size();
    // std::cout << "\n" << T.size();

    // test = &T;
    // std::cout << "\n" << test->size();
    // std::cout << "\n" << T.size();

    return 0;
}