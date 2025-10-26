#include "gameController.h"
#include "../model/joueur.h"
#include "../model/marche.h"
#include "../model/carte.h"
#include <algorithm>
#include <stdexcept>

GameController::GameController(GameView* gameView) 
    : partie(nullptr), view(gameView), jeuEnCours(false) {
    if (!view) {
        throw std::invalid_argument("GameView ne peut pas être null");
    }
}

GameController::~GameController() {
    // unique_ptr se charge automatiquement de la destruction
}

void GameController::lancerJeu() {
    std::string message = "Bienvenue dans Hero Realms !";
    view->afficherMessage(message);
    
    try {
        initialiserPartie();
        boucleJeu();
        finirPartie();
    } catch (const std::exception& e) {
        std::string message = "Erreur fatale: " + std::string(e.what());
        view->afficherErreur(message);
    }
}

void GameController::initialiserPartie() {
    // Demander le nombre de joueurs
    int nombreJoueurs = view->demanderNombreJoueurs();
    
    // Demander les noms des joueurs (vérifier unicité)
    std::vector<std::string> nomsJoueurs;
    for (int i = 1; i <= nombreJoueurs; ++i) {
        std::string nom;
        do {
            nom = view->demanderNomJoueur(i);
            // Vérifier si ce nom est déjà utilisé
            auto it = std::find(nomsJoueurs.begin(), nomsJoueurs.end(), nom);
            if (it != nomsJoueurs.end()) {
                std::string msg = "Ce nom de joueur est déjà pris. Veuillez en choisir un autre.";
                view->afficherErreur(msg);
                nom.clear();
            }
        } while (nom.empty());
        nomsJoueurs.push_back(nom);
    }
    
    // Créer la partie

    partie = std::make_unique<Partie>();
    partie->commencer(nomsJoueurs);
    
    jeuEnCours = true;
    std::string message = "Partie initialisée avec " + std::to_string(nombreJoueurs) + " joueurs !";
    view->afficherMessage(message);
    view->attendreEntree();
}

void GameController::boucleJeu() {
    while (jeuEnCours && !partie->estTerminee()) {
        jouerTour();
    }
}

void GameController::jouerTour() {
    try {
        // Commencer le tour
        partie->commencerTour();
        
        // Afficher l'état du jeu
        afficherEtatJeu();

        // Gérer les actions du joueur
        bool tourTermine = false;
        while (!tourTermine && !partie->estTerminee()) {
            gererActionJoueur();
            
            if (partie->getTourCourant()==nullptr) {
                tourTermine = true;
            }
        }
        
    } catch (const std::exception& e) {
        std::string message = "Erreur pendant le tour: " + std::string(e.what());
        gererErreur(message);
    }
}

void GameController::gererActionJoueur() {
    int choix = view->demanderChoixAction(*partie);
    
    switch (choix) {
        case 1: // Jouer une carte
            if (!jouerCarte()) {
                std::string message = "Impossible de jouer cette carte.";
                view->afficherErreur(message);
            }
            afficherEtatJeu();
            break;
        case 2 ://mobiliser un champion
            if (!mobiliserChampion()) {
                std::string message = "Impossible de mobiliser ce champion.";
                view->afficherErreur(message);
            }
            afficherEtatJeu();
            break;
            
        case 3: // Acheter une carte
            if (!acheterCarte()) {
                std::string message = "Impossible d'acheter cette carte.";
                view->afficherErreur(message);
            }
            afficherEtatJeu();
            break;
        case 4: // Acheter une carte Gemme de feu
            if(!acheterGemmeDeFeu()){
                std::string message = "Impossible d'acheter cette carte.";
                view->afficherErreur(message);
            }
            afficherEtatJeu();
            break;
        case 5: // Attaquer un joueur
            if (!attaquerJoueur()) {
                std::string message = "Impossible d'attaquer ce joueur.";
                view->afficherErreur(message);
            }
            afficherEtatJeu();
            break;

        case 6: // Consulter sa défausse
            if(!consulterDefausse()){
                std::string message = "Impossible de consulter la défausse.";
                view->afficherErreur(message);
            }
            afficherEtatJeu();
            break;
        case 7: // Consulter la main d'un adversaire
            afficherMainAdversaire();
            afficherEtatJeu();
            break;
        case 8: // Finir le tour
            finirTour();
            break;
        case 9: // Activer le godMode
            toggleGodMode();
            afficherEtatJeu();
            break;
    }

}

