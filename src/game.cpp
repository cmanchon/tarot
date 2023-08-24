#include "../include/game.hpp"

Game::Game(int nb_players){
    for (int i = 1 ; i <= nb_players ; i++){
        players.push_back(Player(i));
    }
}

void Game::print_players() const{
    for (int i = 0 ; i < (int) players.size() ; i++){
        players[i].print();
        std::cout << "\n";
    }
}

void Game::print_chien() const{
    chien.print("\t");
}

void Game::print_jeu(int first_player) const{
    int j = first_player;
    for (int i = 0 ; i < jeu.size() ; i++){
        std::cout << "Player " << BOLD;
        if (j == (int)players.size())
            j = 0;
        else if (j == first_player && i != 0){
            std::cerr << "error Game::print_jeu() : too many cards in jeu\n";
            exit(1);
        }
        std::cout << j+1 << END_FORMAT << " played\n";
        j++;

        jeu.get_card(i).print();

        std::cout << "\n\n";
    }
}

void Game::print_all_cards() const{
    std::cout << "Deck\t\t\t" << BOLD << deck.size() << END_FORMAT << " cards\n";
    deck.print("\t");

    std::cout << "Chien\t\t\t" << BOLD << chien.size() << END_FORMAT << " cards\n";
    chien.print("\t");

    std::cout << "\n\n";

    for (int i = 0 ; i < (int) players.size() ; i++){
        players[i].print_hand(1);
        std::cout << "\n"; 
    }
}

Card Game::prompt_card() const{
    std::string prompt;
    std::getline(std::cin, prompt);
    if (prompt == "Excuse" || prompt == "excuse")
        return {0, 'A'};
    int value;
    char color;

    for (int i = 0 ; i < (int)prompt.size() ; i++){
        if (prompt[i] ==  ' ')
            prompt.erase(prompt.begin() + i);
    }

    color = prompt[prompt.size()-1];

    prompt.pop_back();

    if (prompt.size() == 0){
        std::cerr << "error prompt_card : prompt of size 0\n";
        exit(1);
    }
    else if (prompt == "V")
        value = 11;
    else if (prompt == "C")
        value = 12;
    else if (prompt == "D")
        value = 13;
    else if (prompt == "R")
        value = 14;
    else
        value = std::stoi(prompt);

    return {value, color};
}


int Game::plis_winner(int first_player) const{
    if (jeu.size() == 0)
        return -1;
    int value = jeu.get_card(0).get_value();
    char color = jeu.get_card(0).get_color();
    int res = first_player;
    for (int i = 0 ; i < jeu.size() ; i++){
        if ((jeu.get_card(i).get_value() > value && color != 'A' && jeu.get_card(i).get_color() == color) 
        || (color == 'A' && jeu.get_card(i).get_color() == 'A' && value < jeu.get_card(i).get_value())){
            value = jeu.get_card(i).get_value();
            color = jeu.get_card(i).get_color();
            res = (first_player + i) % players.size();
        }
    }

    return res;
}


void Game::deal_cards(){
    int chien_max_size = 6;
    int deal_by = 3;

    if (players.size() == 3)
        deal_by = 4;
    else if (players.size() == 5)
        chien_max_size = 3;
    else if (players.size() == 2)
        chien_max_size = 6;

    while (deck.size() > 0){
        for (int i = 0 ; i < (int)players.size() ; i++){
            for (int j = 0 ; j < deal_by ; j++){
                deck.give_card(players[i].get_hand());
            }
        }
        if (chien.size() < chien_max_size)
            deck.give_card(&chien);
    }
}

void Game::sort_hands(){
    for (int i = 0 ; i < (int) players.size() ; i++){
        players[i].sort_hand();
    }
}

