#include "../include/game.hpp"


Game::Game(int nb_players){
    for (int i = 1 ; i <= nb_players ; i++){
        players.push_back(Player(i));
    }

    id_preneurs[0] = -1;
    id_preneurs[1] = -1;
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
        std::cout << get_players_id(j) << END_FORMAT << " played\n";
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

bool Game::is_preneur(int ind) const{
    if (id_preneur == ind || id_preneurs[1] == ind)
        return true;
    else 
        return false;
}


int Game::nb_AI() const{
    int res = 0;
    for (int i = 0 ; i < nb_players() ; i++){
        if (get_players_id(i) >= ID_AI)
            res++;
    }
    return res;
}

bool Game::is_AI(int ind) const{
    if (get_players_id(ind) >= ID_AI)
        return true;
    else
        return false;
}


int Game::get_players_ind(int id)const{
    for (int i = 0 ; i < (int)players.size() ; i++){
        if (players[i].get_id() == id)
            return i;
    }

    return -1;
}




Card Game::prompt_card() const{
    std::string prompt;
    std::getline(std::cin, prompt);
    if (prompt == "Q")
        exit(1);
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
        // std::cerr << "error prompt_card : prompt of size 0\n";
        // exit(1);
        return {-1, ' '};
    }
    else if (prompt == "V")
        value = 11;
    else if (prompt == "C")
        value = 12;
    else if (prompt == "D")
        value = 13;
    else if (prompt == "R")
        value = 14;
    else{
        for (int i = 0 ; i < (int)prompt.size() ; i++){
            if (!std::isdigit(prompt[i]))
                return {-1, ' '};
        }
        value = std::stoi(prompt);
    }


    return {value, color};
}


void Game::appel_roi(){
    if (players.size() != 5)
        return;
    
    std::cout << CLEAR;
    std::cout << "Le preneur doit appeler un Roi, son propriétaire fera équipe avec le preneur.\n\n";
    sleep(SLEEPTIME/3);
    if (players[id_preneur].get_id() < ID_AI)
        players[id_preneur].print_hand();
    std::cout << "\n\n\nRAPPELS :\n\tIl est possible d'appeler un Roi qui est dans la main du preneur, mais ce dernier jouera donc seul.\n\tSi le preneur détient tous les Rois, il doit appeler une Dame.\n\n";

    Card C;
    if (players[id_preneur].get_id() >= ID_AI){
        Deck choix;
        int value = ROI;
        while (choix.size() == 0){
            if (!players[id_preneur].get_hand()->is_in({value, 'H'}))
                choix.add_card({value, 'H'});
            if (!players[id_preneur].get_hand()->is_in({value, 'D'}))
                choix.add_card({value, 'D'});
            if (!players[id_preneur].get_hand()->is_in({value, 'S'}))
                choix.add_card({value, 'S'});
            if (!players[id_preneur].get_hand()->is_in({value, 'C'}))
                choix.add_card({value, 'C'});
            value--;
        }

        srand((unsigned) time(NULL));
        int rng = rand() % choix.size();

        C = choix.get_card(rng);

    }
    else{

        do{
            std::cout << "\nLe preneur appelle : ";
            C = prompt_card();
        }while(C.get_color() == 'A' || C.get_value() < 12);

    }

    //on va chercher le joueur qui a cette carte
    for (int i = 0 ; i < (int)players.size() ; i++){
        if (i == id_preneur)
            continue;
        for (int j = 0 ; j < (int)players[i].get_hand()->size() ; j++){
            if (players[i].get_hand()->get_card(j).get_value() == C.get_value() && players[i].get_hand()->get_card(j).get_color() == C.get_color()){
                id_preneurs[1] = i;
                i = NB_MAX_PLAYERS+1;
                break;
            }
        }
    }

    sleep(SLEEPTIME*2);
    std::cout << CLEAR;
    std::cout << "Player " << BOLD << players[id_preneur].get_id() << END_FORMAT << " a choisi d'appeler ";
    C.print();
    std::cout << "\n\n\n";

    sleep(SLEEPTIME*2);
}