bool GameController::jouerCarte() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    
    // Demander quelle carte jouer
    std::vector<int> index = view->demanderIndexCarte();

    for (int ind : index) {
        if (!validerIndex(ind,joueurActuel.getMain().getTaille(), "carte")) {
            return false;
        }
        Carte* c = joueurActuel.getMain().getCarte(ind);
        std::vector<void*> cibles = choisirCiblePrimaires(c);
        std::vector<void*> ciblesAllies = choisirCibleAllie(c);    
    
        // Essayer de jouer la carte
        try {
            bool succes = joueurActuel.jouerCarte(ind, *partie, cibles, ciblesAllies);
            if (succes) {
                std::string message = "Carte jouée avec succès !";
                view->afficherMessage(message);
            }
            else {
                std::string message = "Échec du jeu de la carte.";
                view->afficherErreur(message);
                return false;
            }
        } catch (const std::exception& e) {
            std::string message = "Erreur lors du jeu de carte: " + std::string(e.what());
            view->afficherErreur(message);
            return false;
        }
    }
    return true;
}

bool GameController::mobiliserChampion() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    
    // Demander quelle carte jouer
    std::vector<int> index = view->demanderIndexChampion(joueurActuel);

    for (int ind : index) {
        if (!validerIndex(ind,joueurActuel.getChampions().size(), "champion")) {
            return false;
        }
        Champion* c = joueurActuel.getChampions().at(ind);
        std::vector<void*> cibles = choisirCibleMobiliser(c);
    
        // Essayer de jouer la carte
        try {
            bool succes = joueurActuel.mobiliserChampion(ind, *partie, cibles);
            if (succes) {
                std::string message = "Champion mobilisé avec succès !";
                view->afficherMessage(message);
            }
            else {
                std::string message = "Échec de la mobilisation du champion.";
                view->afficherErreur(message);
                return false;
            }
        } catch (const std::exception& e) {
            std::string message = "Erreur lors de la mobilisation du champion: " + std::string(e.what());
            view->afficherErreur(message);
            return false;
        }
    }
    return true;

}

std::vector<void*> GameController::choisirCiblePrimaires(Carte* carte) {
    std::vector<void*> targets;
    for(size_t i=0;i<carte->getEffetsPrimaire().size();i++){
        switch(carte->getEffetsPrimaire().at(i).getTypeCible()){
            case TypeCible::SOI :
                targets.push_back(&partie->getJoueurActuel());
                break;

            case TypeCible::ADVERSAIRE:
                // Demander quel joueur attaquer
                {
                int index = view->demanderIndexJoueur(partie->getJoueurs()); // Réutiliser cette méthode
                targets.push_back(&partie->getJoueurs().at( index ));
                }
                break;

            case TypeCible::CHAMPION_ALLIE:
                {
                int index = view->demanderIndexChampion(partie->getJoueurActuel()).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurActuel().getChampions().at(index));
                }
                break;

            case TypeCible::CHAMPION_ENNEMI:
                {
                int indexJoueur = view->demanderIndexJoueur(partie->getJoueurs()); // Réutiliser cette méthode
                Joueur j = partie->getJoueurs().at( indexJoueur );
                int index = view->demanderIndexChampion(j).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurs().at(indexJoueur).getChampions().at(index));
                }
                break;

            case TypeCible::TOUS_CHAMPIONS:
            {
                for(auto j : partie->getJoueurs()){
                    for(auto c : j.getChampions()){
                        targets.push_back(c);
                    }
                }
            }
                break;

            case TypeCible::CARTE:
                // Pour l'instant, on ne gère pas les cartes sur le terrain
                targets.push_back(nullptr);
                break;

            case TypeCible::NIMPORTE:
                targets.push_back(nullptr); // Ou une autre logique pour "n'importe"
                break;

            case TypeCible::AUCUNE:
                targets.push_back(nullptr);
                break;

            case TypeCible::CHAMPION:
                {
                int indexJoueur = view->demanderIndexJoueur(partie->getJoueurs()); // Réutiliser cette méthode
                Joueur j = partie->getJoueurs().at( indexJoueur );
                int index = view->demanderIndexChampion(j).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurs().at(indexJoueur).getChampions().at(index));
                }
                break;

            default:
                targets.push_back(nullptr);
                break;
        }
    }
    return targets;

}

