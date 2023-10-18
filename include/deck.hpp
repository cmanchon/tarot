#include "card.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <time.h>

#define DFL_DECK "./src/default_deck.txt" 

#ifndef DECK_HPP
#define DECK_HPP

class Deck{
    private:
        std::vector<Card> deck;

    public:
        Deck(){};
        Deck(Card C);
        Deck(std::string filename);
        ~Deck(){};

        void print(std::string separator = "\n", int start = 0, int end = -1) const;
        int size() const{return (int)deck.size();};
        Card get_card(int index) const{return deck[index];};
        int nb_bouts() const;
        int nb_atouts() const;

        bool is_in(Card C) const;
        bool has_color(char color) const;
        bool has_atout_higher_than(int value) const;
        int highest_atout() const;

        void add_card(Card C);
        void give_card(Deck *receiver);
        void give_card(Deck *receiver, int start, int end);
        void give_card(Card C, Deck *receiver);
        void give_all_cards(Deck *receiver);
        void set_empty(){deck.clear();};
        void change_card(int index, Card C);
        void shuffle();
        void sort();
        int count_points() const;

};


#endif