bool Game::is_move_possible(Card C, int player_id) const{
    if (C.get_color() == ' ')                   //non initialisée
        return false;
    if (jeu.size() == 0)
        return true;
    if (C.get_value() == 0 && C.get_color() == 'A')
        return true;

    char color;
    if (jeu.get_card(0).get_value() != 0)
        color = jeu.get_card(0).get_color();
    else{       //première carte du jeu est l'excuse
        if (jeu.size() == 1)
            return true;
        else
            color = jeu.get_card(1).get_color();
    }
    int highest_atout = jeu.highest_atout();

    if (color == 'A'){
        if (C.get_color() == 'A'){
            if (C.get_value() > highest_atout)
                return true;
            else{
                if (players[player_id].has_atout_higher_than(highest_atout))
                    return false;
                else
                    return true;
            }
        }
        else{
            if (players[player_id].has_atout_higher_than(0))            //on compte pas l'excuse
                return false;
            else
                return true;
        }
    } 
    else{
        if (color == C.get_color())
            return true;
        else{
            if (C.get_color() == 'A'){
                if (players[player_id].has_color_in_hand(color))
                    return false;
                else{
                    if (C.get_value() > highest_atout)
                        return true;
                    else{
                        if (players[player_id].has_atout_higher_than(highest_atout))
                            return false;
                        else
                            return true;
                    }
                }
                    
            }
            else{
                if (players[player_id].has_atout_higher_than(0))            //on compte pas l'excuse
                    return false;
                else 
                    return true;
            }
        }
    }

    return true;

}



