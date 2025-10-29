#include "cardRenderer.h"
#include "../../model/effet.h"
#include <iomanip>
#include <algorithm>

std::vector<std::string> CardRenderer::renderCard(const Carte* carte, int index) {
    std::vector<std::string> lines(CARD_HEIGHT);
    
    if (!carte) {
        return renderEmptySlot(index);
    }
    
    std::string color = getFactionColor(carte->getFaction());
    std::string typeSymbol = getTypeSymbol(carte);
    std::string grayOut = carte->getEstJouee() ? RED : color;
    if (const Champion* champion = dynamic_cast<const Champion*>(carte)) {
        grayOut = champion->estMobilise() ? RED : color;
    }


    
    // Ligne 0: Bordure supérieure
    lines[0] = grayOut + "┌" + std::string(CARD_WIDTH-2, '-') + "┐" + RESET;
    
    // Ligne 1: Index et coût
    std::string indexStr = (index >= 0) ? "[" + std::to_string(index + 1) + "]" : "";
    std::string costemoji = "💰 ";
    std::string costStr = costemoji + BOLD + std::to_string(carte->getCout()) + NORMAL;
    std::string line1Content = indexStr + std::string(CARD_WIDTH +7 - indexStr.length() - costStr.length(), ' ') + costStr;
    lines[1] = grayOut + "│ " + line1Content + " │" + RESET;
    
    // Ligne 2: Nom de la carte
    std::string nom = truncateText(carte->getNom(), CARD_WIDTH - 4)[0];
    lines[2] = grayOut + "│ " + BOLD + centerText(nom, CARD_WIDTH - 4) + RESET + grayOut + " │" + RESET;
    
    // Ligne 3: Type et faction
    std::string typeInfo = typeSymbol + " " + getFactionSymbol(carte->getFaction());
    //ajustement a cause des emojis plus larges
    if (carte->getFaction() == Faction::GUILDE) {
        lines[3] = grayOut + "│ " + centerText(typeInfo, CARD_WIDTH+2) + " │" + RESET;
    }
    else if (carte->getFaction() == Faction::IMPERIALE) {
        lines[3] = grayOut + "│ " + centerText(typeInfo, CARD_WIDTH+3) + " │" + RESET;
    }
    else if (carte->getFaction() == Faction::AUCUNE) {
        lines[3] = grayOut + "│ " + centerText(typeInfo, CARD_WIDTH+1) + " │" + RESET;
    }
    else {
        lines[3] = grayOut + "│ " + centerText(typeInfo, CARD_WIDTH) + " │" + RESET;
    }
    
    
    // Ligne 4: Séparateur
    lines[4] = grayOut + "├" + std::string(CARD_WIDTH-2, '-') + "┤" + RESET;
    
    // Lignes 5-10: Effets
    std::vector<std::string> effects;
    
    // Effets primaires
    if (!carte->getEffetsPrimaire().empty()) {
        effects.push_back("Effets primaires");
        for (const auto& effet : carte->getEffetsPrimaire()) {
            effects.push_back("   " + formatEffect(effet));
        }
    }

    // Effets alliés
    if (!carte->getEffetsAllie().empty()) {
        effects.push_back("Effets allies");
        for (const auto& effet : carte->getEffetsAllie()) {
            effects.push_back("   " + formatEffect(effet));
        }
    }

    // Effets sacrifice
    if (!carte->getEffetsSacrifice().empty()) {
        effects.push_back("Effets sacrifice");
        for (const auto& effet : carte->getEffetsSacrifice()) {
            effects.push_back("   " + formatEffect(effet));
        }
    }

    // Si champion, effets d'activation

    if (const Champion* champion = dynamic_cast<const Champion*>(carte)) {
        effects.push_back("Effets mobilisation:");
        for (const auto& effet : champion->getEffetsActiver()) {
            effects.push_back("   " + formatEffect(effet));
        }
        
        // Afficher garde et mobilisation
        if (champion->estGarde()) {
            lines[ CARD_HEIGHT - 2] = grayOut + "│ 🛡️ Garde " +BOLD+std::to_string(champion->getVie())+NORMAL+"❤️"+ std::string(CARD_WIDTH - 15, ' ') + "│" + RESET;
        }
        else {
            // Champion sans garde : afficher PV + statut mobilisé
            lines[ CARD_HEIGHT - 2] = grayOut + "│ " +BOLD+std::to_string(champion->getVie())+NORMAL+"❤️"+ std::string(CARD_WIDTH - 6, ' ') + "│" + RESET;
        }
    }
    else {
        // Si pas champion, ligne vide avant la bordure inférieure
        lines[ CARD_HEIGHT - 2] = grayOut + "│" + std::string(CARD_WIDTH - 2, ' ') + "│" + RESET;
    }
    
    // Remplir les lignes d'effets

    //on commence à la ligne 5 car en-tete carte jusqu'a la ligne 4
    int effectIndex =  0;
    for (int nb_ligne = 5; nb_ligne < CARD_HEIGHT-1; ++nb_ligne) {
        //faire correspondre i avec les indice des effets
        
        // si il y a un effet à afficher
        if (effectIndex < (int)effects.size()) {
            std::vector<std::string> effect = truncateText(effects[effectIndex], CARD_WIDTH - 4);

            for (const auto& line : effect) {
                lines[nb_ligne] = grayOut + "│ " + line + std::string(CARD_WIDTH - 3 - line.length(), ' ') + "│" + RESET;
                ++nb_ligne;
            }
            nb_ligne--; // ajuster car la boucle for va l'incrementer
        } else if (lines[nb_ligne].empty()) {
            lines[nb_ligne] = grayOut + "│" + std::string(CARD_WIDTH - 2, ' ') + "│" + RESET;
        }
        effectIndex++;
    }
    
    // DERNIèRE LIGNE: Bordure inférieure
    lines[CARD_HEIGHT - 1] = grayOut + "└" + std::string(CARD_WIDTH-2, '-') + "┘" + RESET;
    
    return lines;
}

