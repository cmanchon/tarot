#include "game.hpp"
#ifndef AI_HPP
#define AI_HPP

struct Move{
    int value;
    Deck cards;
    std::vector<bool> is_mate;
    
    Move(int id, Game G, int first_player){
        cards = G.get_pli();

        int i = first_player;
        while ((int)is_mate.size() < G.nb_players()){
            if (G.is_preneur(id) == G.is_preneur(i))
                is_mate.push_back(true);
            else
                is_mate.push_back(false);
            i++;

            if (i >= G.nb_players())
                i=0;
        }

        //on détermine la value
        if (G.is_preneur(G.get_players_id(G.plis_winner(first_player))) == G.is_preneur(id)){
            if (G.jeu_points() >= 2 * G.nb_players())
                value = 3;
            else
                value = 2;
        }
        else{
            if (G.jeu_points() >= 2 * G.nb_players())
                value = 0;
            else
                value = 1;
        }
    }
};

class Moves{
    private:
        int player_id;
        std::vector<Move> moves;
    public:
        Moves(){};
        Moves(int id){player_id = id;};
        Moves(int id, Game G, int first_player){moves.push_back({id, G, first_player});};
        Moves(std::string filename);     //csv maybe
        ~Moves(){};

        void add(int id, Game G, int first_player){moves.push_back({id, G, first_player});};

        void remove_useless();      //low and doublon
        bool is_move_similar();     //ou les save de manière à ce que ça se check facilement
        void save_in_file(std::string filename);
};



#endif