std::vector<void*> GameController::choisirCibleAllie(Carte* carte) {
    std::vector<void*> targets;
    for(size_t i=0;i<carte->getEffetsAllie().size();i++){
        switch(carte->getEffetsAllie().at(i).getTypeCible()){
            case TypeCible::SOI :
                targets.push_back(&partie->getJoueurActuel());
                break;

            case TypeCible::ADVERSAIRE:
                // Demander quel joueur attaquer
                {
                int index = view->demanderIndexJoueur(partie->getJoueurs()); // Réutiliser cette méthode
                targets.push_back(&partie->getJoueurs().at( index ));
                }
                break;

            case TypeCible::CHAMPION_ALLIE:
                {
                int index = view->demanderIndexChampion(partie->getJoueurActuel()).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurActuel().getChampions().at(index));
                }
                break;

            case TypeCible::CHAMPION_ENNEMI:
                {
                int indexJoueur = view->demanderIndexJoueur(partie->getJoueurs()); // Réutiliser cette méthode
                Joueur j = partie->getJoueurs().at( indexJoueur );
                int index = view->demanderIndexChampion(j).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurs().at(indexJoueur).getChampions().at(index));
                }
                break;

            case TypeCible::TOUS_CHAMPIONS:
            {
                for(auto j : partie->getJoueurs()){
                    for(auto c : j.getChampions()){
                        targets.push_back(c);
                    }
                }
            }
                break;

            case TypeCible::CARTE:
                // Pour l'instant, on ne gère pas les cartes sur le terrain
                targets.push_back(nullptr);
                break;

            case TypeCible::NIMPORTE:
                targets.push_back(nullptr); // Ou une autre logique pour "n'importe"
                break;

            case TypeCible::AUCUNE:
                targets.push_back(nullptr);
                break;

            case TypeCible::CHAMPION:
                {
                int indexJoueur = view->demanderIndexJoueur(partie->getJoueurs());
                Joueur j = partie->getJoueurs().at( indexJoueur );
                int index = view->demanderIndexChampion(j).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurs().at(indexJoueur).getChampions().at(index));
                break;
                }
            }
        }
    return targets;
}

std::vector<void*> GameController::choisirCibleMobiliser(Champion* carte) {
    std::vector<void*> targets;
    for(size_t i=0;i<carte->getEffetsActiver().size();i++){
        switch(carte->getEffetsActiver().at(i).getTypeCible()){
            case TypeCible::SOI :
                targets.push_back(&partie->getJoueurActuel());
                break;

            case TypeCible::ADVERSAIRE:
                // Demander quel joueur attaquer
                {
                int index = view->demanderIndexJoueur(partie->getJoueurs()); // Réutiliser cette méthode
                targets.push_back(&partie->getJoueurs().at( index ));
                }
                break;

            case TypeCible::CHAMPION_ALLIE:
                {
                int index = view->demanderIndexChampion(partie->getJoueurActuel()).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurActuel().getChampions().at(index));
                }
                break;

            case TypeCible::CHAMPION_ENNEMI:
                {
                int indexJoueur = view->demanderIndexJoueur(partie->getJoueurs()); // Réutiliser cette méthode
                Joueur j = partie->getJoueurs().at( indexJoueur );
                int index = view->demanderIndexChampion(j).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurs().at(indexJoueur).getChampions().at(index));
                }
                break;

            case TypeCible::TOUS_CHAMPIONS:
            {
                for(auto j : partie->getJoueurs()){
                    for(auto c : j.getChampions()){
                        targets.push_back(c);
                    }
                }
            }
                break;

            case TypeCible::CARTE:
                // Pour l'instant, on ne gère pas les cartes sur le terrain
                targets.push_back(nullptr);
                break;

            case TypeCible::NIMPORTE:
                targets.push_back(nullptr); // Ou une autre logique pour "n'importe"
                break;

            case TypeCible::AUCUNE:
                targets.push_back(nullptr);
                break;

            case TypeCible::CHAMPION:
                {
                int indexJoueur = view->demanderIndexJoueur(partie->getJoueurs());
                Joueur j = partie->getJoueurs().at( indexJoueur );
                int index = view->demanderIndexChampion(j).at(0); // Réutiliser cette méthode
                targets.push_back(partie->getJoueurs().at(indexJoueur).getChampions().at(index));
                break;
                }
            }
        }
    return targets;
}

bool GameController::acheterCarte() {

    // Demander quelle carte acheter
    int slot = view->demanderSlotMarche();
    
    // Valider le slot
    if (!validerIndex(slot, 5, "slot du marché")) {
        return false;
    }
    
    // Essayer d'acheter la carte
    try {
        bool succes = partie->getJoueurActuel().acheterDepuisMarche(slot, *partie);
        if (succes) {
            std::string message = "Carte achetée avec succès !";
            view->afficherMessage(message);
        }
        return succes;
    } catch (const std::exception& e) {
        std::string message = "Erreur lors de l'achat: " + std::string(e.what());
        view->afficherErreur(message);
        return false;
    }
}

