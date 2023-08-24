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

        void print(std::string separator = "\n") const;
        int size() const{return (int)deck.size();};
        Card get_card(int index) const{return deck[index];};
        int nb_bouts() const;

        bool is_in(Card C) const;

        void add_card(Card C);
        void give_card(Deck *receiver);
        void give_card(Card C, Deck *receiver);
        void give_all_cards(Deck *receiver);
        void change_card(int index, Card C);
        void shuffle();
        void sort();
        int count_points() const;

};


#endif