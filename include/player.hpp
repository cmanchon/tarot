#include "deck.hpp"
#include <strings.h>

#ifndef PLAYER_HPP
#define PLAYER_HPP

#define PASSE 0
#define PRISE 1
#define GARDE 2
#define GARDE_SANS 3
#define GARDE_CONTRE 4


typedef struct Comb{
    int contrat_type = PASSE;                       //prise x1 ; garde x2 ; garde sans x4 ; garde contre x6
    int contrat_value;                              //0B = 56 ; 1B = 51 ; 2B = 41 ; 3B = 36
    int chelem = 0;                                  //= gagner tous les plis donc 78 cartes dans le deck plis du joueur (quoique non pcq l'excuse doit être jouée en dernier donc faut check si elle est dans plis); annoncé et réalisé = +400 points (11); annoncé et non réalisé = -200 points (10); non annoncé et réalisé = +200 points(12)   
    int poignee = 0;                                //1 = simple = 13/10/8 atouts = 20 points ; 2 = double = 15/13/10 atouts = 30 points ; 3 = triple = 18/15/13 atouts = 40 points
    bool petit_au_bout = false;
}Comb;


class Player{
    private:
        int id;
        int score;
        Deck hand;
        Deck plis;
        Comb comb;
    public:
        Player(): id(-1), score(0){};
        Player(int Id): id(Id), score(0){};
        ~Player(){};


        void print(int show_numbers = 0) const;
        void print_hand(int show_number = 0) const;
        void print_contrat() const;
        Deck* get_hand() {return &hand;};
        Deck* get_plis() {return &plis;};
        Comb get_comb() const {return comb;};
        int size_plis() const{return (int) plis.size();};
        int size_hand() const{return (int) hand.size();};
        int count_score() const{return plis.count_points();}
        bool has_color_in_hand(char color) const{return hand.has_color(color);};
        bool has_atout_higher_than(int value) const{return hand.has_atout_higher_than(value);};

        void sort_hand(){hand.sort();};
        void give_hand_card(Card C, Deck *receiver);

        void contrat(int c);
        int check_contrat(int points, int nb_players);
        void add_score(int n){score+=n;};
        void reset_comb();
};


#endif