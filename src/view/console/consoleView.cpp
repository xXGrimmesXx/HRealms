#include "consoleView.h"
#include "../../model/partie.h"
#include "../../model/joueur.h"
#include "../../model/marche.h"
#include "../../model/carte.h"


// helper pour lire les lignes avec getline en évitant les retours à la ligne
static std::string trimString(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r' || s[start] == '\n')) ++start;
    size_t end = s.size();
    while (end > start && (s[end-1] == ' ' || s[end-1] == '\t' || s[end-1] == '\r' || s[end-1] == '\n')) --end;
    return s.substr(start, end - start);
}

ConsoleView::ConsoleView() {
}

ConsoleView::~ConsoleView() {
    std::cout << "ConsoleView détruite." << std::endl;
}

void ConsoleView::afficherMessage(std::string& message) {
    std::cout << "📢 " << message << std::endl;
}

void ConsoleView::afficherErreur(std::string& erreur) {
    std::cout << "❌ ERREUR: " << erreur << std::endl;
}

void ConsoleView::afficherPartie(Partie& partie) {
    //viderEcran();
    std::cout << "═══════════════════════════════════════" << std::endl;
    std::cout << "🏰 HERO REALMS - Tour " << partie.getNbTour() << std::endl;
    std::cout << "═══════════════════════════════════════" << std::endl;
    
    // Afficher le joueur actuel
     Joueur& joueurActuel = partie.getJoueurActuel();
    std::cout << "🎮 Joueur actuel: " << joueurActuel.getNom() << std::endl;
    std::cout << "❤️  Vie: " << joueurActuel.getPv() << " | ";
    std::cout << "💰 Or: " << partie.getTourCourant()->getOr() << " | ";
    std::cout << "⚔️  Combat: " << partie.getTourCourant()->getDegats() << std::endl;
    std::cout << std::endl;
}

void ConsoleView::afficherJoueur( Joueur& joueur) {
    std::cout << "👤 " << joueur.getNom() << " - ";
    std::cout << "❤️ " << joueur.getPv() << " | ";
}