void Game::deal_cards(){
    int chien_max_size = 6;
    int deal_by = 3;

    if (players.size() == 3)
        deal_by = 4;
    else if (players.size() == 5)
        chien_max_size = 3;
    else if (players.size() == 2){
        chien_max_size = 12;
        deal_by = 2;
    }

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

void Game::undeal_cards(){
    for (int i = 0 ; i < (int)players.size() ; i++){
        players[i].get_hand()->give_all_cards(&deck);
        players[i].get_plis()->give_all_cards(&deck);
    }
    chien.give_all_cards(&deck);
}

void Game::sort_hands(){
    for (int i = 0 ; i < (int) players.size() ; i++){
        players[i].sort_hand();
    }
}

void Game::print_result(int score_def, int points_preneur) const{
    std::cout << CLEAR;

    std::cout << "Player " << BOLD << players[id_preneur].get_id() << END_FORMAT;
    if (score_def < 0){
        std::cout << " a réussi son contrat :\n\n";
    }
    if (score_def > 0){
        std::cout << " n'a pas réussi son contrat :\n\n";
    }

    std::cout << BOLD << "\t" << points_preneur << END_FORMAT << " points remportés (" << players[id_preneur].get_comb().contrat_value << " points requis)\n\n";
    std::cout << "\t+ 25\n";
    if (players[id_preneur].get_comb().petit_au_bout){
        std::cout << "\t+ 10\t\t petit au bout\n";
        points_preneur += 10;
    }
    std::cout << "= " <<players[id_preneur].get_comb().contrat_value - points_preneur + 25 << "\n";
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
    if (id_preneurs[1] != -1)
        std::cout << "Player " << BOLD << id_preneurs[1]+1 << END_FORMAT << "étant en équipe avec le preneur, il "<< (score_def > 0 ? "perd" : "gagne") << " aussi  " << BOLD << -score_def << END_FORMAT << " points.\n\n";
    std::cout << "\n\n\nLes autres joueurs " << (score_def > 0 ? "gagnent" : "perdent") << " " << score_def << " points.\n\n";

}


void Game::jeu_to_players_plis(int ind, int ind_excuse){
    while (!jeu.is_empty()){
        if (jeu.get_card(jeu.size()-1) == (Card){0, 'A'}){      //excuse
            jeu.give_card(players[ind_excuse].get_hand());
            int i = 0;
            while (players[ind_excuse].get_hand()->get_card(i).get_color() == 'A' || players[ind_excuse].get_hand()->get_card(i).get_value() >= 10)
                i++;
            players[ind_excuse].get_hand()->give_card(&jeu, i, i+1);
        }
        else
            jeu.give_card(players[ind].get_hand());
    }
}





void Game::start_game(){
    std::cout << "########## TAROT ##########\n\n";
    if (players.size() < NB_MIN_PLAYERS){
        int nb_players = 0;
        while (nb_players < NB_MIN_PLAYERS || nb_players > NB_MAX_PLAYERS){
            std::cout << "Tarot à combien de joueurs ?\n";
            std::cin >> nb_players;
            FLUSH_CIN;
        }

        

        std::cout << "\n\n";
        int nb_AI = -1;
        while (nb_AI < 0 || nb_AI > nb_players){
            std::cout << "Dont combien de IA ?\n";
            std::cin >> nb_AI;
            FLUSH_CIN;
        }
        for (int i = 1 ; i <= nb_players-nb_AI ; i++){
            players.push_back(Player(i));
        }
        for (int i = ID_AI ; i < nb_AI+ID_AI ; i++){
            players.push_back(Player(i));
        }
    }
    deck.shuffle();
    deal_cards();

    //chaque joueur check son jeu et voit si il veut prendre
    int choix, highest_choix = PASSE, ind_highest = -1;
    for (int i = 0 ; i < (int)players.size() ; i++){
        do {
            std::cout << CLEAR,
            players[i].print();
            if (players[i].get_id() >= ID_AI){
                
                // players[i].print_hand();

                choix = AI_choose_contrat(i);
                std::cout << "\n\nLe joueur " << players[i].get_id() << " (IA) a choisi une ";
                players[i].print_contrat(choix);
                std::cout << ".\n\n";
                sleep(SLEEPTIME);
                break;
            }
            else{
                players[i].print_hand();

                if (highest_choix > PASSE){
                    std::cout << "\n\nLe joueur " << BOLD << ind_highest+1 << END_FORMAT << " a pris une ";
                    players[ind_highest].print_contrat(highest_choix);
                    std::cout << ".";
                }

                std::cout << "\n\nQue faire ?\n";
                std::cout << "  0\t  1\t  2\t  3\t\t  4\n";
                std::cout << "PASSER\tPRISE\tGARDE\tGARDE SANS\tGARDE CONTRE\n\nChoix : ";
                std::cin >> choix;
                FLUSH_CIN;
            }

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

                undeal_cards();
                deck.shuffle();
                deal_cards();
                i = -1;
                continue;
            }

        }
    }


    id_preneur = ind_highest;
    id_preneurs[1] = -1;


    std::cout << CLEAR;
    std::cout << "Le joueur " << BOLD << players[id_preneur].get_id() << END_FORMAT << " a pris une ";
    players[id_preneur].print_contrat();
    std::cout << ".\n";
    sort_hands();

    if (players.size() == 5 && APPEL_ROI_BEFORE_ECART){
        appel_roi();
    }


    int l = 0;
    int ecart_size = (int)chien.size();
    if (players.size() == 2)
        ecart_size = 6;
    if (highest_choix == PRISE || highest_choix == GARDE){
        if (players.size() == 2){
            //le preneur choisit le chien sans le voir
            std::cout << "\nLe preneur a le choix entre 2 chiens :\n\n";
            std::cout << "\t\t1\t2\n";
            std::cout << "\t█ █ █ █ █ █\t█ █ █ █ █ █\n\n\n";
            int chien_num = 0;

            if (players[id_preneur].get_id() >= ID_AI)
                chien_num = 1;                          //on pourrait choisir de manière random mais en soi c'est déjà random grâce au shuffle
            else{
                std::cout << "Chien numéro : ";
                while (chien_num < 1 || chien_num > 2){
                    std::cin >> chien_num;
                    FLUSH_CIN;
                    std::cout << "\n";
                }

            }  

            std::cout << "Il obtient le chien :\n";
            switch (chien_num)
            {
            case 1:
                chien.print("\t", 0, 6);
                chien.give_card(players[id_preneur].get_hand(), 0, 6);
                break;
            case 2:
                chien.print("\t", 6, 12);
                chien.give_card(players[id_preneur].get_hand(), 6, 12);
                break;
            
            default:
                break;
            }
        }

        else{
            std::cout << "Il obtient le chien :\n";
            chien.print("\t"); 
            chien.give_all_cards(players[id_preneur].get_hand());
        }

        //ECART
        std::cout << "\n\net il doit faire un écart de " << ecart_size << " cartes.\n\n";
        players[id_preneur].sort_hand();

        sleep(SLEEPTIME*2);
        if (players[id_preneur].get_id() >= ID_AI){
            std::cout << "Le joueur " << BOLD << players[id_preneur].get_id() << END_FORMAT << " fait son écart...\n\n";
            AI_ecart();


            sleep(SLEEPTIME);
        }
        else{
            while ((players.size() != 2 && players[id_preneur].size_plis() <= ecart_size) || (players.size() == 2 && players[id_preneur].size_plis() <= ecart_size+6)){
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
                    if ((C.get_value() == ROI && C.get_color() != 'A') || (C.get_color() == 'A' && (C.get_value() == 0 || C.get_value() == 1 || C.get_value() == 21)))       //rois et bouts interdits dans l'écart
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
                    FLUSH_CIN;
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
                    exit(1);
                }
            }

        }
        sleep(SLEEPTIME);
        std::cout << CLEAR;
        if (players.size() != 5)
            std::cout << "Le preneur a fait son écart, la partie peut commencer.\n\n";
    }
    else if (highest_choix == GARDE_SANS){
        //chien dans les plis du preneur mais il fait pas d'écart
        std::cout << "\nLe chien est mis dans les plis du preneur. Il ne peut pas faire d'écart.\n";
        chien.give_all_cards(players[id_preneur].get_plis());
    }
    else if (highest_choix == GARDE_CONTRE){
        //chien dans les plis de la défense, du coup on peut les laisser dans chien pcq on compte que les points du preneur 
        std::cout << "\nLe chien est mis dans les plis de la défense. Le preneur ne peut pas faire d'écart.\n";
    }


    sleep(SLEEPTIME);
    
    if (players.size() == 5 && !APPEL_ROI_BEFORE_ECART){
        appel_roi();
        sleep(SLEEPTIME);
    }


}


