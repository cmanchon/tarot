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

    int leader = plis_winner(first_player);

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

}







//MOVES
Moves::Moves(std::string filename){

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