#include "../include/ai.hpp"


int Game::AI_choose_contrat(int ind){
    if (players[ind].get_id() < ID_AI || players[ind].size_hand() == 0)
        return -1;

    int nb_points = players[ind].get_hand()->count_points();
    int nb_points_moyen = (NB_POINTS_TOTAL-chien.count_points()) / players.size();
    int nb_points_moyen_plus10p = nb_points_moyen + (nb_points_moyen * 10) / 100;
    int nb_points_moyen_plus15p = nb_points_moyen + (nb_points_moyen * 15) / 100;
    int nb_bouts = players[ind].get_hand()->nb_bouts();
    int nb_atouts = players[ind].get_hand()->nb_atouts();
    int nb_atouts_moyen = 22 / players.size();

    if (nb_points < nb_points_moyen && nb_bouts < 2){
        if (nb_bouts < 2 || nb_atouts < nb_atouts_moyen)
            return PASSE;
        else
            return PRISE;
    }
    if (nb_points >= nb_points_moyen && nb_points < nb_points_moyen_plus10p){
        if (nb_bouts == 0)
            return PASSE;
        if (nb_bouts == 1)
            return PRISE;
        if (nb_bouts == 2)
            return GARDE;
    }
    if (nb_points >= nb_points_moyen_plus10p){
        if (nb_bouts >= 2)
            return GARDE_SANS;
        else
            return GARDE;
    }
    if (nb_points >= nb_points_moyen_plus15p){
        if (nb_bouts >= 2)
            return GARDE_CONTRE;
        else
            return GARDE_SANS;
    }
    if (nb_bouts >= 2)
        return PRISE;


    return PASSE;
}


void Game::AI_ecart(){

    std::vector<int> H, D, S, C;

    int ecart_size = 6;
    if (players.size() == 5)
        ecart_size = 3;

    //on compte les cartes sans les Rois
    for (int i = 0 ; i < (int)players[id_preneur].get_hand()->size() ; i++){
        if (players[id_preneur].get_hand()->get_card(i).get_color() == 'A' || players[id_preneur].get_hand()->get_card(i).get_value() == ROI)
            continue;
        if (players[id_preneur].get_hand()->get_card(i).get_color() == 'H')
            H.push_back(i);
        if (players[id_preneur].get_hand()->get_card(i).get_color() == 'D')
            D.push_back(i);
        if (players[id_preneur].get_hand()->get_card(i).get_color() == 'S')
            S.push_back(i);
        if (players[id_preneur].get_hand()->get_card(i).get_color() == 'C')
            C.push_back(i);
    }

    std::vector<int> empty;
    std::vector<int> *color = &empty;
    if ((int)H.size() == ecart_size)
        color = &H;
    if ((int)D.size() == ecart_size)
        color = &D;
    if ((int)S.size() == ecart_size)
        color = &S;
    if ((int)C.size() == ecart_size)
        color = &C;


    if (!color->empty()){
        for (int i = 0 ; i < ecart_size ; i++){
            players[id_preneur].give_hand_card(color[0][i], players[id_preneur].get_plis());
        }
        return;
    }

    std::vector<std::vector<int>> cartes;
    cartes.push_back(H);
    cartes.push_back(D);
    cartes.push_back(S);
    cartes.push_back(C);

    bool sorted = false;

    do {
        sorted = true;
        for (int i = 0 ; i < 4-1 ; i++){
            if (cartes[i+1].size() <= cartes[i].size())
                continue;
            sorted = false;
            std::vector<int> tmp = cartes[i+1];
            cartes[i+1] = cartes[i];
            cartes[i] = tmp;
        }
    } while(!sorted);

    sleep(2); 

    int i = 0;
    while (players[id_preneur].get_plis()->size() < ecart_size){
        if (cartes[i].size() == 0)
            i++;
        players[id_preneur].give_hand_card(cartes[i][0], players[id_preneur].get_plis());
        cartes[i].erase(cartes[i].begin());

    }

}


Card Game::AI_play(int ind, int first_player){
    if (players[ind].get_id() < ID_AI){
        std::cerr << "error AI_play(): player "<< ind << " ("<< players[ind].get_id() << ") is not an AI\n";
        exit(1);
    }

    std::vector<int> possible_moves;

    for (int i = 0 ; i < (int)players[ind].get_hand()->size() ; i++){
        if (is_move_possible(players[ind].get_hand()->get_card(i), ind))
            possible_moves.push_back(i);
    }

    int leader = pli_winner(first_player);

    bool is_mate = false;
    if ((is_preneur(ind) && is_preneur(leader)) || (!is_preneur(ind) && !is_preneur(leader)))
        is_mate = true;

    Card C;

    if (is_mate){
        for (int i = 0 ; i < (int)possible_moves.size() ; i++){
            if (players[ind].get_hand()->get_card(i).get_color() != 'A' && players[ind].get_hand()->get_card(i).get_value() > 10)
                C = players[ind].get_hand()->get_card(i);
        }

        if (C.get_color() != ' ') return C;

        if (players[ind].get_hand()->nb_atouts() == (int)possible_moves.size()){
            //peut-être jouer le 21 si il y est 
            C = players[ind].get_hand()->get_card(0);
        }

    }

    return C;
}







//MOVES
Moves::Moves(int id, std::string filename): player_id(id){
    std::ifstream file(filename);

    if (!file.is_open()){
        std::cerr << "error Moves::Moves(std::string filename): couldn't open " << filename << "\n";
        exit(1);
    }

    Deck cards;
    int card_value;
    char card_color;
    int value;
    std::vector<bool> is_mate;
    bool tmp;

    while (file.peek() != EOF){
        file >> value;
        file.get();

        while (file.peek() != ','){
            file >> card_value >> card_color;
            cards.add_card({card_value, card_color});
        }

        file.get();
        while (file.peek() != '\n'){
            file >> tmp;
            is_mate.push_back(tmp);
        }

        // file.get();
        moves.push_back({value, cards, is_mate});

        cards.set_empty();
        is_mate.clear();
    }


}

