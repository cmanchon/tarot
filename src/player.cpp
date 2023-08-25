#include "../include/player.hpp"

void Player::print(int show_numbers) const{
    std::cout << "Player " << BOLD << id << END_FORMAT << "\n";
    std::cout << BOLD << score << END_FORMAT << " points\n";
    if (show_numbers){
        std::cout << hand.size() << " cards in hand\n";
        std::cout << plis.size() << " cards in plis\n";
    }
    if (comb.contrat_type != PASSE)
        print_contrat();

}

void Player::print_hand(int show_number) const{
    if (show_number)
        std::cout << BOLD << hand.size() << END_FORMAT << " cards :\n";
    
    hand.print("\t");
}

void Player::print_contrat() const{
    switch (comb.contrat_type)
    {
    case PASSE:
        std::cout << "PASSE";
        break;
    case PRISE:
        std::cout << "PRISE";
        break;
    case GARDE:
        std::cout << "GARDE";
        break;
    case GARDE_SANS:
        std::cout << "GARDE SANS";
        break;
    case GARDE_CONTRE:
        std::cout << "GARDE_CONTRE";
        break;
    
    default:
        break;
    }
}



void Player::give_hand_card(Card C, Deck *receiver){
    hand.give_card(C, receiver);
}



void Player::contrat(int c){
    if (c < PASSE || c > GARDE_CONTRE){
        std::cerr << "erreur Player::contrat() : contract type(" << c << ") non valable\n";
        exit(1);
    }
    else if (c == PASSE)
        return;
    
    comb.contrat_type = c;
    switch (hand.nb_bouts())
    {
    case 0:
        comb.contrat_value = 56;
        break;
    case 1:
        comb.contrat_value = 51;
        break;
    case 2:
        comb.contrat_value = 41;
        break;
    case 3:
        comb.contrat_value = 36;
        break;
    
    default:
        break;
    }
}

int Player::check_contrat(int points, int nb_players){
    // returns the score of the other players (sign correct) 


    int score_tmp = points - comb.contrat_value + 25;

    //coefficient
    switch (comb.contrat_type)
    {
    case PRISE:
        score_tmp *= 1;
        break;
    case GARDE:
        score_tmp *= 2;
        break;
    case GARDE_SANS:
        score_tmp *= 4;
        break;
    case GARDE_CONTRE:
        score_tmp *= 6;
        break;
    default:
        break;
    }

    //poignée
    switch (comb.poignee)
    {
    case 1:
        score_tmp += 20;
        break;
    case 2:
        score_tmp += 30;
        break;
    case 3:
        score_tmp += 40;
        break;
    
    default:
        break;
    }

    //petit au bout
            //faudrait gérer quand c'est les autres joueurs qui le font.......
    int nb_atouts = 0;
    bool petit = 0;
    for (int i = plis.size()-1 ; i >= plis.size()-nb_players ; i--){
        if (plis.size() == 0)
            break;
        if (plis.get_card(i).get_value() == 1 && plis.get_card(i).get_color() == 'A')
            petit = 1;
        if (plis.get_card(i).get_color() == 'A')
            nb_atouts++;
    }

    if (petit && nb_atouts == 1){
        switch (comb.contrat_type)
        {
        case PRISE:
            score_tmp += 10;
            break;
        case GARDE:
            score_tmp += 20;
            break;
        case GARDE_SANS:
            score_tmp += 40;
            break;
        case GARDE_CONTRE:
            score_tmp += 60;
            break;
        default:
            break;
        }

        comb.petit_au_bout = 1;
    }

    //chelem
    bool chelem = false;
    if (plis.size() == 78)
        chelem = true;
    

    for (int i = 78-nb_players-1 ; i >= 78-2*nb_players ; i--){
        if (i >= plis.size())
            break;
        if (plis.get_card(i).get_value() == 0){
            chelem = true;
            break;
        }
    }


    //on check si le contrat a changé au cours de la partie (ie si le petit des défenseurs a été volé)
    switch (plis.nb_bouts()){
        case 0:
            comb.contrat_value = 56;
            break;
        case 1:
            comb.contrat_value = 51;
            break;
        case 2:
            comb.contrat_value = 41;
            break;
        case 3:
            comb.contrat_value = 36;
            break;
        
        default:
            break;
    }   

    if (points >= comb.contrat_value){
        //win
        score += 3*score_tmp;
        if (chelem && comb.chelem){
            score += 400;
            comb.chelem = 11;
        }
        if (chelem && !comb.chelem){
            score += 200;
            comb.chelem = 12;
        }
        if (!chelem && comb.chelem){
            score -= 200;
            comb.chelem = 10;
        }
        return -score_tmp;
    }
    else{
        if (!chelem && comb.chelem)
            score -= 200;
        score -= 3*score_tmp;
        return score_tmp;
    }

}


void Player::reset_comb(){
    comb.contrat_type = PASSE;
    comb.contrat_value = 0;
    comb.chelem = 0;
    comb.poignee = 0;
    comb.petit_au_bout = false;
}