bool GameController::acheterGemmeDeFeu() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    
    bool succes = partie->getGemmeDeFeu().acheter(joueurActuel, *partie);
    
    if (succes) {
        std::string message = "Gemme de Feu achetée avec succès !";
        view->afficherMessage(message);
    }
    
    return succes;
}

bool GameController::attaquerJoueur() {
    std::vector<Joueur>& joueurs = partie->getJoueurs();
    
    
    // Demander quel joueur attaquer
    // si il n'y a qu'un seul adversaire, on l'attaque directement
    int index = view->demanderIndexJoueur(joueurs,partie->getIdxJoueurActuel()); // Réutiliser cette méthode
    
    // Valider l'index
    if (!validerIndex(index, static_cast<int>(joueurs.size()), "joueur")) {
        return false;
    }
    
    
    
    try {
        Joueur& cible = partie->getJoueurs().at(index);
        // si l'adversaire a un champion garde, on doit l'attaquer en premier
        if (cible.aGarde()){
            std::string message = joueurs.at(index).getNom() + " a au moins un champion garde. Lequel voulez-vous attaquer?";
            view->afficherMessage(message);
            std::vector<int> indexGarde = view->demanderIndexGarde(cible);
            for (int ind : indexGarde){
                if (!validerIndex(ind,cible.getChampions().size(), "champion garde")) {
                    return false;
                }
                Champion* garde = cible.getChampions().at(ind);
                if (!garde->estGarde()){
                    throw std::runtime_error("Le champion sélectionné n'est pas un garde.");
                    return false;
                }
                std::cout << garde->getVie() <<" : "<<partie->getTourCourant()->getDegats() << std::endl;
                if (garde->getVie()<=partie->getTourCourant()->getDegats()){
                    std::cout << "Attaque du garde " << garde->getNom() << std::endl;
                    cible.assomerChampion(garde);
                    partie->getTourCourant()->utiliserDegats(garde->getVie());
                    return true;
                }
                else {
                    std::string message = "Vous ne pouvez pas attaquer ce garde car vous n'avez pas assez d'attaques restantes.";
                    view->afficherErreur(message);
                    return false;
                }
            }
        }
        else {
            // Essayer d'attaquer le joueur
            bool succes = cible.prendreDegats(partie->getTourCourant()->getDegats());
            partie->getTourCourant()->utiliserDegats(partie->getTourCourant()->getDegats());
            std::string message = "Attaque réussie contre " + cible.getNom() + " !";
            view->afficherMessage(message);
            return succes;
        }
        
    } catch (const std::exception& e) {
        std::string message = "Erreur lors de l'attaque: " + std::string(e.what());
        view->afficherErreur(message);
        return false;
    }
    return false;
}

bool GameController::consulterDefausse() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    
    if (joueurActuel.getDefausse().getTaille() == 0) {
        std::string message = "Votre défausse est vide.";
        view->afficherMessage(message);
        return false;
    }
    
    view->afficherDefausse(joueurActuel);
    view->attendreEntree();
    return true;
}

void GameController::finirTour() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    if(joueurActuel.getNombreCartesJouables() > 0){
        std::string message = "Vous avez des cartes non jouées ou des ressources non utilisées. Êtes-vous sûr de vouloir finir votre tour ?";
        if (!view->confirmerAction(message)) {
            return;
        }
    }
    else if (partie->getTourCourant()->getDegats() > 0){
        std::string message = "Vous avez "+std::to_string(partie->getTourCourant()->getDegats())+" dégâts non utilisés. Êtes-vous sûr de vouloir finir votre tour ?";
        if (!view->confirmerAction(message)) {
            return;
        }
    }
    else if (partie->getTourCourant()->getOr() > 0){
        std::string message = "Vous avez "+std::to_string(partie->getTourCourant()->getOr())+" or non utilisé. Êtes-vous sûr de vouloir finir votre tour ?";
        if (!view->confirmerAction(message)) {
            return;
        }
    }
    
    std::string message = "Tour terminé.";
    view->afficherMessage(message);
    partie->finTour();
}

void GameController::toggleGodMode() {
    if (!partie->getGodMode().actif) {
        // GodMode inactif → demander mot de passe
        std::string motDePasse = view->demanderMotDePasseGodMode();
        
        if (motDePasse != "joelgaylegoat") {
            std::string message = "❌ Mot de passe incorrect.";
            view->afficherErreur(message);
            return;
        }
        
        // Activer le GodMode
        partie->toggleGodMode();
        std::string message = "🔧 GodMode ACTIVÉ";
        view->afficherMessage(message);
    }
    
    int choixGod = view->demanderChoixGodMode();
    
    if (choixGod == -1 || choixGod < 1 || choixGod > 3) {
        std::string message = "❌ Choix invalide.";
        view->afficherErreur(message);
        return;
    }
    executerActionGodMode(choixGod);
}

