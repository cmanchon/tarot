#ifndef AI_HPP
#define AI_HPP
#include "game.hpp"

struct Move{
    int value;
    Deck cards;
    std::vector<bool> is_mate;
    
    Move(int v, Deck D, std::vector<bool> tmp){
        value = v;
        cards = D;
        is_mate = tmp;
    }

    Move(int id, Game G, int first_player){
        std::cout << "id : " << id;

        cards = G.get_pli();

        int i = first_player;
        while ((int)is_mate.size() < G.nb_players()){
            if (G.are_mates(G.get_players_ind(id), i))
                is_mate.push_back(true);
            else
                is_mate.push_back(false);
            i++;

            if (i >= G.nb_players())
                i=0;
        }

        //on détermine la value
        if (G.are_mates(G.pli_winner(first_player), G.get_players_ind(id))){    
            if (G.jeu_points() >= 1.5 * G.nb_players())
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
        Moves(int id = -1, std::string filename = "");     //csv maybe
        ~Moves(){};

        void print(int ind) const;
        void print() const;
        int get_id() const{return player_id;};
        int size() const{return (int) moves.size();};

        void add(int id, Game G, int first_player){moves.push_back({id, G, first_player});};

        void remove_useless();      //low and doublon
        bool is_move_similar();     //ou les save de manière à ce que ça se check facilement
        void save_in_file(std::string filename);
        std::vector<Deck> relevant_moves(Game G, int first_player) const;

        Card AI_play_ML(Game G, int ind , int first_player);

};


void print_moves(std::vector<Moves> MV);
bool are_similar(Card A, Card B);

#endif