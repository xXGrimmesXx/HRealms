#include "consoleView.h"
#include "../../model/partie.h"
#include "../../model/joueur.h"
#include "../../model/marche.h"
#include "../../model/carte.h"
#include <iostream>
#include <iomanip>
#include <limits>

ConsoleView::ConsoleView() {
    // Constructeur
}

ConsoleView::~ConsoleView() {
    // Destructeur
}

void ConsoleView::afficherMessage(const std::string& message) {
    std::cout << "📢 " << message << std::endl;
}

void ConsoleView::afficherErreur(const std::string& erreur) {
    std::cout << "❌ ERREUR: " << erreur << std::endl;
}

void ConsoleView::afficherPartie(const Partie& partie) {
    viderEcran();
    std::cout << "═══════════════════════════════════════" << std::endl;
    std::cout << "🏰 HERO REALMS - Tour " << partie.getTourActuel() << std::endl;
    std::cout << "═══════════════════════════════════════" << std::endl;
    
    // Afficher le joueur actuel
    const Joueur& joueurActuel = partie.getJoueurActuel();
    std::cout << "🎮 Joueur actuel: " << joueurActuel.getNom() << std::endl;
    std::cout << "❤️  Vie: " << joueurActuel.getVie() << " | ";
    std::cout << "💰 Or: " << joueurActuel.getOr() << " | ";
    std::cout << "⚔️  Combat: " << joueurActuel.getCombat() << std::endl;
    std::cout << std::endl;
}

void ConsoleView::afficherJoueur(const Joueur& joueur) {
    std::cout << "👤 " << joueur.getNom() << " - ";
    std::cout << "❤️ " << joueur.getVie() << " | ";
    std::cout << "💰 " << joueur.getOr() << " | ";
    std::cout << "⚔️ " << joueur.getCombat() << std::endl;
}

void ConsoleView::afficherMain(const Joueur& joueur) {
    std::cout << "🎴 Main de " << joueur.getNom() << ":" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;
    
    // Note: Vous devrez adapter selon votre implémentation de Joueur
    // const auto& main = joueur.getMain();
    // for (size_t i = 0; i < main.size(); ++i) {
    //     std::cout << "[" << i + 1 << "] " << main[i]->getNom();
    //     std::cout << " (Coût: " << main[i]->getCout() << ")";
    //     std::cout << " - " << getFactionSymbol(main[i]->getFaction()) << std::endl;
    // }
    
    std::cout << "🎴 Cartes en main: " << joueur.getNombreCartesEnMain() << std::endl;
    std::cout << std::endl;
}

void ConsoleView::afficherMarche(const Marche& marche) {
    std::cout << "🏪 MARCHÉ:" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;
    
    // Note: Vous devrez adapter selon votre implémentation de Marche
    // const auto& cartes = marche.getCartesDisponibles();
    // for (size_t i = 0; i < cartes.size(); ++i) {
    //     if (cartes[i] != nullptr) {
    //         std::cout << "[" << i + 1 << "] " << cartes[i]->getNom();
    //         std::cout << " (Coût: " << cartes[i]->getCout() << ")";
    //         std::cout << " - " << getFactionSymbol(cartes[i]->getFaction()) << std::endl;
    //     } else {
    //         std::cout << "[" << i + 1 << "] Slot vide" << std::endl;
    //     }
    // }
    
    std::cout << std::endl;
}

void ConsoleView::afficherScores(const std::vector<Joueur>& joueurs) {
    std::cout << "🏆 SCORES ACTUELS:" << std::endl;
    std::cout << "───────────────────────────────────────" << std::endl;
    
    for (const auto& joueur : joueurs) {
        std::cout << joueur.getNom() << ": " << joueur.getVie() << " PV" << std::endl;
    }
    std::cout << std::endl;
}

void ConsoleView::afficherGagnant(const Joueur& gagnant) {
    std::cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << std::endl;
    std::cout << "🏆 VICTOIRE DE " << gagnant.getNom() << " ! 🏆" << std::endl;
    std::cout << "🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉" << std::endl;
    std::cout << "Score final: " << gagnant.getVie() << " points de vie" << std::endl;
}

int ConsoleView::demanderNombreJoueurs() {
    int nombre;
    do {
        std::cout << "👥 Combien de joueurs ? (2-4): ";
        std::cin >> nombre;
        
        if (std::cin.fail() || nombre < 2 || nombre > 4) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            afficherErreur("Veuillez entrer un nombre entre 2 et 4.");
        }
    } while (nombre < 2 || nombre > 4);
    
    return nombre;
}

std::string ConsoleView::demanderNomJoueur(int numero) {
    std::string nom;
    std::cout << "👤 Nom du joueur " << numero << ": ";
    std::cin.ignore(); // Ignorer le retour à la ligne restant
    std::getline(std::cin, nom);
    
    while (nom.empty()) {
        afficherErreur("Le nom ne peut pas être vide.");
        std::cout << "👤 Nom du joueur " << numero << ": ";
        std::getline(std::cin, nom);
    }
    
    return nom;
}

int ConsoleView::demanderChoixAction() {
    int choix;
    std::cout << "🎯 Choisissez votre action:" << std::endl;
    std::cout << "[1] Jouer une carte" << std::endl;
    std::cout << "[2] Acheter une carte" << std::endl;
    std::cout << "[3] Attaquer un joueur" << std::endl;
    std::cout << "[4] Finir le tour" << std::endl;
    std::cout << "[5] Afficher ma main" << std::endl;
    std::cout << "[6] Afficher le marché" << std::endl;
    std::cout << "Votre choix: ";
    
    std::cin >> choix;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    
    return choix;
}

int ConsoleView::demanderIndexCarte() {
    int index;
    std::cout << "🎴 Numéro de la carte: ";
    std::cin >> index;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    
    return index - 1; // Convertir en index 0-based
}

int ConsoleView::demanderSlotMarche() {
    int slot;
    std::cout << "🏪 Numéro du slot dans le marché: ";
    std::cin >> slot;
    
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    
    return slot - 1; // Convertir en index 0-based
}

bool ConsoleView::confirmerAction(const std::string& action) {
    char reponse;
    std::cout << "❓ " << action << " (o/n): ";
    std::cin >> reponse;
    
    return (reponse == 'o' || reponse == 'O' || reponse == 'y' || reponse == 'Y');
}

void ConsoleView::attendreEntree() {
    std::cout << "Appuyez sur Entrée pour continuer...";
    std::cin.ignore();
    std::cin.get();
}

void ConsoleView::viderEcran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string ConsoleView::getFactionSymbol(Faction faction) const {
    switch (faction) {
        case Faction::IMPERIALE: return "🛡️ Impériale";
        case Faction::SAUVAGE: return "🐺 Sauvage";
        case Faction::GUILDE: return "⚖️ Guilde";
        case Faction::NECROS: return "💀 Nécros";
        case Faction::AUCUNE: return "⚪ Neutre";
        default: return "❓ Inconnue";
    }
}
