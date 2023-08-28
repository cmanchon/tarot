#include "player.hpp"
#ifndef GAME_HPP
#define GAME_HPP


#define NB_POINTS_TOTAL 91
#define NB_MIN_PLAYERS 2
#define NB_MAX_PLAYERS 5
#define APPEL_ROI_BEFORE_ECART false


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

        Card prompt_card() const;
        void appel_roi();
        int plis_winner(int first_player) const;
        bool is_move_possible(Card C, int player_id) const;

        void deal_cards();
        void undeal_cards();
        void sort_hands();

        void print_result(int score_def, int points_preneur) const;


        //AI
        int AI_choose_contrat(int player_id);

        void start_game();
        void game();

};


#endif