void ConsoleView::afficherMain( Joueur& joueur) {
    std::cout << "🎴 Main de " << joueur.getNom() << ":" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;
    
    int taille = joueur.getMain().getTaille();
    std::cout << "🎴 Cartes en main: " << taille << std::endl;
    const auto& main = joueur.getMain();

    // On convertit les enums en string pour un affichage lisible
    auto typeEffetToString = [](TypeEffet t) -> std::string {
        switch (t) {
            case TypeEffet::AJOUT_OR: return "Ajout Or";
            case TypeEffet::AJOUT_COMBAT: return "Ajout Combat";
            case TypeEffet::AJOUT_VIE: return "Ajout Vie";
            case TypeEffet::PIOCHER: return "Piocher";
            case TypeEffet::DEFAUSSER: return "Défausser";
            case TypeEffet::SACRIFIER: return "Sacrifier";
            case TypeEffet::ASSOMMER_CHAMPION: return "Assommer Champion";
            case TypeEffet::PREPARE_CHAMPION: return "Préparer Champion";
            case TypeEffet::COMBAT_OU_VIE: return "Combat ou Vie";
            case TypeEffet::OR_OU_COMBAT: return "Or ou Combat";
            case TypeEffet::COMBAT_PAR_CHAMPION: return "Combat par Champion";
            case TypeEffet::COMBAT_PAR_GARDE: return "Combat par Garde";
            case TypeEffet::COMBAT_PAR_ALLIE: return "Combat par Allié";
            case TypeEffet::VIE_PAR_CHAMPION: return "Vie par Champion";
            case TypeEffet::OR_OU_VIE_PAR_CHAMPION: return "Or ou Vie par Champion";
            case TypeEffet::DEFAUSSE_AU_DESSUS_DU_DECK: return "Défausse au dessus du deck";
            case TypeEffet::CHAMPION_DEFAUSSE_AU_DESSUS_DECK: return "Champion défausse au dessus deck";
            case TypeEffet::PROCHAIN_ACHAT_ACTION_DESSUS_DECK: return "Prochain achat action dessus deck";
            case TypeEffet::PROCHAIN_ACHAT_DANS_MAIN: return "Prochain achat dans main";
            case TypeEffet::SACRIFIER_SIOUI_AJOUT_COMBAT: return "Sacrifier, si oui -> Ajout Combat";
            case TypeEffet::PIOCHE_SIOUI_DEFAUSSE: return "Piocher, si oui -> Défausse";
            default: return "Effet inconnu";
        }
    };

    auto typeCibleToString = [](TypeCible c) -> std::string {
        switch (c) {
            case TypeCible::SOI: return "Soi";
            case TypeCible::ADVERSAIRE: return "Adversaire";
            case TypeCible::CHAMPION: return "Champion";
            case TypeCible::TOUS_CHAMPIONS: return "Tous champions";
            case TypeCible::CARTE: return "Carte";
            case TypeCible::NIMPORTE: return "N'importe";
            case TypeCible::AUCUNE: return "Aucune";
            default: return "Cible inconnue";
        }
    };

    // Pour chaque carte, afficher ses détails
    for (int i = 0; i < taille; ++i) {
        Carte* c = main.getCarte(i);
        if (!c) {
            
            std::cout << "[" << i + 1 << "] <vide>" << std::endl;
            continue;
        }
        if (c->getEstJouee()) {
                std::cout << RED; 
        }

        // Type de carte
        std::string typeStr = "Inconnu";
        if (dynamic_cast<Champion*>(c)) typeStr = "Champion";
        else if (dynamic_cast<CarteObjet*>(c)) typeStr = "Objet";
        else if (dynamic_cast<CarteAction*>(c)) typeStr = "Action";

        // Son indice, nom, et faction
        std::cout << "[" << i + 1 << "] " << typeStr << " - " << c->getNom();
        std::cout << " - " << getFactionSymbol(c->getFaction()) << std::endl;

        // Effets primaires
        const auto prim = c->getEffetsPrimaire();
        if (!prim.empty()) {
            std::cout << "   Effets primaires:" << std::endl;
            for (const auto& e : prim) {
                std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                          << " (" << e.getMontant1();
                if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                std::cout << ") cible: " << typeCibleToString(e.getTypeCible()) << std::endl;
            }
        }

        // Effets alliés
        const auto alli = c->getEffetsAllie();
        if (!alli.empty()) {
            std::cout << "   Effets alliés:" << std::endl;
            for (const auto& e : alli) {
                std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                          << " (" << e.getMontant1();
                if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                std::cout << ") cible: " << typeCibleToString(e.getTypeCible()) << std::endl;
            }
        }

        // Effets sacrifice
        const auto sac = c->getEffetsSacrifice();
        if (!sac.empty()) {
            std::cout << "   Effets de sacrifice:" << std::endl;
            for (const auto& e : sac) {
                std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                          << " (" << e.getMontant1();
                if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                std::cout << ") cible: " << typeCibleToString(e.getTypeCible()) << std::endl;
            }
        }

        // Si champion, afficher ses effets d'activation
        if (Champion* ch = dynamic_cast<Champion*>(c)) {
            const auto act = ch->getEffetsActiver();
            if (!act.empty()) {
                std::cout << "   Effets d'activation:" << std::endl;
                for (const auto& e : act) {
                    std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                              << " (" << e.getMontant1();
                    if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                    std::cout << ") cible: " << typeCibleToString(e.getTypeCible()) << std::endl;
                }
            }
        }
        std::cout << RESET;
    }
    std::cout << std::endl;
}

