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
        std::vector<ChoixEffet*> cibles = choisirCible(c,c->getEffetsPrimaire());
        std::vector<ChoixEffet*> ciblesAllies = choisirCible(c,c->getEffetsAllie());

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
        std::vector<ChoixEffet*> cibles = choisirCible(c,c->getEffetsActiver());
    
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

std::vector<ChoixEffet*> GameController::choisirCible(Carte* carte,std::vector<Effet> effets) {
    std::vector<ChoixEffet*> targets;
    for(size_t i=0;i<effets.size();i++){
            
        targets.push_back(demanderChoixEffet(effets[i],*partie,carte));
    }
    return targets;
}

ChoixEffet* GameController::demanderChoixEffet(Effet effet, Partie& partie, Carte* carteSource) {
    TypeEffet tE = effet.getTypeEffet();
    switch (tE) {
        
        case TypeEffet::AJOUT_OR:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::AJOUT_COMBAT:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::AJOUT_VIE:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::PIOCHER:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::DEFAUSSER:
            int indexJoueur = view->demanderIndexJoueur(partie.getJoueurs(),partie.getIdxJoueurActuel());
            Joueur* j = getJoueurParIndex(indexJoueur);
            if(j==nullptr){
                return new ChoixEffet(nullptr,0);
            }
            int indexCarte = view->demanderIndexCarte().at(0);
            Carte* c = j->getMain().getCarte(indexCarte);
            return new ChoixEffet(c,0);

        case TypeEffet::SACRIFIER:
            int indexJoueur = view->demanderIndexJoueur(partie.getJoueurs(),partie.getIdxJoueurActuel());
            Joueur* j = getJoueurParIndex(indexJoueur);
            if(j==nullptr){
                return new ChoixEffet(nullptr,0);
            }
            int indexCarte = view->demanderIndexCarte().at(0);
            Carte* c = j->getMain().getCarte(indexCarte);
            return new ChoixEffet(c,0);


        case TypeEffet::ASSOMMER_CHAMPION:
            int indexJoueur = view->demanderIndexJoueur(partie.getJoueurs(),partie.getIdxJoueurActuel());
            Joueur* j = getJoueurParIndex(indexJoueur);

            if(j==nullptr){
                return new ChoixEffet(nullptr,0);
            }
            int indexCarte = view->demanderIndexCarte().at(0);
            Champion* c = j->getChampions().at(indexCarte);
            return new ChoixEffet(c,0);

        case TypeEffet::PREPARE_CHAMPION:
            int indexJoueur = view->demanderIndexJoueur(partie.getJoueurs(),partie.getIdxJoueurActuel());
            Joueur* j = getJoueurParIndex(indexJoueur);

            if(j==nullptr){
                return new ChoixEffet(nullptr,0);
            }
            int indexCarte = view->demanderIndexCarte().at(0);
            Champion* c = j->getChampions().at(indexCarte);
            return new ChoixEffet(c,0);

        case TypeEffet::COMBAT_OU_VIE:
            bool choix = view->demanderChoix(effet);
            return new ChoixEffet(&partie.getJoueurActuel(),choix);

        case TypeEffet::OR_OU_COMBAT:
            bool choix = view->demanderChoix(effet);
            return new ChoixEffet(&partie.getJoueurActuel(),choix);

        case TypeEffet::COMBAT_PAR_CHAMPION:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::COMBAT_PAR_GARDE:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::COMBAT_PAR_ALLIE:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::VIE_PAR_CHAMPION:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::OR_OU_VIE_PAR_CHAMPION:
            bool choix = view->demanderChoix(effet);
            return new ChoixEffet(&partie.getJoueurActuel(),choix);

        case TypeEffet::DEFAUSSE_AU_DESSUS_DU_DECK:
            view ->afficherDefausse(partie.getJoueurActuel());
            int indexCarte = view->demanderIndexCarte().at(0);
            if (!validerIndex(indexCarte,partie.getJoueurActuel().getDefausse().getTaille(),"carte de la défausse")){
                return new ChoixEffet(nullptr,0);
            }
            Carte* c = partie.getJoueurActuel().getDefausse().getCartes().at(indexCarte);
            return new ChoixEffet(c,0);

        case TypeEffet::CHAMPION_DEFAUSSE_AU_DESSUS_DECK:
        {
            std::vector<Champion*> defausseChampions = partie.getJoueurActuel().getDefausseChampions();
            view ->afficherDefausseChampion(partie.getJoueurActuel());
            int indexCarte = view->demanderIndexCarte().at(0);
            if (!validerIndex(indexCarte,partie.getJoueurActuel().getDefausseChampions().size(),"champion de la défausse")){
                return new ChoixEffet(nullptr,0);
            }
            Champion* c = partie.getJoueurActuel().getDefausseChampions().at(indexCarte);
        }
            return new ChoixEffet(c,0);

        case TypeEffet::PROCHAIN_ACHAT_ACTION_DESSUS_DECK:
            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::PROCHAIN_ACHAT_DANS_MAIN:

            return new ChoixEffet(&partie.getJoueurActuel(),0);

        case TypeEffet::SACRIFIER_SIOUI_AJOUT_COMBAT:
            bool choix = view->demanderChoix(effet);
            return new ChoixEffet(&partie.getJoueurActuel(),choix);
        case TypeEffet::PIOCHE_SIOUI_DEFAUSSE:
            bool choix = view->demanderChoix(effet);
            if (choix == true){
                int indexCarte = view->demanderIndexCarte().at(0);
                if (!validerIndex(indexCarte,partie.getJoueurActuel().getDefausse().getTaille(),"carte de la défausse")){
                    return new ChoixEffet(nullptr,0);
                }
                Carte* c = partie.getJoueurActuel().getDefausse().getCartes().at(indexCarte);
                return new ChoixEffet(c,choix);
            }
            return new ChoixEffet(nullptr,choix);
    }
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
    int idxActuel = partie->getIdxJoueurActuel();

    // Filtrer les adversaires
    std::vector<int> indicesAdversaires;
    for (size_t i = 0; i < joueurs.size(); ++i) {
        if ((int)i != idxActuel) {
            indicesAdversaires.push_back((int)i);
        }
    }
    
    // Afficher la liste détaillée
    std::cout << "\n═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "Adversaires disponibles :" << std::endl;
    for (int i : indicesAdversaires) {
        Joueur& adv = joueurs[i];
        std::cout << "[" << i << "] " << adv.getNom() << " (" << adv.getPv() << " ❤️)" << std::endl;

        // Afficher les champions de l'adversaire
        if (!adv.getChampions().empty()) {
            view->afficherChampions(adv.getChampions());
        }
        std::cout << "-------------------------------------------------------------" << std::endl;
    }
    std::cout << "═══════════════════════════════════════════════════════════════\n" << std::endl;

    int index = -1;
    // Demander quel joueur attaquer
    if (indicesAdversaires.size() > 1) {
        index = view->demanderIndexJoueur(joueurs,partie->getIdxJoueurActuel());
        if (!validerIndex(index, static_cast<int>(joueurs.size()), "joueur")) {
            return false;
        }
    }
    // Sauf s'il y en a un seul
    else{
        index = indicesAdversaires[0];
    }
    

    Joueur& cible = partie->getJoueurs().at(index);
    int degatsRestants = partie->getTourCourant()->getDegats();

    // Afficher état cible
    std::cout << "\n═══════════════════════════════════════════════════════════════" << std::endl;
    std::cout << "🎯 Cible : " << cible.getNom() << " (" << cible.getPv() << " ❤️)" << std::endl;
    std::cout << "⚔️  Vos dégâts disponibles : " << degatsRestants << std::endl;
    std::cout << "═══════════════════════════════════════════════════════════════\n" << std::endl;

    // Si le joueur n'a pas assez de dégâts pour tuer au moins un garde, on bloque l'action
    if (cible.aGarde()) {
        bool possible = false;
        for (auto* garde : cible.getGardes()) {
            if (degatsRestants >= garde->getVie()) {
                possible = true;
                break;
            }
        }
        if (!possible) {
            std::string err = "❌ Vous n'avez pas assez de dégâts pour éliminer un garde. Action impossible.";
            view->afficherErreur(err);
            return false;
        }
    }

    try {
        
        // 1 : Si l'adversaire a un champion garde, on doit l'attaquer en premier
        while (cible.aGarde() && degatsRestants > 0){
            std::string message = joueurs.at(index).getNom() + " a au moins un champion garde. Lequel voulez-vous attaquer?";
            view->afficherMessage(message);

            std::vector<int> indexGarde = view->demanderIndexGarde(cible);
            
            if (indexGarde.empty() || indexGarde[0] < 0) {
                std::string err = "Vous devez choisir un garde.";
                view->afficherErreur(err);
                continue;
            }

            int ind = indexGarde[0];

            if (!validerIndex(ind, cible.getGardes().size(), "garde")) {
                continue;
            }

            Champion* garde = cible.getGardes().at(ind);

            if (degatsRestants < garde->getVie()) {
                std::string err = "❌ Pas assez de dégâts pour ce garde (" 
                                + std::to_string(garde->getVie()) + " PV). Il vous reste " 
                                + std::to_string(degatsRestants) + " dégâts.";
                view->afficherErreur(err);
                continue;
            }
            
            degatsRestants -= garde->getVie();
            partie->getTourCourant()->utiliserDegats(garde->getVie());
            
            std::cout << "⚔️ " << garde->getVie() << " dégâts → " << garde->getNom() << std::endl;
            std::cout << "💀 " << garde->getNom() << " éliminé !" << std::endl;
            
            cible.assomerChampion(garde);

            // Réafficher les gardes restants
            if (cible.aGarde()) {
                std::cout << "\n🛡️ Gardes restants :" << std::endl;
                view->afficherChampions(cible.getGardes());
                std::cout << std::endl;
            }
        }

        // 2 : Attaquer les autres champions si le joueur le souhaite
        if (degatsRestants > 0 && !cible.getChampions().empty()) {
            std::string choix = "Attaquer un champion (c) ou le joueur (j) ?";
            view->afficherMessage(choix);
            
            char reponse;
            std::cout << "Choix (c/j): ";
            std::cin >> reponse;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (reponse == 'c' || reponse == 'C') {
                while (degatsRestants > 0 && !cible.getChampions().empty()) {
                    
                    std::vector<int> indexChamp = view->demanderIndexChampion(cible);
                    
                    if (indexChamp.empty() || indexChamp[0] < 0) {
                        break;
                    }
                    
                    int ind = indexChamp[0];
                    
                    if (!validerIndex(ind, cible.getChampions().size(), "champion")) {
                        break;
                    }
                    
                    Champion* champ = cible.getChampions().at(ind);
                    
                    if (degatsRestants < champ->getVie()) {
                        std::string err = "❌ Pas assez de dégâts pour ce champion (" 
                                        + std::to_string(champ->getVie()) + " PV).";
                        view->afficherErreur(err);
                        
                        std::cout << "Choisir un autre champion ? (o/n): ";
                        char rep;
                        std::cin >> rep;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        
                        if (rep != 'o' && rep != 'O') break;
                        continue;
                    }
                    
                    // ONE-SHOT le champion
                    degatsRestants -= champ->getVie();
                    partie->getTourCourant()->utiliserDegats(champ->getVie());
                    
                    std::cout << "⚔️ " << champ->getVie() << " dégâts → " << champ->getNom() << std::endl;
                    std::cout << "💀 " << champ->getNom() << " éliminé !" << std::endl;
                    
                    cible.assomerChampion(champ);
                    
                    // Demander si continuer
                    if (degatsRestants > 0 && !cible.getChampions().empty()) {
                        std::cout << "Reste " << degatsRestants << " dégâts. Attaquer un autre champion ? (o/n): ";
                        char rep;
                        std::cin >> rep;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        
                        if (rep != 'o' && rep != 'O') break;
                    }
                }
            }
        }
        // 3. Dégâts restants au joueur
        if (degatsRestants > 0) {
            bool succes = cible.prendreDegats(degatsRestants);
            partie->getTourCourant()->utiliserDegats(degatsRestants);
            
            std::string message = "💔 " + cible.getNom() + " perd " 
                                + std::to_string(degatsRestants) + " PV !";
            view->afficherMessage(message);
            return succes;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::string message = "Erreur lors de l'attaque: " + std::string(e.what());
        view->afficherErreur(message);
        return false;
    }
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

                if (!(j.getChampions().empty())) {
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
        if (!(j.getChampions().empty())) {
                    view->afficherChampions(j.getChampions());
        }
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

Joueur* GameController::getJoueurParIndex(int index){
    std::vector<Joueur>& joueurs = partie->getJoueurs();
    if (!validerIndex(index, joueurs.size(), "joueur")) {
        return nullptr;
    }
    return &joueurs.at(index);
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