void GameController::executerActionGodMode(int choix) {
    switch (choix) {
        case 1: // Mettre tous à 1 PV
            partie->GOD_mettreTousJoueurs1PV();
            {
                std::string message = "Tous les joueurs ont maintenant 1 PV.";
                view->afficherMessage(message);
            }
            break;
        case 2: // Acheter depuis deck
            {
                Deck& deckMarche = partie->getMarche()->getDeckMarche();
                if (deckMarche.getTaille() == 0) {
                    std::string message = "Le deck du marché est vide.";
                    view->afficherErreur(message);
                    break;
                }
                
                int idx = view->demanderIndexDeck(deckMarche);
                if (idx >= 0 && idx < deckMarche.getTaille()) {
                    partie->GOD_acheterDepuisDeckMarche(idx, partie->getJoueurActuel());
                    std::string message = "Carte achetée depuis le deck.";
                    view->afficherMessage(message);
                } else {
                    std::string message = "Index invalide.";
                    view->afficherErreur(message);
                }
            }
            break;
        case 3: // Désactiver GodMode
            partie->toggleGodMode();
            {
                std::string message = "🔧 GodMode DÉSACTIVÉ";
                view->afficherMessage(message);
            }
            break;
    }
}

void GameController::afficherEtatJeu() {
    //view->viderEcran();
    view->afficherPartie(*partie);
    view->afficherScores(partie->getJoueurs());
    afficherMarche();
    afficherPileGemmeDeFeu();
    afficherMain();
    if(!partie->getJoueurActuel().getChampions().empty()){
        afficherChampions();
    }
    
}

void GameController::afficherMain() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    view->afficherMain(joueurActuel);
}

void GameController::afficherMarche() {
    Marche* marche = partie->getMarche();
    view->afficherMarche(*marche);
}

void GameController::afficherPileGemmeDeFeu() {
    const std::vector<Carte*>& pileGemmes = partie->getGemmeDeFeu().getPile().getCartes();
    view->afficherPileGemmeDeFeu(pileGemmes);
}

void GameController::afficherChampions() {
    const Joueur& joueurActuel = partie->getJoueurActuel();
    view->afficherChampions(joueurActuel.getChampions());
}

void GameController::afficherMainAdversaire() {
    std::vector<Joueur>& joueurs = partie->getJoueurs();
    
    // Filtrer pour ne montrer que les adversaires
    std::vector<Joueur> adversaires;
    for (auto& j : joueurs) {
        if (j.getNom() != partie->getJoueurActuel().getNom()) {
            adversaires.push_back(j);
        }
    }

    // S'il y a plus d'un adversaire, demander lequel afficher
    if(adversaires.size() > 1){
        
        int index = view->demanderIndexJoueur(adversaires);
        
        if (index < 0 || index >= (int)adversaires.size()) {
            std::string message = "Index invalide.";
            view->afficherErreur(message);
            return;
        }
        
        // Trouver le vrai joueur dans la liste complète
        for (auto& j : joueurs) {
            if (j.getNom() == adversaires.at(index).getNom()) {
                view->afficherMain(j);
                
                if (!j.getChampions().empty()) {
                    view->afficherChampions(j.getChampions());
                }
                
                view->attendreEntree();
                return;
            }
        }
    }
    // S'il n'y a qu'un seul adversaire
    else{
        Joueur& j = adversaires.at(0);
        view->afficherMain(j);
        view->attendreEntree();
    }
}

void GameController::finirPartie() {
    if (partie && partie->estTerminee()) {
        Joueur* gagnant = partie->getGagnant();
        if(gagnant!=nullptr){
            view->afficherGagnant(*gagnant);
            std::string message = "Merci d'avoir joué à Hero Realms !";
            view->afficherMessage(message);
            jeuEnCours = false;
        }
        else{
            std::string message = "La partie s'est terminée sans gagnant clair.";
            view->afficherMessage(message);
            jeuEnCours = false;
        }

        
    }
    
    
}

bool GameController::validerIndex(int index, int max, const std::string& type) {
    if (index < 0 || index >= max) {
        std::string message = "Index de " + type + " invalide. Doit être entre 0 et " + std::to_string(max) + ".";
        view->afficherErreur(message);
        return false;
    }
    return true;
}

void GameController::gererErreur(std::string& erreur) {
    view->afficherErreur(erreur);
    view->attendreEntree();
}