#include "../include/deck.hpp"

Deck::Deck(Card C){
    deck.push_back(C);
}

Deck::Deck(std::string filename){
    std::ifstream file(filename);

    if (!file.is_open()){
        std::cerr << "error Deck::Deck(std::string filename) : error openening file" << filename <<"\n";
        exit(1);
    } 

    file.seekg(0);

    while (file.peek() != std::ifstream::traits_type::eof()){
        int value;
        char color;
        file >> value >> color;
        deck.push_back({value, color});
    }

}


void Deck::print(std::string separator, int start, int end) const{
    if (end == -1) end = (int)deck.size();
    for (int i = start ; i < end ; i++){
        deck[i].print();
        std::cout << separator;
    }
}



int Deck::nb_bouts() const{
    int bouts = 0; 
    
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (deck[i].get_color() == 'A' && (deck[i].get_value() == 0 || deck[i].get_value() == 1 || deck[i].get_value() == 21))
            bouts++;
    }

    return bouts;
}


int Deck::nb_atouts() const{
    int atouts = 0; 
    
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (deck[i].get_color() == 'A')
            atouts++;
    }

    return atouts;
}


bool Deck::is_in(Card C) const{
    if (C.get_color() == ' ')
        return false;
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (C.get_value() == deck[i].get_value() && C.get_color() == deck[i].get_color()){
            return true;
        }
    }
    return false;
}


bool Deck::has_color(char color) const{
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (color == deck[i].get_color()){
            return true;
        }
    }
    return false;

}

bool Deck::has_atout_higher_than(int value) const{
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (deck[i].get_color() == 'A' && deck[i].get_value() > value){
            return true;
        }
    }
    return false;

}

int Deck::highest_atout() const{
    int value = -1;
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (deck[i].get_color() == 'A' && deck[i].get_value() != 0 && deck[i].get_value() > value)
            value = deck[i].get_value();
    }
    return value;
}




void Deck::add_card(Card C){
    deck.push_back({C.get_value(), C.get_color()});
}

void Deck::give_card(Deck *receiver){
    if (deck.size() > 0){
        receiver->add_card(deck[deck.size()-1]);
        deck.pop_back();
    }
}

void Deck::give_card(Deck *receiver, int start, int end){
    if (deck.size() - (end-start) >= 0){
        for (int i = start ; i < end ; i++){
            receiver->add_card(deck[start]);
            deck.erase(deck.begin() + start);
        }
    }
}


void Deck::give_card(Card C, Deck *receiver){
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (C.get_value() == deck[i].get_value() && C.get_color() == deck[i].get_color()){
            receiver->add_card(deck[i]);
            deck.erase(deck.begin() + i);
            return;
        }
    }
}

void Deck::give_all_cards(Deck *receiver){
    while (deck.size() > 0){
        give_card(receiver);
    }
}



void Deck::change_card(int index, Card C){
    if (index < (int)deck.size())
        deck[index] = C;
}


void Deck::shuffle(){
    Deck tmp;
    int rng;
    srand((unsigned) time(NULL));

    while (deck.size() > 0){
        rng = rand() % deck.size();
        tmp.deck.push_back({deck[rng].get_value(), deck[rng].get_color()});
        deck.erase(deck.begin() + rng);
        
    }

    for (long unsigned int i = 0 ; i < tmp.deck.size() ; i++){
        deck.push_back({tmp.deck[i].get_value(), tmp.deck[i].get_color()});
    }
}

void Deck::sort(){
    Deck clubs, diamonds, spades, hearts, atouts;

    while (deck.size() > 0){
        if (deck[deck.size()-1].get_color() == 'C')
            give_card(&clubs);
        else if (deck[deck.size()-1].get_color() == 'D')
            give_card(&diamonds);
        else if (deck[deck.size()-1].get_color() == 'S')
            give_card(&spades);
        else if (deck[deck.size()-1].get_color() == 'H')
            give_card(&hearts);
        else if (deck[deck.size()-1].get_color() == 'A')
            give_card(&atouts);
        else{
            std::cerr << "error Deck::sort() : deck[" << deck.size()-1 << "].color = " << deck[deck.size()-1].get_color() << "\n";
            exit(1);
        }
    }

    //CLUBS
    bool sorted = false;
    do {
        sorted = true;
        for (int i = 0 ; i < clubs.size() - 1 ; i++){
            if (clubs.get_card(i+1).get_value() <= clubs.get_card(i).get_value())
                continue;
            sorted = false;
            Card tmp = clubs.get_card(i+1);
            clubs.change_card(i+1, clubs.get_card(i));
            clubs.change_card(i, tmp);

        }
    } while(!sorted);
    clubs.give_all_cards(this);

    //DIAMONDS
    sorted = false;
    do {
        sorted = true;
        for (int i = 0 ; i < diamonds.size() - 1 ; i++){
            if (diamonds.get_card(i+1).get_value() <= diamonds.get_card(i).get_value())
                continue;
            sorted = false;
            Card tmp = diamonds.get_card(i+1);
            diamonds.change_card(i+1, diamonds.get_card(i));
            diamonds.change_card(i, tmp);

        }
    } while(!sorted);
    diamonds.give_all_cards(this);

    //SPADES
    sorted = false;
    do {
        sorted = true;
        for (int i = 0 ; i < spades.size() - 1 ; i++){
            if (spades.get_card(i+1).get_value() <= spades.get_card(i).get_value())
                continue;
            sorted = false;
            Card tmp = spades.get_card(i+1);
            spades.change_card(i+1, spades.get_card(i));
            spades.change_card(i, tmp);

        }
    } while(!sorted);
    spades.give_all_cards(this);

    //HEARTS
    sorted = false;
    do {
        sorted = true;
        for (int i = 0 ; i < hearts.size() - 1 ; i++){
            if (hearts.get_card(i+1).get_value() <= hearts.get_card(i).get_value())
                continue;
            sorted = false;
            Card tmp = hearts.get_card(i+1);
            hearts.change_card(i+1, hearts.get_card(i));
            hearts.change_card(i, tmp);

        }
    } while(!sorted);
    hearts.give_all_cards(this);

    //ATOUTS
    sorted = false;
    do {
        sorted = true;
        for (int i = 0 ; i < atouts.size() - 1 ; i++){
            if (atouts.get_card(i+1).get_value() <= atouts.get_card(i).get_value())
                continue;
            sorted = false;
            Card tmp = atouts.get_card(i+1);
            atouts.change_card(i+1, atouts.get_card(i));
            atouts.change_card(i, tmp);

        }
    } while(!sorted);
    atouts.give_all_cards(this);

}

int Deck::count_points() const{
    float points = 0;
    for (int i = 0 ; i < (int)deck.size() ; i++){
        if (deck[i].get_color() == 'A'){
            if (deck[i].get_value() == 0 || deck[i].get_value() == 1 || deck[i].get_value() == 21)
                points += 4.5;
            else
                points += 0.5;
        }
        else{
            switch (deck[i].get_value())
            {
            case VALET:
                points += 1.5;
                break;
            case CAVALIER:
                points += 2.5;
                break;
            case DAME:
                points += 3.5;
                break;
            case ROI:
                points += 4.5;
                break;
            
            default:
                points += 0.5;
                break;
            }
        }
    }

    return (int) points;
}