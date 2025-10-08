#include "effet.h"


Effet::Effet(TypeEffet t, int m, TypeCible c){
    type = t;
    montant = m;
    cible = c;
}

void Effet::appliquer(Joueur* source, void* target, Partie& partie,int choix=0){
    switch (type) {
        case TypeEffet::AJOUT_OR:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr && dynamic_cast<Joueur*>(joueurTgt)) {
                    // Assuming Joueur has a method to add gold
                    partie.getTourCourant()->ajouterOr(montant);
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
                    partie.getTourCourant()->ajouterDegats(montant);
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for AJOUT_COMBAT effect");
            }
        case TypeEffet::AJOUT_VIE:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    joueurTgt->soigner(montant);
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
                    joueurTgt->piocher(montant);
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
                    joueurTgt->defausser(montant);
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
                    championTgt->assommer();
                } else {
                    throw std::invalid_argument("Target is not a valid Champion pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for ASSOMMER_CHAMPION effect");
            }
            break;
        case TypeEffet::ASSOMMER_TOUS_CHAMPIONS:
            if (cible == TypeCible::ADVERSAIRE) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    for (Champion* champ : joueurTgt->getChampions()) {
                        champ->assommer();
                    }
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for ASSOMMER_TOUS_CHAMPIONS effect");
            }
            break;
        case TypeEffet::SACRIFIER:
            if (cible == TypeCible::CHAMPION) {
                Champion* championTgt = static_cast<Champion*>(target);
                if (championTgt != nullptr) {
                    championTgt->sacrifier(partie.getJoueurActuel(), partie);
                } else {
                    throw std::invalid_argument("Target is not a valid Champion pointer");
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
                    if (choix == 1) {
                        partie.getTourCourant()->ajouterDegats(montant);
                    } else if (choix == 2) {
                        joueurTgt->soigner(montant);
                    } else {
                        throw std::invalid_argument("Invalid choice for COMBAT_Ou_VIE effect");
                    }
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for COMBAT_Ou_VIE effect");
            }
            break;

        case TypeEffet::OR_OU_VIE:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    if (choix == 1) {
                        partie.getTourCourant()->ajouterOr(montant);
                    } else if (choix == 2) {
                        joueurTgt->soigner(montant);
                    } else {
                        throw std::invalid_argument("Invalid choice for OR_OU_VIE effect");
                    }
                } else {
                    throw std::invalid_argument("Target is not a valid Joueur pointer");
                }
            }
            else{
                // Invalid target type for this effect
                throw std::invalid_argument("Invalid target type for OR_OU_VIE effect");
            }
            break;
        case TypeEffet::OR_OU_COMBAT:
            if (cible == TypeCible::SOI) {
                Joueur* joueurTgt = static_cast<Joueur*>(target);
                if (joueurTgt != nullptr) {
                    if (choix == 1) {
                        partie.getTourCourant()->ajouterOr(montant);
                    } else if (choix == 2) {
                        partie.getTourCourant()->ajouterDegats(montant);
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

        
        default:
            throw std::invalid_argument("Effet inconnu");
    }
}