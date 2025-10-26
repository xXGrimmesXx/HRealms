#include "consoleView.h"
#include "../../model/partie.h"
#include "../../model/joueur.h"
#include "../../model/marche.h"
#include "../../model/carte.h"
#include "../../model/champion.h"
#include "cardRenderer.h"


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
    std::cout << "📚 Pioche: " << joueurActuel.getDeck().getTaille() << " cartes | ";
    std::cout << "🗂️  Défausse: " << joueurActuel.getDefausse().getTaille() << " cartes" << std::endl;
    std::cout << std::endl;
}

void ConsoleView::afficherJoueur( Joueur& joueur) {
    std::cout << "👤 " << joueur.getNom() << " - ";
    std::cout << "❤️ " << joueur.getPv() << " | ";
}

std::string ConsoleView::typeEffetToString (TypeEffet t) {
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
}

std::string ConsoleView::typeCibleToString(TypeCible c) {
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

void ConsoleView::afficherPileGemmeDeFeu(const std::vector<Carte*>& pile) {
    std::cout << "💎 GEMMES DE FEU disponibles: " << pile.size() << std::endl;
    if (!pile.empty()) {
        CardRenderer::printCards(std::vector<Carte*>{pile.back()}, 1);
    }
    std::cout << "───────────────────────────────────────" << std::endl;
}

void ConsoleView::afficherDefausse(const Joueur& joueur) {
    std::cout << "🗂️ Défausse de " << joueur.getNom() << ":" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;
    
    const Pile& defausse = joueur.getDefausse();
    int taille = defausse.getTaille();
    
    if (taille == 0) {
        std::cout << "Défausse vide." << std::endl;
        return;
    }
    
    std::cout << "Nombre de cartes: " << taille << std::endl;
    const auto& cartes = defausse.getCartes();
    
    const int CARDS_PER_ROW = 5;
    CardRenderer::printCards(cartes, CARDS_PER_ROW);
    
    std::cout << "───────────────────────────────────────" << std::endl;
}

void ConsoleView::afficherDeckMarche(Deck& deckMarche) {
    std::cout << "🗂️ Deck du marché:" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;

    int taille = deckMarche.getTaille();

    if (taille == 0) {
        std::cout << "marché vide." << std::endl;
        return;
    }
    
    std::cout << "Nombre de cartes: " << taille << std::endl;
    const auto& cartes = deckMarche.getCartes();
    const int CARDS_PER_ROW = 5;

    CardRenderer::printCards(cartes, CARDS_PER_ROW);

    std::cout << "───────────────────────────────────────" << std::endl;
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

int ConsoleView::demanderChoixAction(Partie& partie) {
    std::cout << "🎯 Choisissez votre action:" << std::endl;
    
    // Récupérer les informations nécessaires
    Joueur& joueur = partie.getJoueurActuel();
    Tour* tour = partie.getTourCourant();
    Marche* marche = partie.getMarche();
    
    // Construire un mapping entre numéro affiché et action réelle
    std::vector<int> actionsDisponibles;
    int numeroAffiche = 1;
    
    // Seulement si le joueur a des cartes en main
    if (joueur.getNombreCartesJouables() > 0) {
        std::cout << "[" << numeroAffiche << "] Jouer une carte" << std::endl;
        actionsDisponibles.push_back(1);
        numeroAffiche++;
    }
    
    // Seulement s'il y a des champions
    if (joueur.getChampions().size() > 0) {
        bool aChampionMobilisable = std::any_of(
            joueur.getChampions().begin(), 
            joueur.getChampions().end(),
            [](Champion* c) { return c && !c->estMobilise(); }
        );
        
        if (aChampionMobilisable) {
            std::cout << "[" << numeroAffiche << "] Mobiliser un champion" << std::endl;
            actionsDisponibles.push_back(2);
            numeroAffiche++;
        }
    }
    
    // Seulement si le joueur a de l'or et qu'il y a des cartes au marché (cas très rare où toutes les cartes ont été jouées)
    if (tour && tour->getOr() > 0 && marche && marche->getLigne().size() > 0) {
        // Vérifier s'il y a au moins une carte que le joueur peut se payer
        bool carteAbordable = false;
        for (const auto& carte : marche->getLigne()) {
            if (carte && carte->getCout() <= tour->getOr()) {
                carteAbordable = true;
                break;
            }
        }
        
        if (carteAbordable) {
            std::cout << "[" << numeroAffiche << "] Acheter une carte" << std::endl;
            actionsDisponibles.push_back(3);
            numeroAffiche++;
        }
    }

    // Seulement si le joueur a au moins 2 d'or et qu'il y a des gemmes
    if (tour && tour->getOr() >= 2 && partie.getGemmeDeFeu().getPile().getTaille() > 0) {
        std::cout << "[" << numeroAffiche << "] Acheter une Gemme de Feu" << std::endl;
        actionsDisponibles.push_back(4);
        numeroAffiche++;
    }
    
    // Attaquer un joueur - seulement si le joueur a des dégâts
    if (tour && tour->getDegats() > 0) {
        std::cout << "[" << numeroAffiche << "] Attaquer un joueur" << std::endl;
        actionsDisponibles.push_back(5);
        numeroAffiche++;
    }
    
    // Seulement si le joueur à au moins une carte dans sa défausse
    if (joueur.getDefausse().getTaille() > 0) {
        std::cout << "[" << numeroAffiche << "] Consulter sa défausse" << std::endl;
        actionsDisponibles.push_back(6);
        numeroAffiche++;
    }

    // Consulter main adversaire - TOUJOURS DISPONIBLE
    std::cout << "[" << numeroAffiche << "] Consulter main adversaire" << std::endl;
    actionsDisponibles.push_back(7);
    numeroAffiche++;
    
    // Finir le tour - toujours disponible
    std::cout << "[" << numeroAffiche << "] Finir le tour" << std::endl;
    actionsDisponibles.push_back(8);
    numeroAffiche++;
    
    // GodMode - toujours disponible
    std::cout << "[" << numeroAffiche << "] GodMode" << std::endl;
    actionsDisponibles.push_back(9);
    std::cout << "Votre choix: ";
    
    int choixAffiche;
    if (!(std::cin >> choixAffiche)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    // retirer le reste de la ligne (y compris '\n') pour que getline() suivant fonctionne
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Si choix invalide
    if (choixAffiche < 1 || choixAffiche > (int)actionsDisponibles.size()) {
        std::cout << "❌ ERREUR: Choix invalide. Veuillez choisir entre 1 et " << actionsDisponibles.size() << "." << std::endl;
        return -1;
    }

    // Convertir le choix affiché en action réelle
    return actionsDisponibles[choixAffiche - 1];
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
            indices.push_back(std::stoi(p) - 1); // Convertir en index 0-based
            p = strtok( NULL , " " );
        }
    }
    return indices;
    
}

std::vector<int> ConsoleView::demanderIndexChampion(Joueur& joueur) {
    std::vector<int> indices;
    char * line;
    std::string input;
    if (joueur.getChampions().empty()) {
        indices.push_back(-1);
        return indices; // Retourner vide si pas de champions
    }
    afficherChampions(joueur.getChampions());
    std::cout << "🛡️ Numéro du champion (séparer les numeros pas des espaces): ";
    std::getline(std::cin, input, '\n');
    if (!input.empty()) {
        line = trimString(input).data();
        char * p;
        p = strtok(line, " " ); 
        while (p !=  NULL ) {
            indices.push_back(std::stoi(p) - 1); // Convertir en index 0-based
            p = strtok( NULL , " " );
        }
    }
    return indices;
    
}

std::vector<int> ConsoleView::demanderIndexGarde(Joueur& joueur) {
    std::vector<int> indices;
    char * line;
    std::string input;
    if (joueur.getGardes().empty()) {
        return indices; // Retourner vide si pas de gardes
    }
    afficherChampions(joueur.getGardes());
    std::cout << "🛡️ Numéro du garde (séparer les numeros pas des espaces): ";
    std::getline(std::cin, input, '\n');
    if (!input.empty()) {
        line = trimString(input).data();
        char * p;
        p = strtok(line, " " ); 
        while (p !=  NULL ) {
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

int ConsoleView::demanderIndexJoueur(std::vector<Joueur>& joueurs,int joueurActuelIndex) {
    int index;
    std::cout << "👤 Numéro du joueur: ";
    for(size_t i=0;i<joueurs.size();i++){
        if((int)i==joueurActuelIndex) continue; // ne pas afficher le joueur actuel
        std::cout << "[" << i+1 << "] " << joueurs.at(i).getNom() << std::endl;
    }
    if (!(std::cin >> index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    if (index-1 == joueurActuelIndex) {
        throw std::invalid_argument("Vous ne pouvez pas choisir vous-même comme cible.");
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

int ConsoleView::demanderChoixGodMode() {
    std::cout << "🔧 GOD MODE:" << std::endl;
    std::cout << "[1] Mettre tous à 1 PV" << std::endl;
    std::cout << "[2] Acheter depuis deck" << std::endl;
    std::cout << "[3] Désactiver GodMode" << std::endl;
    std::cout << "Choix: ";

    int choix;
    if (!(std::cin>>choix)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choix;
}

int ConsoleView::demanderIndexDeck(Deck& deckMarche) {
    afficherDeckMarche(deckMarche);
    int index;
    std::cout << "📚 Index de la carte dans le deck du marché: ";
    if (!(std::cin >> index)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    // retirer le reste de la ligne
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return index - 1; // Convertir en index 0-based
}

std::string ConsoleView::demanderMotDePasseGodMode() {
    std::string motDePasse;
    std::cout << "🔐 Entrez le mot de passe God Mode: ";
    std::getline(std::cin, motDePasse);
    return motDePasse;
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

void ConsoleView::afficherMain( Joueur& joueur) {
    std::cout << "🎴 Main de " << joueur.getNom() << ":" << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
    
    const auto& main = joueur.getMain();
    int taille = main.getTaille();
    
    if (taille == 0) {
        std::cout << "Main vide." << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
        return;
    }

    // Afficher les cartes par groupes de 5 pour éviter que ça déborde
    const int CARDS_PER_ROW = 5;
    CardRenderer::printCards(main.getCartes(), CARDS_PER_ROW);

    std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
}

void ConsoleView::afficherMarche(Marche& marche) {
    std::cout << "🏪 MARCHÉ:" << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
    
    const auto& ligne = marche.getLigne();
    std::vector<std::vector<std::string>> cardsToRender;
    
   CardRenderer::printCards(ligne, 5);

    std::cout << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
}

void ConsoleView::afficherChampions(const std::vector<Champion*>& champions) {
    std::cout << "👑 CHAMPIONS EN JEU:" << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
    
    if (champions.empty()) {
        std::cout << "Aucun champion en jeu." << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
        return;
    }
    else{
        const int CARDS_PER_ROW = 5;
        // Convert Champion* vector to Carte* vector because CardRenderer expects Carte*
        std::vector<Carte*> cartes;
        cartes.reserve(champions.size());
        for (Champion* c : champions) {
            cartes.push_back(static_cast<Carte*>(c));
        }
        CardRenderer::printCards(cartes, CARDS_PER_ROW);
        std::cout << std::endl;
        std::cout << "═══════════════════════════════════════════════════════════════════════════════" << std::endl;
    }  
}