void Moves::print() const{
    if (moves.size() == 0)
        std::cout << "no moves.\n";
    else{
        for (int i = 0 ; i < (int)moves.size() ; i++){
            std::cout << "Value: " << moves[i].value << "\n";
            moves[i].cards.print("\t");
            for (int j = 0 ; j < (int)moves[i].is_mate.size() ; j++){
                if (moves[i].is_mate[j])
                    std::cout << "MATE";
                std::cout << "\t"; 
            }
            std::cout << "\n";
        }

    }
}

void print_moves(std::vector<Moves> MV){
    for (int i = 0 ; i < (int)MV.size() ; i++){
        MV[i].print();
    }
}


bool are_similar(Card A, Card B){
    if (A.is_bout() || B.is_bout()){
        if (A == B)
            return true;
        else
            return false;
    }
    if (A == B 
    || (A.get_color() == 'A' && B.get_color() == 'A' && A.get_value() > 15 && B.get_value() > 15)
    || (A.get_color() == 'A' && B.get_color() == 'A' && A.get_value() < 9 && B.get_value() < 9)
    || (A.get_color() == 'A' && B.get_color() == 'A' && A.get_value() >= 9 && B.get_value() >= 9 && A.get_value() <= 15 && B.get_value() <= 15)
    || (A.get_color() != 'A' && B.get_color() != 'A' && A.get_value() >= CAVALIER && B.get_value() >= CAVALIER)
    || (A.get_color() != 'A' && B.get_color() != 'A' && A.get_value() <= 10 && B.get_value() <= 10)
    || (A.get_color() != 'A' && B.get_color() != 'A' && A.get_value() < CAVALIER && B.get_value() < CAVALIER && A.get_value() > 10 && B.get_value() > 10)
    )
        return true;
    else
        return false;
}




void Moves::save_in_file(std::string filename){
    std::ofstream file(filename);

    if (!file.is_open()){
        std::cerr << "error Moves::save_in_file(): couldn't open " << filename << "\n";
        exit(1);
    }

    for (int i = 0 ; i < (int)moves.size() ; i++){
        file << moves[i].value << ","; 
        for (int j = 0 ; j < moves[i].cards.size() ; j++){
            file << moves[i].cards.get_card(j).get_value() << moves[i].cards.get_card(j).get_color() << ' '; 
        }
        file << ",";
        for (int j = 0 ; j < (int)moves[i].is_mate.size() ; j++){
            file << moves[i].is_mate[j] << ' '; 
        }

        file << "\n";

    }
   
  
}







std::vector<Deck> Moves::relevant_moves(Game G, int first_player) const{
    std::vector<Deck> RM;
    for (int i = 0 ; i < 4 ; i++)
        RM.push_back({});

    if (first_player == G.get_players_ind(player_id)){ 
        for (int i = 0 ; i < (int)moves.size() ; i++){
            if (moves[i].value >= 2 && moves[i].is_mate[0])
                RM[moves[i].value].add_card(moves[i].cards.get_card(0));
        }

    }
            
    else{
        Card winning_card_Game = G.get_pli().get_card(G.get_pli().winner());
        Card winning_card_move;
        for (int i = 0 ; i < (int)moves.size() ; i++){
            winning_card_move = moves[i].cards.get_card(moves[i].cards.winner());
            if (are_similar(moves[i].cards.get_card(0), G.get_pli().get_card(0)) && are_similar(winning_card_move, winning_card_Game)){
                    for (int k = 0 ; k < moves[i].cards.size() ; k++){
                        if ((G.are_mates(G.get_players_ind(player_id), G.pli_winner(first_player))
                            && moves[i].is_mate[k] == moves[i].is_mate[moves[i].cards.winner()])
                            || 
                            (!G.are_mates(G.get_players_ind(player_id), G.pli_winner(first_player))
                            && moves[i].is_mate[k] != moves[i].is_mate[moves[i].cards.winner()]))
                        {
                                //IA et winner sont mate $
                                if (moves[i].value > 2 && moves[i].cards.get_card(k).get_value() < 10) //carte faible
                                    RM[moves[i].value-2].add_card(moves[i].cards.get_card(k));
                                else
                                    RM[moves[i].value].add_card(moves[i].cards.get_card(k));
                        }
                    }
                
            }
        }

    }



    return RM;
}


Card Moves::AI_play_ML(Game G, int ind, int first_player){
    std::vector<Deck> RM = relevant_moves(G, first_player);
    int rng = -1;
    srand((unsigned) time(NULL));
    Card C;



    if (RM[0].size() > 0 || RM[1].size() > 0 || RM[2].size() > 0 || RM[3].size() > 0){
        int i = 0;
        int v = 3;
        while (v >= 0 || i < RM[0].size()){
            while (i == RM[v].size()){
                i = 0;
                v--;
            }
            if (v >= 0 && i < RM[v].size()){
                for(int j = 0 ; j < G.get_players_hand_size(ind) ; j++){
                    if (are_similar(G.get_players_card(ind, j), RM[v].get_card(i)) && G.is_move_possible(G.get_players_card(ind, j), ind)){
                        C = G.get_players_card(ind, j);
                        return C;
                    }
                }
            }
            i++;
        }
    }


    do {
        rng = rand() % G.get_players_hand_size(ind);
        C = G.get_players_card(ind, rng);

    }while (!G.is_move_possible(C, ind));


    return C;

}