void ConsoleView::afficherMarche( Marche& marche) {
    std::cout << "🏪 MARCHÉ:" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;
    int taille = marche.getLigne().size();
    std::vector<Carte*> ligne = marche.getLigne();

    // On convertit les enums en string pour un affichage lisible
    auto typeEffetToString = [](TypeEffet t) -> std::string {
        switch (t) {
            case TypeEffet::AJOUT_OR: return "Ajout Or";
            case TypeEffet::AJOUT_COMBAT: return "Ajout Combat";
            case TypeEffet::AJOUT_VIE: return "Ajout Vie";
            case TypeEffet::PIOCHER: return "Piocher";
            case TypeEffet::DEFAUSSER: return "Défausser";
            case TypeEffet::SACRIFIER: return "Sacrifier";
            case TypeEffet::ASSOMMER_CHAMPION: return "Assommer Champion";
            case TypeEffet::PREPARE_CHAMPION: return "Préparer Champion";
            case TypeEffet::COMBAT_OU_VIE: return "Combat ou Vie";
            case TypeEffet::OR_OU_COMBAT: return "Or ou Combat";
            case TypeEffet::COMBAT_PAR_CHAMPION: return "Combat par Champion";
            case TypeEffet::COMBAT_PAR_GARDE: return "Combat par Garde";
            case TypeEffet::COMBAT_PAR_ALLIE: return "Combat par Allié";
            case TypeEffet::VIE_PAR_CHAMPION: return "Vie par Champion";
            case TypeEffet::OR_OU_VIE_PAR_CHAMPION: return "Or ou Vie par Champion";
            case TypeEffet::DEFAUSSE_AU_DESSUS_DU_DECK: return "Défausse au dessus du deck";
            case TypeEffet::CHAMPION_DEFAUSSE_AU_DESSUS_DECK: return "Champion défausse au dessus deck";
            case TypeEffet::PROCHAIN_ACHAT_ACTION_DESSUS_DECK: return "Prochain achat action dessus deck";
            case TypeEffet::PROCHAIN_ACHAT_DANS_MAIN: return "Prochain achat dans main";
            case TypeEffet::SACRIFIER_SIOUI_AJOUT_COMBAT: return "Sacrifier, si oui -> Ajout Combat";
            case TypeEffet::PIOCHE_SIOUI_DEFAUSSE: return "Piocher, si oui -> Défausse";
            default: return "Effet inconnu";
        }
    };

    // Pour chaque carte, afficher ses détails
    for (int i = 0; i < taille; ++i) {
        Carte* c = ligne.at(i);
        if (!c) {
            std::cout << "[" << i + 1 << "] <vide>" << std::endl;
            continue;
        }

        // Type de carte
        std::string typeStr = "Inconnu";
        if (dynamic_cast<Champion*>(c)) typeStr = "Champion";
        else if (dynamic_cast<CarteObjet*>(c)) typeStr = "Objet";
        else if (dynamic_cast<CarteAction*>(c)) typeStr = "Action";

        // Son indice, nom, et faction
        std::cout << "[" << i + 1 << "] " << typeStr << " - " << c->getNom();
        std::cout << " - " << getFactionSymbol(c->getFaction()) << std::endl;
        std::cout << "   Coût: " << c->getCout() << " Or" << std::endl;

        // Effets primaires
        const auto prim = c->getEffetsPrimaire();
        if (!prim.empty()) {
            std::cout << "   Effets primaires:" << std::endl;
            for (const auto& e : prim) {
                std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                          << " (" << e.getMontant1();
                if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                std::cout << ")" << std::endl;
            }
        }

        // Effets alliés
        const auto alli = c->getEffetsAllie();
        if (!alli.empty()) {
            std::cout << "   Effets alliés:" << std::endl;
            for (const auto& e : alli) {
                std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                          << " (" << e.getMontant1();
                if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                std::cout << ")" << std::endl;
            }
        }

        // Effets sacrifice
        const auto sac = c->getEffetsSacrifice();
        if (!sac.empty()) {
            std::cout << "   Effets de sacrifice:" << std::endl;
            for (const auto& e : sac) {
                std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                          << " (" << e.getMontant1();
                if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                std::cout << ")" << std::endl;
            }
        }

        // Si champion, afficher ses effets d'activation
        if (Champion* ch = dynamic_cast<Champion*>(c)) {
            const auto act = ch->getEffetsActiver();
            if (!act.empty()) {
                std::cout << "   Effets d'activation:" << std::endl;
                for (const auto& e : act) {
                    std::cout << "     - " << typeEffetToString(e.getTypeEffet())
                              << " (" << e.getMontant1();
                    if (e.getMontant2() != 0) std::cout << ", " << e.getMontant2();
                    std::cout << ")" << std::endl;
                }
            }
        }
    }
    std::cout << std::endl;
}

void ConsoleView::afficherScores(std::vector<Joueur>& joueurs) {
    std::cout << "🏆 SCORES ACTUELS:" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;

    for (size_t i = 0; i < joueurs.size(); ++i) {
        afficherJoueur(joueurs.at(i));
    }

    std::cout << "\n" << std::endl;
}

void ConsoleView::afficherGagnant( Joueur& gagnant) {
    std::cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << std::endl;
    std::cout << "🏆 VICTOIRE DE " << gagnant.getNom() << " ! 🏆" << std::endl;
    std::cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << std::endl;
    std::cout << "Score final: " << gagnant.getPv() << " points de vie" << std::endl;
}

