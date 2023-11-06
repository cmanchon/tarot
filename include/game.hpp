#include "player.hpp"
#include "ai.hpp"
#ifndef GAME_HPP
#define GAME_HPP


#define NB_POINTS_TOTAL 91
#define NB_MIN_PLAYERS 2
#define NB_MAX_PLAYERS 5
#define APPEL_ROI_BEFORE_ECART false

#define AI_MOVES true
#define MOVES_FILE "src/moves.csv"


class Game{
    private:
        std::vector <Player> players;
        union{
            int id_preneur = -1;
            int id_preneurs[2];
        };
        Deck deck = Deck(DFL_DECK);
        Deck chien;
        Deck jeu;
    public:
        Game(){};
        Game(int nb_players);
        ~Game(){};

        void print_players() const;
        void print_chien() const;
        void print_jeu(int first_player = 0) const;
        void print_all_cards() const;

        bool is_preneur(int ind) const;
        bool are_mates(int ind1, int ind2) const{return is_preneur(ind1) == is_preneur(ind2);};

        int nb_players()const {return (int)players.size();};
        int nb_AI() const;
        bool is_AI(int ind) const;
        Deck get_pli(){return jeu;};
        int get_players_id(int ind)const {return players[ind].get_id();};
        int get_players_ind(int id)const;
        Card get_players_card(int player_ind, int card_ind){return players[player_ind].get_hand()->get_card(card_ind);};
        int get_players_hand_size(int player_ind){return players[player_ind].get_hand()->size();};
        int jeu_points(){return jeu.count_points();};

        Card prompt_card() const;
        void appel_roi();
        int pli_winner(int first_player) const{return jeu.winner(first_player);};
        bool is_move_possible(Card C, int player_id) const;

        void deal_cards();
        void undeal_cards();
        void sort_hands();
        void jeu_to_players_plis(int ind, int ind_excuse);

        void print_result(int score_def, int points_preneur) const;


        //AI
        int AI_choose_contrat(int ind);
        void AI_ecart();
        Card AI_play(int ind, int first_player);

        void start_game();
        void game();

};


#endif