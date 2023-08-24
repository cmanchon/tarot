#include "player.hpp"
#ifndef GAME_HPP
#define GAME_HPP

#define NB_MIN_PLAYERS 3
#define NB_MAX_PLAYERS 5


class Game{
    private:
        std::vector <Player> players;
        int id_preneur = -1;
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
        int plis_winner(int first_player) const;

        void deal_cards();
        void sort_hands();

        void print_result(int score_def, int points_preneur) const;

        void start_game();
        void game();

};


#endif