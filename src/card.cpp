#include "../include/card.hpp"



Card::Card(){
    color = ' ';
    value = -1;
}

Card::Card(int V, char C): value(V), color(C) {}

Card::~Card(){}


void Card::print() const{
    if (color == 'A'){
        if (value == 0)
            std::cout << "Excuse";
        else if (value > 0 && value <= 21)
            std::cout << value;
        else{
            std::cerr << "erreur Card::print() : value (" << value <<") illogique\n";
            exit(1);
        }
    }
    else{
        if (value > 0 && value <= 10)
            std::cout << value;
        else {
            switch (value)
            {
            case VALET:
                std::cout << "V";
                break;
            case CAVALIER:
                std::cout << "C";
                break;
            case DAME:
                std::cout << "D";
                break;
            case ROI:
                std::cout << "R";
                break;
            
            default:
                std::cout << "?";
                break;
            }
        }
    }

    if (color == 'H')
        std::cout << RED << " ♥" << END_FORMAT;
    else if (color == 'D')
        std::cout << RED << " ♦" << END_FORMAT;
    else if (color == 'C')
        std::cout << " ♣";
    else if (color == 'S')
        std::cout << " ♠";
    else if (color == 'A' && value != 0)
        std::cout << GREEN << " █" << END_FORMAT;

}



bool operator==(const Card& A, const Card& B){
    return (A.color == B.color && A.value == B.value);
}
