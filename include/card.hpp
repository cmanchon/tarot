#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <string.h>
#include <unistd.h>     
#include <limits>
#include <ios>


#define VALET 11
#define CAVALIER 12
#define DAME 13
#define ROI 14


#define RED "\033[31m"
#define GREEN "\033[32m"
#define BOLD "\033[1;37m"
#define END_FORMAT "\033[0m"
#define CLEAR "\033[2J\033[H"
#define SLEEPTIME 0
#define FLUSH_CIN std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')

class Card{
    private:
        int value;          //1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 (valet), 12 (cavalier), 13 (dame), 14 (roi). 
        char color;         //'H', 'D', 'C', 'S', 'A'
                            //excuse = 0 ; A
    public:
        Card();
        Card(int V, char C);
        ~Card();

        void print() const;
        int get_value() const{return value;};
        char get_color() const{return color;};
        bool is_bout() const;


        friend bool operator==(const Card&, const Card&);
};


#endif