std::vector<std::string> CardRenderer::renderEmptySlot(int index) {
    std::vector<std::string> lines(CARD_HEIGHT);
    std::string indexStr = (index >= 0) ? "[" + std::to_string(index + 1) + "]" : "";
    
    lines[0] = GRAY + std::string("┌") + std::string(CARD_WIDTH-2, '-') + "┐" + RESET;
    lines[1] = GRAY + std::string("│") + indexStr + std::string(CARD_WIDTH - 2 - indexStr.length(), ' ') + "│" + RESET;
    
    for (int i = 2; i < CARD_HEIGHT - 1; ++i) {
        if (i == CARD_HEIGHT / 2) {
            lines[i] = GRAY + std::string("│") + centerText("VIDE", CARD_WIDTH - 2) + "│" + RESET;
        } else {
            lines[i] = GRAY + std::string("│") + std::string(CARD_WIDTH - 2, ' ') + "│" + RESET;
        }
    }

    lines[CARD_HEIGHT-1] = GRAY + std::string("└") + std::string(CARD_WIDTH-2, '-') + "┘" + RESET;
    return lines;
}

void CardRenderer::printCardsRow(const std::vector<std::vector<std::string>>& cards) {
    if (cards.empty()) return;
    
    for (int line = 0; line < CARD_HEIGHT; ++line) {
        for (size_t card = 0; card < cards.size(); ++card) {
            std::cout << cards[card][line];
            if (card < cards.size() - 1) {
                std::cout << "  "; // Espacement entre les cartes
            }
        }
        std::cout << std::endl;
    }
}

void CardRenderer::printCards(const std::vector<Carte*>& cartes, const int cardsPerRow) {
    int taille = cartes.size();
    
    if (taille == 0) {
        std::cout << "Aucune carte à afficher." << std::endl;
        return;
    }
    
    for (int row = 0; row < (taille + cardsPerRow - 1) / cardsPerRow; ++row) {
        std::vector<std::vector<std::string>> cardsInRow;
        
        for (int i = 0; i < cardsPerRow; ++i) {
            int cardIndex = row * cardsPerRow + i;
            if (cardIndex < taille) {
                Carte* carte = cartes.at(cardIndex);
                cardsInRow.push_back(renderCard(carte, cardIndex));
            }
        }
        
        if (!cardsInRow.empty()) {
            printCardsRow(cardsInRow);
            std::cout << std::endl;
        }
    }
}

std::string CardRenderer::getFactionColor(Faction faction) {
    switch (faction) {
        case Faction::IMPERIALE: return BLUE;
        case Faction::SAUVAGE: return GREEN;
        case Faction::GUILDE: return YELLOW;
        case Faction::NECROS: return MAGENTA;
        case Faction::AUCUNE: return WHITE;
        default: return WHITE;
    }
}

std::string CardRenderer::getTypeSymbol(const Carte* carte) {
    if (dynamic_cast<const Champion*>(carte)) return "👑";
    if (dynamic_cast<const CarteObjet*>(carte)) return "⚔️";
    if (dynamic_cast<const CarteAction*>(carte)) return "📜";
    return "❓";
}