int ConsoleView::demanderNombreJoueurs() {
    int nombre = 0;
    std::string line;
    do {
        std::cout << "👥 Combien de joueurs ? (2-4): ";
        std::getline(std::cin, line);
        line = trimString(line);
        if (line.empty()) {
            std::string msg = "Veuillez entrer un nombre entre 2 et 4.";
            afficherErreur(msg);
            continue;
        }
        try {
            nombre = std::stoi(line);
        } catch (...) {
            std::string msg = "Entrée invalide. Veuillez entrer un nombre entre 2 et 4.";
            afficherErreur(msg);
            nombre = 0;
            continue;
        }
        if (nombre < 2 || nombre > 4) {
            std::string msg = "Veuillez entrer un nombre entre 2 et 4.";
            afficherErreur(msg);
            nombre = 0;
        }
    } while (nombre < 2 || nombre > 4);
    return nombre;
}

std::string ConsoleView::demanderNomJoueur(int numero) {
    std::string nom;
    do {
        std::cout << "👤 Nom du joueur " << numero << ": ";
        std::getline(std::cin, nom);
        nom = trimString(nom);
        if (nom.empty()) {
            std::string msg = "Le nom ne peut pas être vide.";
            afficherErreur(msg);
        }
    } while (nom.empty());
    return nom;
}

int ConsoleView::demanderChoixAction() {
    int choix;
    std::cout << "🎯 Choisissez votre action:" << std::endl;
    std::cout << "[1] Jouer une carte" << std::endl;
    std::cout << "[2] Acheter une carte" << std::endl;
    std::cout << "[3] Attaquer un joueur" << std::endl;
    std::cout << "[4] Finir le tour" << std::endl;
    std::cout << "Votre choix: ";
    
    
    if (!(std::cin >> choix)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    // retirer le reste de la ligne (y compris '\n') pour que getline() suivant fonctionne
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return choix;
}

std::vector<int> ConsoleView::demanderIndexCarte() {
    std::vector<int> indices;
    char * line;
    std::string input;
    std::cout << "🎴 Numéro de la carte (séparer les numeros pas des espaces): ";
    std::getline(std::cin, input, '\n');
    if (!input.empty()) {
        line = trimString(input).data();
        char * p;
        p = strtok(line, " " ); 
        while (p !=  NULL ) {
            std::cout << "Indice lu: " << p << std::endl;
            indices.push_back(std::stoi(p) - 1); // Convertir en index 0-based
            p = strtok( NULL , " " );
        }
    }
    return indices;
    
}

int ConsoleView::demanderIndexJoueur(std::vector<Joueur>& joueurs) {
    int index;
    std::cout << "👤 Numéro du joueur: ";
    for(size_t i=0;i<joueurs.size();i++){
        std::cout << "[" << i+1 << "] " << joueurs.at(i).getNom() << std::endl;
    }
    if (!(std::cin >> index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    // retirer le reste de la ligne
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return index - 1; // Convertir en index 0-based
}

int ConsoleView::demanderIndexChampion(Joueur& joueur) {
    int index;
    std::cout << "🛡️ Numéro du champion: ";
    for(size_t i=0;i<joueur.getChampions().size();i++){
        std::cout << "[" << i+1 << "] " << joueur.getChampions().at(i) << std::endl;
    }
    if (!(std::cin >> index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    // retirer le reste de la ligne
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return index - 1; // Convertir en index 0-based
}

int ConsoleView::demanderSlotMarche() {
    int slot;
    std::cout << "🏪 Numéro du slot dans le marché: ";
    if (!(std::cin >> slot)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    // retirer le reste de la ligne
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return slot - 1; // Convertir en index 0-based
}

bool ConsoleView::confirmerAction( std::string& action) {
    char reponse;
    std::cout << "❓ " << action << " (o/n): ";
    std::cin >> reponse;
    
    return (reponse == 'o' || reponse == 'O' || reponse == 'y' || reponse == 'Y');
}

void ConsoleView::attendreEntree() {
    std::cout << "Appuyez sur Entrée pour continuer...";
    // Si des caractères sont déjà disponibles dans le buffer, on les consomme
    // sans bloquer (pour éviter de consommer l'entrée qu'on attend).
    if (std::cin.rdbuf()->in_avail() > 0) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // Puis attendre une nouvelle ligne (bloque jusqu'à Enter)
    std::string dummy;
    std::getline(std::cin, dummy);
}

void ConsoleView::viderEcran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string ConsoleView::getFactionSymbol(Faction faction)  {
    switch (faction) {
        case Faction::IMPERIALE: return "🛡️ Impériale";
        case Faction::SAUVAGE: return "🐺 Sauvage";
        case Faction::GUILDE: return "⚖️ Guilde";
        case Faction::NECROS: return "💀 Nécros";
        case Faction::AUCUNE: return "⚪ Neutre";
        default: return "❓ Inconnue";
    }
}