void Game::print_result(int score_def, int points_preneur) const{
    std::cout << CLEAR;

    std::cout << "Player " << BOLD << id_preneur+1 << END_FORMAT;
    if (score_def < 0){
        std::cout << " a réussi son contrat :\n\n";
    }
    if (score_def > 0){
        std::cout << " n'a pas réussi son contrat :\n\n";
    }

    std::cout << BOLD << "\t" << points_preneur << END_FORMAT << " points remportés (" << players[id_preneur].get_comb().contrat_value << "points requis)\n";
    std::cout << "\t+ 25\n";
    if (players[id_preneur].get_comb().petit_au_bout){
        std::cout << "\t+ 10\t\t petit au bout\n";
        points_preneur += 10;
    }
    std::cout << "= " << points_preneur + 25 << "\n";
    switch (players[id_preneur].get_comb().contrat_type){
    case PRISE:
        std::cout <<"\t* 1\t\tprise\n";
        break;
    case GARDE:
        std::cout <<"\t* 2\t\tgarde\n";
        break;
    case GARDE_SANS:
        std::cout <<"\t* 4\t\tgarde sans\n";
        break;
    case GARDE_CONTRE:
        std::cout <<"\t* 6\t\tgarde contre\n";
        break;
    default:
        break;
    } 


    switch (players[id_preneur].get_comb().poignee){
    case 1:
        std::cout <<"\t+ 20\t\tpoignée\n";
        break;
    case 2:
        std::cout <<"\t+ 30\t\tpoignée\n";
        break;
    case 3:
        std::cout <<"\t+ 40\t\tpoignée\n";
        break;
    default:
        break;
    } 

    switch (players[id_preneur].get_comb().chelem){
    case 11:
        std::cout <<"\t+ 400\t\tchelem annoncé et réalisé\n";
        break;
    case 10:
        std::cout <<"\t- 200\t\tchelem annnoncé et non réalisé\n";
        break;
    case 12:
        std::cout <<"\t+ 200\t\tchelem non annoncé et réalisé\n";
        break;
    default:
        break;
    } 

    std::cout << "\n\nTOTAL = " << -score_def;
    std::cout << "\n\n\nLes autres joueurs gagnent " << score_def << " points.\n\n";


}

void Game::start_game(){
    std::cout << "########## TAROT ##########\n\n";
    if (players.size() < NB_MIN_PLAYERS){
        int nb_players = 0;
        while (nb_players < NB_MIN_PLAYERS || nb_players > NB_MAX_PLAYERS){
            std::cout << "Combien de joueurs ?\n";
            std::cin >> nb_players;
        }
        for (int i = 1 ; i <= nb_players ; i++){
            players.push_back(Player(i));
        }
    }
    deck.shuffle();
    deal_cards();

    //chaque joueur check son jeu et voit si il veut prendre
    int choix, highest_choix = PASSE, ind_highest = -1;
    for (int i = 0 ; i < (int)players.size() ; i++){
        do {
            std::cout << CLEAR;
            players[i].print();
            players[i].print_hand();

            std::cout << "\n\nQue faire ?\n";
            std::cout << "  0\t  1\t  2\t  3\t\t  4\n";
            std::cout << "PASSER\tPRISE\tGARDE\tGARDE SANS\tGARDE CONTRE\n\nChoix : ";
            std::cin >> choix;

        }while((choix <= highest_choix && choix != 0) || choix > GARDE_CONTRE);

        
        if (highest_choix < choix){
            highest_choix = choix;
            ind_highest = i;
        }

        if (i == (int)players.size()-1){
            if (ind_highest != -1){
                players[ind_highest].contrat(highest_choix);
            }
            else{
                std::cout << CLEAR;
                std::cout << "Aucun joueur n'a pris.\n\nLe chien :\n";
                chien.print("\t");
                sleep(SLEEPTIME/3);
                std::cout << "\n\nNouveau mélange.\n";
                sleep(SLEEPTIME);

                deck.shuffle();
                deal_cards();
                i = -1;
                continue;
            }

        }
    }

    //on vide le buffer
    std::string buff;
    std::getline(std::cin, buff);


    id_preneur = ind_highest;

    //chien à gérer pour garde sans et contre

    std::cout << CLEAR;
    std::cout << "Le joueur " << BOLD << id_preneur+1 << END_FORMAT << " a pris une ";
    players[id_preneur].print_contrat();
    sort_hands();
    std::cout << ".\nIl obtient le chien :\n";
    chien.print("\t"); 
    int ecart_size = (int)chien.size();
    std::cout << "\n\net il doit faire un écart de " << ecart_size << " cartes.\n\n";
    chien.give_all_cards(players[id_preneur].get_hand());
    players[id_preneur].sort_hand();
    sleep(SLEEPTIME*2);


    int l = 0;
    while (players[id_preneur].size_plis() <= ecart_size){
        std::cout << CLEAR;
        players[id_preneur].print();
        std::cout << "\n\n";

        std::cout << "Écart :\n";
        if (players[id_preneur].size_plis() == 0)
            std::cout << "(vide)";
        players[id_preneur].get_plis()->print("\t");
        std::cout << "\n\n";
        players[id_preneur].print_hand();

        if (players[id_preneur].size_plis() < ecart_size){
            std::cout << "\n\n\nSélectionner " << ecart_size << " cartes à mettre dans l'écart :\n";
            Card C = prompt_card();
            if ((C.get_value() == 14 && C.get_color() != 'A') || (C.get_color() == 'A' && (C.get_value() == 0 || C.get_value() == 1 || C.get_value() == 21)))       //rois et bouts interdits dans l'écart
                continue;
            if (players[id_preneur].get_hand()->is_in(C)){
                players[id_preneur].get_hand()->give_card(C, players[id_preneur].get_plis());
            }
            else if (players[id_preneur].get_plis()->is_in(C)){
                players[id_preneur].get_plis()->give_card(C, players[id_preneur].get_hand());
            }

        }
        else{
            std::cout << "\n\n\nValider cet écart ? (Y/N)\n";
            char tmp;
            std::cin >> tmp;
            if (tmp == 'Y')
                break;
            else if (tmp == 'N')
                continue;
            else
                return;
        }

        //safety
        l++;
        if (l > 15){
            std::cerr << "error Game::start_game() : too many tries\n";
            return;
        }
    }

    std::cout << CLEAR;
    std::cout << "Le preneur a fait son écart, la partie peut commencer.\n\n";
    sleep(SLEEPTIME);

    //on vide le buffer
    std::getline(std::cin, buff);

}


