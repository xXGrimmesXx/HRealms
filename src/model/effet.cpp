#include "effet.h"

#include "joueur.h"
#include "partie.h"
#include "tour.h"

Effet::Effet(TypeEffet t, int m1, TypeCible c){
    type = t;
    montant1 = m1;
    montant2 = -1;
    cible = c;
}

// Pour les effets qui ont deux options (comme COMBAT_OU_VIE)
Effet::Effet(TypeEffet t, int m1,int m2, TypeCible c){
    type = t;
    montant1 = m1;
    montant2 = m2;
    cible = c;
}

void Effet::appliquer(Carte* source, void* target, Partie* partie, bool choix) const {
    switch (type) {
        case TypeEffet::AJOUT_OR :
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    partie->getTourCourant()->ajouterOr(montant1);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
                
            }
            else{
                throw std::invalid_argument("Invalid target type for AJOUT_OR effect");
            }
            break;
        case TypeEffet::AJOUT_COMBAT:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    partie->getTourCourant()->ajouterDegats(montant1);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                throw std::invalid_argument("Invalid target type for AJOUT_COMBAT effect");
            }
            break;
        case TypeEffet::AJOUT_VIE:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    joueurTgt->soigner(montant1);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                throw std::invalid_argument("Invalid target type for AJOUT_VIE effect");
            }
            break;
        case TypeEffet::PIOCHER:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    joueurTgt->piocher(montant1);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                throw std::invalid_argument("Invalid target type for PIOCHER effect");
            }
            break;
        case TypeEffet::DEFAUSSER:
            if (cible == TypeCible::CARTE) {
                Carte* carteTgt = static_cast<Carte*>(target);
                if (carteTgt != nullptr) {
                    //on cherche le propriétaire de la carte à défausser
                    for (auto j : partie->getJoueurs()){
                        //on cherche dans la main car si c'etait un champion c'est l'effet demobiliser
                        for (int i=0;i<j->getMain()->getTaille();i++){
                            if (j->getMain()->getCarte(i) == carteTgt){
                                //si on trouve la bonne carte on la défausse
                                j->defausser(carteTgt);
                                break;
                            }
                        }
                    }
                }
                //si carteTgt est nullptr on ne fait rien le joueur ne voulait pas faire défausser de carte
            }
            else{
                throw std::invalid_argument("Invalid target type for DEFAUSSE_CIBLE effect");
            }
            break;
        case TypeEffet::ASSOMMER_CHAMPION:
            if (cible == TypeCible::CHAMPION) {
                Champion* championTgt = static_cast<Champion*>(target);
                if (championTgt != nullptr) {
                    //on cherche le champion dans tous les champions de tous les joueurs
                    for (Joueur* j : partie->getJoueurs()){
                        for (Champion* champ : j->getChampions()){
                            if (champ == championTgt){
                                //si on le trouve on l'assome
                                j->assomerChampion(championTgt);
                                break;
                            }
                        }
                    }
                }
                for (Joueur* j : partie->getJoueurs()){
                    std::cout << j->getChampions().size() <<" champions dans effet.cpp pour "<< j->getNom() << std::endl;
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for ASSOMMER_CHAMPION effect");
            }
            break;

        case TypeEffet::SACRIFIER:
            if (cible == TypeCible::CARTE) {
                Carte* carteTgt = static_cast<Carte*>(target);
                if (carteTgt != nullptr) {
                    //on cherche le propriétaire de la carte à sacrifier
                    for (Joueur* j : partie->getJoueurs()){
                        //on cherche dans la main
                        for (int i=0;i<j->getMain()->getTaille();i++){
                            if (j->getMain()->getCarte(i) == carteTgt){
                                j->sacrifier(carteTgt,partie);
                                break;
                            }
                        }
                    }
                }
                //si carteTgt est nullptr on ne fait rien le joueur ne voulait pas sacrifier de carte
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for SACRIFIER effect");
            }
            break;

        case TypeEffet::COMBAT_OU_VIE:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    if (choix) {
                        joueurTgt->soigner(montant2);
                    } else {
                        partie->getTourCourant()->ajouterDegats(montant1);
                    }
                } else {
                    // le joueur est obligé d'appliquer cet effet
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for COMBAT_OU_VIE effect");
            }
            break;

        case TypeEffet::OR_OU_COMBAT:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    if (choix) {
                        partie->getTourCourant()->ajouterDegats(montant2);
                    } else {
                        partie->getTourCourant()->ajouterOr(montant1);
                    }
                } else {
                    //le joueur est obligé d'appliquer cet effet
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for OR_OU_COMBAT effect");
            }
            break;

        case TypeEffet::COMBAT_PAR_CHAMPION:
            if (cible== TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    int nbChampions = joueurTgt->getChampions().size();
                    int nbChampionsMain = joueurTgt->getChampionsMain().size();
                    partie->getTourCourant()->ajouterDegats(montant1*(nbChampions + nbChampionsMain));
                }
                else{
                    //le joueur est obligé d'appliquer cet effet
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                    // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for COMBAT_PAR_CHAMPION effect");
            }
            break;

        case TypeEffet::COMBAT_PAR_GARDE:
            if (cible== TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    int nbGardes = joueurTgt->getGardes().size();
                    int nbGardesMain = joueurTgt->getGardesMain().size();
                    partie->getTourCourant()->ajouterDegats(montant1*(nbGardes + nbGardesMain));

                }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                    // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for COMBAT_PAR_GARDE effect");
            }
            break;

        case TypeEffet::COMBAT_PAR_ALLIE:
            if (cible== TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    Faction allie = source->getFaction();
                    int nbAllie = 0;

                    for (auto champ : joueurTgt->getChampions()){
                        if (champ->getFaction()==allie){
                            nbAllie++;
                        }
                    }
                    for (auto carte : joueurTgt->getMain()->getCartes()){
                        if (carte->getFaction() == allie){
                            nbAllie++;
                        }
                    }

                    partie->getTourCourant()->ajouterDegats(nbAllie*montant1);

                }
                else{
                    throw std::invalid_argument("Target is not a valid Carte pointer");
                }
            }
            else{
                    // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for COMBAT_PAR_ALLIE effect");
            }
            break;

        case TypeEffet::OR_OU_VIE_PAR_CHAMPION:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    if (choix) {
                        int nbChampions = joueurTgt->getChampions().size();
                        int nbChampionsMain = joueurTgt->getChampionsMain().size();
                        joueurTgt->soigner(montant2 * (nbChampions + nbChampionsMain));
                    } else {
                        partie->getTourCourant()->ajouterOr(montant1);
                    }
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            break;

        case TypeEffet::VIE_PAR_CHAMPION :
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    int nbChampions = joueurTgt->getChampions().size();
                    int nbChampionsMain = joueurTgt->getChampionsMain().size();
                    joueurTgt->soigner(montant1 * (nbChampions + nbChampionsMain));
                }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for VIE_PAR_CHAMPION effect");
            }
            break;

        case TypeEffet::DEFAUSSE_AU_DESSUS_DU_DECK :
            if (cible == TypeCible::CARTE){
                Carte* carteTgt = static_cast<Carte*>(target);
                if (carteTgt != nullptr){
                    partie->getJoueurActuel()->getDeck()->ajouterDessus(carteTgt);
                }
                else{
                    throw std::invalid_argument("Target is not a valid Carte pointer");
                }
                
            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for DEFAUSSE_AU_DESSUS_DU_DECK effect");
            }
            break;

        case TypeEffet::CHAMPION_DEFAUSSE_AU_DESSUS_DECK :
            if (cible == TypeCible::CHAMPION){
                Champion* championTgt = static_cast<Champion*>(target);
                if (championTgt != nullptr){
                    partie->getJoueurActuel()->getDeck()->ajouterDessus(championTgt);
                }
                else{
                    throw std::invalid_argument("Target is not a valid Champion pointer");
                }
            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for CHAMPION_DEFAUSSE_AU_DESSUS_DECK effect");
            }
            break;

        case TypeEffet::PROCHAIN_ACHAT_ACTION_DESSUS_DECK :
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    joueurTgt->activerProchainAchatActionDessusDeck();
                }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
                

            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for PROCHAIN_ACHAT_ACTION_DESSUS_DECK effect");
            }
            break;

        case TypeEffet::PROCHAIN_ACHAT_DANS_MAIN :
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    joueurTgt->activerProchainAchatDansMain();
                }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
                

            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for PROCHAIN_ACHAT_DANS_MAIN effect");
            }
            break;

        case TypeEffet::SACRIFIER_SIOUI_AJOUT_COMBAT :
                if (cible == TypeCible::CARTE) {
                    Carte* carteTgt = static_cast<Carte*>(target);
                    if (carteTgt != nullptr) {
                        Joueur* joueurProprietaire = partie->getJoueurActuel();
                        bool carteDansMain = false;
                        // On vérifie si la carte à sacrifier est dans la main du joueur actuel
                        // Pour éviter un segfault
                        for (int i = 0; i < joueurProprietaire->getMain()->getTaille(); ++i) {
                            if (joueurProprietaire->getMain()->getCarte(i) == carteTgt) {
                                carteDansMain = true;
                                break;
                            }
                        }
                        if (carteDansMain) {
                            // Sacrifier la carte sans utiliser ses effets de sacrifice
                            joueurProprietaire->sacrifier(carteTgt, partie);
                            partie->getTourCourant()->ajouterDegats(montant1);
                        } else {
                            std::cerr << "Erreur : la carte à sacrifier n'est pas dans la main du joueur actuel." << std::endl;
                        }
                    }
                    //si carteTgt est nullptr on ne fait rien le joueur ne voulait pas sacrifier de carte
                }
                else {
                    // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for SACRIFIER_SIOUI_AJOUT_COMBAT effect");
                }
            break;

        case TypeEffet::PIOCHE_SIOUI_DEFAUSSE :
            if (cible == TypeCible::CARTE){
                std::vector<Carte*>* carteTgt = static_cast<std::vector<Carte*>*>(target);
                    if (carteTgt != nullptr){
                        //si on applique l'effet c'est que le joueur veut bien piocher et a choisi sa carte à defausser

                        //le joueur qui applique l'effet est forcément le joueur actuel
                        Joueur* joueurProprietaire = partie->getJoueurActuel();
                        for (size_t i=0; i<carteTgt->size(); i++){
                            //on vérifie que la carte n'est pas un nullptr (au cas où le joueur ne veut pas défausser de carte)
                            if (carteTgt->at(i)!=nullptr){
                                joueurProprietaire->piocher(1);
                                joueurProprietaire->defausser(carteTgt->at(i));
                            }
                        }
                    }
                    //si carteTgt est nullptr on ne fait rien le joueur ne voulait pas défausser de carte
            }else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for PIOCHE_SIOUI_DEFAUSSE effect");
            }
            break;

        case TypeEffet::PREPARE_CHAMPION:
            if (cible == TypeCible::CHAMPION){
                Champion* championTgt = static_cast<Champion*>(target);
                if (championTgt != nullptr){
                    championTgt->demobiliser();
                }
            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for PREPARE_CHAMPION effect");
            }
            break;
        
        default:
            throw std::invalid_argument("Effet inconnu");
    }
}