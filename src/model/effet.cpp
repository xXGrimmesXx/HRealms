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

void Effet::appliquer(Carte* source, void* target, Partie& partie, bool choix) const {
    switch (type) {
        case TypeEffet::AJOUT_OR :
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    partie.getTourCourant()->ajouterOr(montant1);
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
                    partie.getTourCourant()->ajouterDegats(montant1);
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
            if (cible == TypeCible::SOI || cible == TypeCible::ADVERSAIRE) {
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
                    for (auto& j : partie.getJoueurs()){
                        //on cherche dans la main car si c'etait un champion c'est l'effet demobiliser
                        for (int i=0;i<j.getMain().getTaille();i++){
                            if (j.getMain().getCarte(i) == carteTgt){
                                //si on trouve la bonne carte on la défausse
                                j.getMain().defausser(i);
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
                    for (auto j : partie.getJoueurs()){
                        for (auto champ : j.getChampions()){
                            if (champ==championTgt){
                                //si on le trouve on l'assome
                                j.assomerChampion(championTgt);
                            }
                        }
                    }
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
                    for (auto& j : partie.getJoueurs()){
                        //on cherche dans la main
                        for (int i=0;i<j.getMain().getTaille();i++){
                            if (j.getMain().getCarte(i) == carteTgt){
                                carteTgt->sacrifier(j,partie);
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
                        partie.getTourCourant()->ajouterDegats(montant1);
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
                        partie.getTourCourant()->ajouterDegats(montant2);
                    } else {
                        partie.getTourCourant()->ajouterOr(montant1);                        
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
                    partie.getTourCourant()->ajouterDegats(montant1*nbChampions);

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
                    partie.getTourCourant()->ajouterDegats(montant1*nbGardes);

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
                    for (auto carte : joueurTgt->getMain().getCartes()){
                        if (carte->getFaction() == allie){
                            nbAllie++;
                        }
                    }

                    partie.getTourCourant()->ajouterDegats(nbAllie*montant1);

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
                        std::cout << "Nombre de champions pour le calcul de vie : " << nbChampions << std::endl;
                        std::cout << "Montant2 : " << montant2 << " total : " << montant2 * nbChampions << std::endl;
                        joueurTgt->soigner(montant2*nbChampions);
                    } else {
                        std::cout << "Ajout d'or : " << montant1 << std::endl;
                        partie.getTourCourant()->ajouterOr(montant1); 
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
                    std::cout << "Nombre de champions pour le calcul de vie : " << nbChampions << std::endl;
                    std::cout << "Montant2 : " << montant2 << " total : " << montant1 * nbChampions << std::endl;
                    joueurTgt->soigner(montant1*nbChampions);
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
                    partie.getJoueurActuel().getDeck().ajouterDessus(carteTgt);
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
                    partie.getJoueurActuel().getDeck().ajouterDessus(championTgt);
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
                    Carte * carteTgt = static_cast<Carte*>(target);
                    if (carteTgt != nullptr) {
                        //le joueur qui applique l'effet est forcément le joueur actuel
                        Joueur joueurProprietaire = partie.getJoueurActuel();
                        carteTgt->sacrifier(joueurProprietaire,partie);
                    }
                    //si carteTgt est nullptr on ne fait rien le joueur ne voulait pas sacrifier de carte
                }
                else{
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
                        Joueur joueurProprietaire = partie.getJoueurActuel();
                        joueurProprietaire.piocher(montant1);
                        for (auto c : *carteTgt){
                            int indexCarteDefausse=-1;
                            //on cherche la carte dans la main du joueur
                            for (int i=0;i<joueurProprietaire.getMain().getTaille();i++){
                                if (joueurProprietaire.getMain().getCarte(i) == c){
                                    indexCarteDefausse = i;
                                    break;
                                }
                            }
                            joueurProprietaire.defausser(indexCarteDefausse);
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