void Game::game(){
    srand((unsigned) time(NULL));
    int first_player = rand() % players.size();
    //si chelem -> preneur joue en premier

    /* 78 cartes

    5 joueurs : 3 dans l'écart donc 75 en jeu -> 15 plis
    4 joueurs : 6 dans l'écart donc 72 en jeu -> 18 plis
    3 joueurs : 6 dans l'écart donc 72 en jeu -> 24 plis 
    2 joueurs : 12 dans l'écart donc 66 en jeu -> 33 plis */

    int nb_plis_total;
    switch (players.size())
    {
    case 5:
        nb_plis_total = 15;
        break;
    case 4:
        nb_plis_total = 18;
        break;
    case 3:
        nb_plis_total = 24;
        break;
    case 2:
        nb_plis_total = 33;
        break;
    default:
        break;
    }

    Card C;
    int l = 0;
    int j;
    for (int i = 0 ; i < nb_plis_total ; i++){
        l = 0;
        j = first_player;
        while (j != first_player || (j == first_player && l == 0) ){
            l = 1;

            if (j >= (int) players.size()){
                j = 0;
                if (first_player == j)
                    break;
            }

            while (!players[j].get_hand()->is_in(C) && l < 20){           //faut aussi check si cette carte peut être jouer...... à faire dans une méthode ig
                std::cout << CLEAR;
                players[j].print(1);
                std::cout << "\n\n";

                print_jeu(first_player);
                std::cout << "\n\n\n";

                players[j].print_hand();
                std::cout << "\nJouer la carte : ";
                C = prompt_card();

                l++;
            }   

            players[j].give_hand_card(C, &jeu);
            j++;
        }

        first_player = plis_winner(first_player);
        jeu.give_all_cards(players[first_player].get_plis());

    }

    int score_preneur = players[id_preneur].count_score();

    int score_def = players[id_preneur].check_contrat(score_preneur, players.size());
    
    // on donne les points aux non preneurs
    for (int i = 0 ; i < (int)players.size() ; i++){
        if (i == id_preneur)
            continue;
        players[i].add_score(score_def);
    }

    //PRINT RANKING

    print_result(score_def, score_preneur);
    sleep(SLEEPTIME);

    for (int i = 0 ; i < (int)players.size() ; i++){
        players[i].reset_comb();
    }

}