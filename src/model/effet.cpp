#include "effet.h"

#include "joueur.h"
#include "partie.h"
#include "tour.h"

Effet::Effet(TypeEffet t, int m1, TypeCible c){
    type = t;
    montant1 = m1;
    montant2 =0;
    cible = c;
}

// Pour les effets qui ont deux options (comme COMBAT_OU_VIE)
Effet::Effet(TypeEffet t, int m1,int m2, TypeCible c){
    type = t;
    montant1 = m1;
    montant2 = m2;
    cible = c;
}

void Effet::appliquer(Carte* source, void* target, Partie& partie) const {
    switch (type) {
        case TypeEffet::AJOUT_OR :
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    // Assuming Joueur has a method to add gold
                    partie.getTourCourant()->ajouterOr(montant1);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
                
            }
            // Handle other target types if necessary
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for AJOUT_OR effect");
            }
            break;
        case TypeEffet::AJOUT_COMBAT:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    // Assuming Joueur has a method to add combat points
                    partie.getTourCourant()->ajouterDegats(montant1);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
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
                // Invalid target type for this effect
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
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for PIOCHER effect");
            }
            break;
        case TypeEffet::DEFAUSSER:
            if (cible == TypeCible::ADVERSAIRE|| cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    joueurTgt->defausser(montant1);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for DEFAUSSE_CIBLE effect");
            }
            break;
        case TypeEffet::ASSOMMER_CHAMPION:
            if (cible == TypeCible::CHAMPION) {
                Champion* championTgt = static_cast<Champion*>(target);
                if (championTgt != nullptr) {
                    for (auto j : partie.getJoueurs()){
                        for (auto champ : j.getChampions()){
                            if (champ==championTgt){
                                j.assomerChampion(championTgt);
                            }
                        }
                    }
                } else {
                    throw std::invalid_argument("Target is not a valid Champion pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for ASSOMMER_CHAMPION effect");
            }
            break;

        case TypeEffet::SACRIFIER:
            if (cible == TypeCible::ADVERSAIRE|| cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    joueurTgt->sacrifier(montant1,partie);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
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
                    std::cout << "1 : " << montant1 << " degats " << std::endl;
                    std::cout << "2 : " << montant2 << " vie " << std::endl;
                    std::cout << "Choisissez une option (1 ou 2) : ";
                    int choix;
                    std::cin >> choix;
                    while (choix != 1 && choix != 2) {
                        std::cout << "Choix invalide. Veuillez choisir 1 ou 2 : ";
                        std::cin >> choix;
                    }
                    if (choix == 1) {
                        partie.getTourCourant()->ajouterDegats(montant1);
                    } else if (choix == 2) {
                        joueurTgt->soigner(montant2);
                    } else {
                        throw std::invalid_argument("Invalid choice for COMBAT_OU_VIE effect");
                    }
                } else {
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
                    std::cout << "1 : " << montant1 << " Or " << std::endl;
                    std::cout << "2 : " << montant2 << " Degat(s) " << std::endl;
                    std::cout << "Choisissez une option (1 ou 2) : ";
                    int choix;
                    std::cin >> choix;
                    while (choix != 1 && choix != 2) {
                        std::cout << "Choix invalide. Veuillez choisir 1 ou 2 : ";
                        std::cin >> choix;
                    }
                    if (choix == 1) {
                        partie.getTourCourant()->ajouterOr(montant1);
                    } else if (choix == 2) {
                        partie.getTourCourant()->ajouterDegats(montant2);
                    } else {
                        throw std::invalid_argument("Invalid choice for OR_OU_COMBAT effect");
                    }
                } else {
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
            if (cible== TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    std::cout << "1 : " << montant1 << " Or " << std::endl;
                    std::cout << "2 : " << montant2 << " soin par champion en jeu " << std::endl;
                    std::cout << "Choisissez une option (1 ou 2) : ";
                    int choix;
                    std::cin >> choix;
                    while (choix != 1 && choix != 2) {
                        std::cout << "Choix invalide. Veuillez choisir 1 ou 2 : ";
                        std::cin >> choix;
                    }
                    if (choix == 1) {
                        partie.getTourCourant()->ajouterOr(montant1);
                    } else if (choix == 2) {
                        int nbChampions = joueurTgt->getChampions().size();
                        partie.getTourCourant()->ajouterDegats(montant2*nbChampions);
                    } else {
                        throw std::invalid_argument("Invalid choice for OR_OU_VIE_PAR_CHAMPION effect");
                    }
                }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                    // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for OR_OU_VIE_PAR_CHAMPION effect");
            }
            break;

        case TypeEffet::VIE_PAR_CHAMPION :
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    int nbChampions = joueurTgt->getChampions().size();
                    joueurTgt->soigner(montant2*nbChampions);
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
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    joueurTgt->choisirCarteDefausseAjouterDessusDeck(montant1);
                }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
                
            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for DEFAUSSE_AU_DESSUS_DU_DECK effect");
            }
            break;

        case TypeEffet::CHAMPION_DEFAUSSE_AU_DESSUS_DECK :
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr){
                    joueurTgt->choisirChampionDefausseAjouterDessusDeck(montant1);
                }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
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
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                    if (joueurTgt != nullptr){
                        std::cout << "Voulez vous sacrifier "<<montant1<<" carte(s) et ensuite gagner "<<montant2 <<" combat(s) ?"<<std::endl;
                        std::cout << "1 : oui" << std::endl;
                        std::cout << "2 : non" << std::endl;
                        std::cout << "Choisissez une option (1 ou 2) : ";
                        int choix;
                        std::cin >> choix;
                        while (choix != 1 && choix != 2) {
                            std::cout << "Choix invalide. Veuillez choisir 1 ou 2 : ";
                            std::cin >> choix;
                        }
                        if (choix == 1) {
                            joueurTgt->sacrifier(montant1,partie);
                            partie.getTourCourant()->ajouterDegats(montant2);

                        } else if (choix == 2) {
                            break;
                        } else {
                            throw std::invalid_argument("Invalid choice for SACRIFIER_SIOUI_AJOUT_COMBAT effect");
                        }
                    }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for SACRIFIER_SIOUI_AJOUT_COMBAT effect");
            }
            break;

        case TypeEffet::PIOCHE_SIOUI_DEFAUSSE :
            if (cible == TypeCible::SOI){
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                    if (joueurTgt != nullptr){
                        std::cout << "Voulez vous piocher "<<montant1<<" carte(s) et ensuite defausser "<<montant2 <<" carte(s) ?"<<std::endl;
                        std::cout << "1 : oui" << std::endl;
                        std::cout << "2 : non" << std::endl;
                        std::cout << "Choisissez une option (1 ou 2) : ";
                        int choix;
                        std::cin >> choix;
                        while (choix != 1 && choix != 2) {
                            std::cout << "Choix invalide. Veuillez choisir 1 ou 2 : ";
                            std::cin >> choix;
                        }
                        if (choix == 1) {
                            joueurTgt->piocher(montant1);

                        } else if (choix == 2) {
                            joueurTgt->defausser(montant2);
                        } else {
                            throw std::invalid_argument("Invalid choice for PIOCHE_SIOUI_DEFAUSSE effect");
                        }
                    }
                else{
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }else{
                // Invalid target type for this effect
                    throw std::invalid_argument("Invalid target type for PIOCHE_SIOUI_DEFAUSSE effect");
            }
            break;
        
        default:
            throw std::invalid_argument("Effet inconnu");
    }
}