void Game::game(){

    std::vector<Moves> MV;

    if (AI_MOVES){
        for (int i = 0 ; i < (int)players.size() ; i++){
            if (players[i].get_id() >= ID_AI){
                MV.push_back({(players[i].get_id()), MOVES_FILE});
            }
        }
    }
    
    while (true){
        deal_cards();
        sleep(SLEEPTIME);
        
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
        int ind_excuse = -1;

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

                if (get_players_id(j) >= ID_AI){            //ai
                    C = MV[get_players_id(j) - ID_AI].AI_play_ML(*this, j, first_player);
                }
                else{                                       //real player
                    do{
                        std::cout << CLEAR;
                        players[j].print(1);
                        std::cout << "\n\n";

                        print_jeu(first_player);
                        std::cout << "\n\n\n";

                        players[j].print_hand();
                        std::cout << "\nJouer la carte : ";
                        C = prompt_card();

                        l++;
                    } while ((!players[j].get_hand()->is_in(C) || !is_move_possible(C, j)) && l < 20);

                }
                if (C == (Card){0, 'A'})
                    ind_excuse = j;
                players[j].give_hand_card(C, &jeu);
                j++;
            }

            std::cout << CLEAR;
            players[j].print(1);
            std::cout << "\n\n";
            print_jeu(first_player);
            std::cout << "\n\n";
            sleep(SLEEPTIME);

            if (AI_MOVES){
                for (int k = 0 ; k < (int)MV.size() ; k++)
                    MV[k].add(MV[k].get_id(), *this, first_player);

            }

            first_player = pli_winner(first_player);
            jeu.give_all_cards(players[first_player].get_plis());
            jeu_to_players_plis(first_player, ind_excuse);

        }


        if (id_preneurs[1] != -1)
            players[id_preneurs[1]].get_plis()->give_all_cards(players[id_preneur].get_plis());

        int score_preneur = players[id_preneur].count_score();

        int score_def = players[id_preneur].check_contrat(score_preneur, players.size());
        
        // on donne les points aux non preneurs
        for (int i = 0 ; i < (int)players.size() ; i++){
            if (i == id_preneur)
                continue;
            else if (i == id_preneurs[1])
                players[i].add_score(-score_def);
            players[i].add_score(score_def);
        }

        std::cout << CLEAR;
        std::cout << "Fin de la partie.\n\n";

        if (players.size() == 2){
            std::cout << "Le chien qui n'a pas été pris par le preneur :\n\n";
            chien.print("\t");
        }

        sleep(SLEEPTIME*2);

        //PRINT RANKING

        print_result(score_def, score_preneur);
        sleep(SLEEPTIME);

        for (int i = 0 ; i < (int)players.size() ; i++){
            players[i].reset_comb();
        }

        char choice;
        if (nb_AI() == nb_players()){
                std::cout << "\nContinuer de jouer ? (Y/N) ";
                std::cin >> choice;
                if (choice == 'N' || choice == 'Q'){
                    break;
                }
        }
        else{
            for (int i = 0 ; i < nb_players() ; i++){
                if (!is_AI(i)){
                    std::cout << "\nPlayer " << BOLD << get_players_id(i) << END_FORMAT << " :\nContinuer de jouer ? (Y/N) ";
                    std::cin >> choice;
                    if (choice == 'N' || choice == 'Q'){
                        players.erase(players.begin() + i);
                        i--;
                    }
                }
            }

        }


        if (nb_players() < NB_MIN_PLAYERS)
            break;
        
        undeal_cards();
    }


    if (AI_MOVES){
        for (int i = 0 ; i < (int)MV.size() ; i++)
            MV[i].save_in_file(MOVES_FILE);
    }


}