std::vector<std::string> CardRenderer::truncateText(const std::string& text, int maxLength) {
    std::vector<std::string> result;
    std::istringstream words(text);
    std::string line = "  ";
    std::string word;
    // si le texte est déjà plus court que la longueur max, on le retourne directement
    if (text.length() <= (size_t)maxLength) {
        result.push_back(text);
        return result;
        
    }
    while (words >> word) {
        //tant qu'il y a des mots 
        if (line.length() + word.length() + 1 > (size_t)maxLength) {
            // check si le mot peut rentrer dans la ligne actuelle 
            //si non on ajoute la ligne dans le resultat et on commence une nouvelle ligne
            result.push_back(line);
            line = "  " + word;
        } else {
            //si oui on ajoute le mot à la ligne actuelle
            if (!line.empty()) line += " ";
            line += word;
        }
    }
    //ajout de la dernierre ligne
    if (!line.empty()){
        result.push_back(line);
    } 
    return result;
}

std::string CardRenderer::centerText(const std::string& text, int width) {
    if ((int)text.length() >= width) return text.substr(0, width);
    int padding = (width - text.length()) / 2;
    return std::string(padding, ' ') + text + std::string(width - text.length() - padding, ' ');
}

std::string CardRenderer::formatEffect(const Effet& effet) {
    std::string result;
    
    switch (effet.getTypeEffet()) {
        case TypeEffet::AJOUT_OR:
            result = "+" + std::to_string(effet.getMontant1()) + " or";
            break;
        case TypeEffet::AJOUT_COMBAT:
            result = "+" + std::to_string(effet.getMontant1()) + " combat";
            break;
        case TypeEffet::AJOUT_VIE:
            result = "+" + std::to_string(effet.getMontant1()) + " vie";
            break;
        case TypeEffet::PIOCHER:
            result = "Piocher " + std::to_string(effet.getMontant1());
            break;
        case TypeEffet::DEFAUSSER:
            result = "Defausser " + std::to_string(effet.getMontant1());
            break;
        case TypeEffet::SACRIFIER:
            result = "Sacrifier";
            break;
        case TypeEffet::ASSOMMER_CHAMPION:
            result = "Assommer un champion";
            break;
        case TypeEffet::PREPARE_CHAMPION:
            result = "Preparer un champion";
            break;
        case TypeEffet::COMBAT_OU_VIE:
            result = "+" + std::to_string(effet.getMontant1()) + " combat ou "+ std::to_string(effet.getMontant2()) + " vie";
            break;
        case TypeEffet::OR_OU_COMBAT:
            result = "+" + std::to_string(effet.getMontant1()) + " or ou "+ std::to_string(effet.getMontant2()) + " combat";
            break;
        case TypeEffet::COMBAT_PAR_CHAMPION:
            result = "+" + std::to_string(effet.getMontant1()) + " combat par champion";
            break;
        case TypeEffet::COMBAT_PAR_GARDE:
            result = "+" + std::to_string(effet.getMontant1()) + " combat par garde";
            break;
        case TypeEffet::COMBAT_PAR_ALLIE:
            result = "+" + std::to_string(effet.getMontant1()) + " combat par allie";
            break;
        case TypeEffet::VIE_PAR_CHAMPION:
            result = "+" + std::to_string(effet.getMontant1()) + " vie par champion";
            break;
        case TypeEffet::OR_OU_VIE_PAR_CHAMPION:
            result = "+" + std::to_string(effet.getMontant1()) + " or ou " + std::to_string(effet.getMontant2()) + " vie par champion";
            break;
        case TypeEffet::DEFAUSSE_AU_DESSUS_DU_DECK:
            result = "Defausse au dessus du deck";
            break;
        case TypeEffet::CHAMPION_DEFAUSSE_AU_DESSUS_DECK:
            result = "Champion defausse au dessus du deck";
            break;
        case TypeEffet::PROCHAIN_ACHAT_ACTION_DESSUS_DECK:
            result = "Prochain achat: action au dessus du deck";
            break;
        case TypeEffet::PROCHAIN_ACHAT_DANS_MAIN:
            result = "Prochain achat: dans la main";
            break;
        case TypeEffet::SACRIFIER_SIOUI_AJOUT_COMBAT:
            result = "Sacrifier, si oui -> +" + std::to_string(effet.getMontant1()) + " combat";
            break;
        case TypeEffet::PIOCHE_SIOUI_DEFAUSSE:
            result = "Piocher, si oui -> defausse " + std::to_string(effet.getMontant1());;
            break;

        default:
            result = "Effet special";
            break;
    }
    
    return result;
}

std::string CardRenderer::getFactionSymbol(Faction faction) {
    switch (faction) {
        case Faction::IMPERIALE: return "🛡️";
        case Faction::SAUVAGE: return "🐺";
        case Faction::GUILDE: return "⚖️";
        case Faction::NECROS: return "💀";
        case Faction::AUCUNE: return "⚪";
        default: return "❓";
    }
}