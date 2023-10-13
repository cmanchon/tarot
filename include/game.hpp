#include "player.hpp"
#ifndef GAME_HPP
#define GAME_HPP


#define NB_POINTS_TOTAL 91
#define NB_MIN_PLAYERS 2
#define NB_MAX_PLAYERS 5
#define APPEL_ROI_BEFORE_ECART false

#define AI_MOVES true


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
        Moves GM;
    public:
        Game(){};
        Game(int nb_players);
        ~Game(){};

        void print_players() const;
        void print_chien() const;
        void print_jeu(int first_player = 0) const;
        void print_all_cards() const;

        bool is_preneur(int ind) const;

        int nb_players(){return (int)players.size();};
        Deck get_pli(){return jeu;};
        int get_players_id(int ind){return players[ind].get_id();};
        int jeu_points(){return jeu.count_points();};

        Card prompt_card() const;
        void appel_roi();
        int plis_winner(int first_player) const;
        bool is_move_possible(Card C, int player_id) const;

        void deal_cards();
        void undeal_cards();
        void sort_hands();

        void print_result(int score_def, int points_preneur) const;


        //AI
        int AI_choose_contrat(int ind);
        void AI_ecart();
        Card AI_play(int ind, int first_player);

        void start_game();
        void game();

};


#endif