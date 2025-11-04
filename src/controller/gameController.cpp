#include "gameController.h"
#include "joueur.h"
#include "marche.h"
#include "carte.h"
#include "saveManager.h"
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
    
    int choixChargement = view->demanderChoixInitialisationPartie();
    while (choixChargement != 1 && choixChargement != 2) {
        std::string msg = "Choix invalide. Veuillez réessayer.";
        view->afficherErreur(msg);
        choixChargement = view->demanderChoixInitialisationPartie();
    }
    switch (choixChargement) {
        case 1:
            initialiserPartie();
            break;
        case 2:
            chargerPartie();

            break;
    }

    try {
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

    //demander le mode de jeu
    ModeJeu modeJeu = ModeJeu::CLASSIQUE;
    //si on est deux joueurs on ne peux joueur que le mode classique
    if (nombreJoueurs > 2) {
        //sinon on demande le mode de jeu
        int choixMode = view->demanderChoixModeJeu();
        while (choixMode < 0 || choixMode > 2) {
            std::string msg = "Choix invalide. Veuillez réessayer.";
            view->afficherErreur(msg);
            choixMode = view->demanderChoixModeJeu();
        }
        modeJeu = static_cast<ModeJeu>(choixMode);
    }
    
    // Créer la partie
    try{
        partie = std::make_unique<Partie>();
        partie->commencer(nomsJoueurs, modeJeu);
        
        jeuEnCours = true;
        std::string message = "Partie initialisée avec " + std::to_string(nombreJoueurs) + " joueurs !";
        view->afficherMessage(message);
        view->attendreEntree();

        HRealms::Serialization::SaveManager::sauvegarderPartie(partie.get(), "autosave.json");
    } catch (const std::exception& e) {
        std::string msg = "Erreur lors de l'initialisation de la partie: " + std::string(e.what());
        view->afficherErreur(msg);
        initialiserPartie(); // Recommencer l'initialisation
    }
}

void GameController::chargerPartie() {
    //récupérer les cauvegardes possibles
    std::vector<std::string> sauvegardes = HRealms::Serialization::SaveManager::listerSauvegardes();
    if (sauvegardes.empty()) {
        std::string message = "Aucune sauvegarde trouvée. Nouvelle partie créée.";
        view->afficherErreur(message);
        initialiserPartie();
        return;
    }
    else {
        std::string message = "Sauvegardes disponibles:";
        view->afficherMessage(message);
        //on affiche toutes les sauvegardes disponibles
        for (std::string nom : sauvegardes) {
            view->afficherMessage(nom);
        }
    }

    std::string nom_fichier = view->demanderNomFichierSauvegarde();
    Partie* partieChargee = HRealms::Serialization::SaveManager::chargerPartie(nom_fichier);

    if (partieChargee == nullptr) {
        std::string message = "Erreur lors du chargement de la sauvegarde. Nouvelle partie créée.";
        view->afficherErreur(message);
        initialiserPartie();
        return;
    }
    
    // Transférer la partie chargée dans le unique_ptr
    partie.reset(partieChargee);
    jeuEnCours = true;
    
    std::string message = "Partie chargée avec succès !";
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
        
        //sauvegarde automatique avant chaque tour
        HRealms::Serialization::SaveManager::sauvegarderPartie(partie.get(), "autosave.json");

        // si il n'y à pas de tour en cours, on commence un nouveau tour
        if(partie->getTourCourant()==nullptr){
            partie->commencerTour();
        }

        // Afficher l'état du jeu
        afficherEtatJeu();

        // Gérer les actions du joueur
        bool tourTermine = false;
        while (!tourTermine && !partie->estTerminee()) {
            gererActionJoueur();
            
            //si le tour courant est nul c'est que le tour est terminé
            if (partie->getTourCourant()==nullptr) {
                tourTermine = true;
            }
            else {
                afficherEtatJeu();
            }
        }
        
    } catch (const std::exception& e) {
        std::string message = "Erreur pendant le tour: " + std::string(e.what());
        gererErreur(message);
    }
}

void GameController::gererActionJoueur() {
    int choix = view->demanderChoixAction(partie.get());
    
    switch (choix) {
        case -1:
            {
            std::string message = "Choix invalide. Veuillez réessayer.";
            view->afficherErreur(message);
            break;
            }
        case 1: // Jouer une carte
            jouerCarte();
            break;
        case 2 ://mobiliser un champion
            mobiliserChampion();
            break;
            
        case 3: // Acheter une carte
            acheterCarte();
            break;

        case 4: // Acheter une carte Gemme de feu
            acheterGemmeDeFeu();
            break;
        case 5: // Attaquer un joueur
            attaquerJoueur();

            for (int i = 0; i < (int)partie->getJoueurs().size(); ++i) {
                //si un joueur est mort on affiche quel joueur est éliminé
                if (partie->getJoueurs().at(i)->getPv() <= 0) {
                    std::string msg = "Le joueur " + partie->getJoueurs().at(i)->getNom() + " est éliminé.";
                    view->afficherMessage(msg);

                    //si on est en mode dernier en lice, on le retire de la partie,
                    // et on soigne de 10 et on fait piocher une carte au joueur de droite
                    if (partie->getModeJeu() == ModeJeu::DERNIER_EN_LICE) {
                        partie->getJoueurs().at((i+1) % partie->getJoueurs().size())->soigner(10);
                        partie->getJoueurs().at((i+1) % partie->getJoueurs().size())->piocher(1);
                        std::vector<Joueur*> j = partie->getJoueurs();
                        j.erase(j.begin() + i);
                        partie->setJoueurs(j);
                        if (i <= partie->getIdxJoueurActuel()) {
                            partie->setIdxJoueurActuel(partie->getIdxJoueurActuel() - 1);
                        }
                    }
                }
            }
            
            break;
        case 6 : // Utiliser un effetSacrifice
            utiliserEffetSacrifice();
            break;

        case 7: // Consulter sa défausse
            consulterDefausse();
            break;

        case 8: // Consulter pile de sacrifice
            consulterPileSacrifice();
            break;
        case 9: // Consulter les champions/défausse d'un adversaire
            afficherChampionsDefausseAdversaire();
            break;
        case 10: // Finir le tour
            finirTour();
            break;
        case 11: // Activer le godMode
            toggleGodMode();
            break;
        case 12: // Sauvegarder la partie
            {
            std::string nom_sauvegarde = view->demanderNomFichierSauvegarde();
            HRealms::Serialization::SaveManager::sauvegarderPartie(partie.get(), nom_sauvegarde);
            
                std::string message = "Partie sauvegardée avec succès.";
                view->afficherMessage(message);
            }
            break;
        case 13: // Quitter le jeu
        {
            partie->setTourCourant(nullptr);
            jeuEnCours = false;
            std::string message = "Vous avez quitté la partie.";
            view->afficherMessage(message);
        }
    }

}

bool GameController::jouerCarte() {
    Joueur* joueurActuel = partie->getJoueurActuel();
    
    // Demander quelle carte jouer
    std::vector<int> index = view->demanderIndexCarte();
    std::vector<Carte*> cartesToPlay;
    for (int idx : index) {
        if (validerIndex(idx,joueurActuel->getMain()->getTaille(), "carte")) {
            cartesToPlay.push_back(joueurActuel->getMain()->getCarte(idx));
        }
    }
    if (cartesToPlay.empty()) {
        return true;
    }
    for (Carte* c : cartesToPlay) {
        bool carteEnMain = false;
        //on cherche si la carte est toujours dans la main (au cas où plusieurs cartes sont jouées)
        for (auto carte : partie->getJoueurActuel()->getMain()->getCartes()) {
            if (carte == c) {
                carteEnMain = true;
                break;
            }
        }
        if (!carteEnMain) {
            continue;
        }

        std::vector<ChoixEffet*> cibles ;
        std::vector<ChoixEffet*> ciblesAllies;

        if (c->getEffetPrimaireActive()==false) {
            cibles = choisirCible(c,c->getEffetsPrimaire());
        }
        if (joueurActuel->possedeAllie(c) && c->getEffetAllieActive() == false) {
            ciblesAllies = choisirCible(c,c->getEffetsAllie());
        }
        // Essayer de jouer la carte
        try {
            bool succes = joueurActuel->jouerCarte(c, partie.get(), cibles, ciblesAllies);
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
        afficherEtatJeu();
    }
    return true;
}

bool GameController::mobiliserChampion() {
    Joueur* joueurActuel = partie->getJoueurActuel();
    
    // Demander quelle carte jouer
    afficherEtatJeu();
    std::vector<int> index = view->demanderIndexChampion(joueurActuel);

    for (int ind : index) {
        if (!validerIndex(ind,joueurActuel->getChampions().size(), "champion")) {
            return false;
        }
        Champion* c = joueurActuel->getChampions().at(ind);

        std::vector<ChoixEffet*> cibles = std::vector<ChoixEffet*>();
        std::vector<ChoixEffet*> ciblesAllies = std::vector<ChoixEffet*>();


        if (c->estMobilise()==false) {
            cibles = choisirCible(c,c->getEffetsActiver());
        }

        // Vérifier si les effets alliés n'ont pas encore été joués
        if (c->getEffetAllieActive() == false) {
            if (joueurActuel->possedeAllie(c)) {
                ciblesAllies = choisirCible(c,c->getEffetsAllie());
            }
        }
        // Essayer de jouer la carte
        try {
            bool succes = joueurActuel->mobiliserChampion(ind, partie.get(), cibles,ciblesAllies);
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
    std::vector<ChoixEffet*> targets= std::vector<ChoixEffet*>();
    for(size_t i=0;i<effets.size();i++){

        targets.push_back(demanderChoixEffet(effets.at(i),*partie,carte));
    }
    return targets;
}

ChoixEffet* GameController::demanderChoixEffet(Effet effet, Partie& partie, Carte* carteSource) {
    (void)carteSource;
    TypeEffet tE = effet.getTypeEffet();
    std::string msg = "Choix pour l'effet " + view->typeEffetToString(tE) +" de la carte " + carteSource->getNom();
    switch (tE) {
        case TypeEffet::AJOUT_OR:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::AJOUT_COMBAT:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::AJOUT_VIE:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::PIOCHER:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::DEFAUSSER:{{
            //on ne peut faire defausser une carte que chez son voisin de gauche
            view->afficherMessage(msg);

            Joueur* cible = nullptr;
            if (partie.getModeJeu() != ModeJeu::CLASSIQUE){
                int nbJoueurs = partie.getJoueurs().size();
                int indexJoueur = (partie.getIdxJoueurActuel()-1+nbJoueurs)%nbJoueurs;
                cible = partie.getJoueurs().at(indexJoueur);
            }
            else{
                //en mode classique le joueur choisit n'importe quel adversaire
                std::vector<Joueur*> joueursPossibles = {};
                for (size_t i = 0; i < partie.getJoueurs().size(); i++) {
                    if ((int)i != partie.getIdxJoueurActuel()) {
                        joueursPossibles.push_back(partie.getJoueurs().at(i));
                        view->afficherJoueur(partie.getJoueurs().at(i));
                        std::cout << std::endl;

                        if (!partie.getJoueurs().at(i)->getChampions().empty()) {
                            view->afficherChampions(partie.getJoueurs().at(i)->getChampions());
                        }
                    }
                    
                }
                int index = view->demanderIndexJoueur(joueursPossibles, -1);
                if (!validerIndex(index, joueursPossibles.size(), "joueur")) {
                    if (view->confirmerAnnulation(effet)) {
                        return new ChoixEffet(nullptr,0);
                    }
                    return demanderChoixEffet(effet,partie,carteSource);
                }
                cible = joueursPossibles.at(index);

            }
            //on fait choisir une carte dans la main de l'adversaire
            view->afficherMain(cible);
            int indexCarte = view->demanderIndexCarte().at(0);
            if (!validerIndex(indexCarte,cible->getMain()->getTaille(),"carte de la main")){
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            Carte* c = cible->getMain()->getCarte(indexCarte);
            return new ChoixEffet(c,0);
        }}

        case TypeEffet::SACRIFIER:{
            Joueur* j = partie.getJoueurActuel();
            std::vector<Carte*> cartesSacrifiables;
            std::vector<Carte*> championsEnJeu;
            std::vector<Carte*> cartesMain;
            std::vector<Carte*> cartesDefausse;
            // Champions en jeu
            for (Champion* champ : j->getChampions()) {
                if (champ) {
                    championsEnJeu.push_back(static_cast<Carte*>(champ));
                    cartesSacrifiables.push_back(static_cast<Carte*>(champ));
                }
            }
            // Cartes jouées
            for (int i = 0; i < j->getMain()->getTaille(); ++i) {
                Carte* c = j->getMain()->getCarte(i);
                if (c) {
                    cartesMain.push_back(c);
                    cartesSacrifiables.push_back(c);
                }
            }
            // Cartes de la défausse
            for (Carte* c : j->getDefausse()->getCartes()) {
                if (c) {
                    cartesDefausse.push_back(c);
                    cartesSacrifiables.push_back(c);
                }
            }
            int totalCartes = championsEnJeu.size() + cartesMain.size() + cartesDefausse.size();
            int indexCarte = view->demanderIndexCarteSacrifier(championsEnJeu, cartesMain, cartesDefausse);

            if (!validerIndex(indexCarte, totalCartes, "carte à sacrifier")) {
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            Carte* c = cartesSacrifiables.at(indexCarte);
            return new ChoixEffet(c,0);
        }

        case TypeEffet::ASSOMMER_CHAMPION:{
            view->afficherMessage(msg);
            std::vector<Joueur*> joueursAttaquables = {};

            Joueur* JoueurCible = nullptr;

            if (partie.getModeJeu() != ModeJeu::CLASSIQUE){
                int nbJoueurs = partie.getJoueurs().size();
                joueursAttaquables.push_back(partie.getJoueurs().at((partie.getIdxJoueurActuel()+1)%nbJoueurs));
                joueursAttaquables.push_back(partie.getJoueurs().at((partie.getIdxJoueurActuel()-1+nbJoueurs)%nbJoueurs));
            }
            else{
                //en mode classique le joueur choisit n'importe quel adversaire
                for (size_t i = 0; i < partie.getJoueurs().size(); i++) {
                    if ((int)i != partie.getIdxJoueurActuel()) {
                        joueursAttaquables.push_back(partie.getJoueurs().at(i));

                    }
                }
            }
            //affichage des joueurs attaquables
            for (Joueur* j : joueursAttaquables) {
                view->afficherJoueur(j);
                std::cout <<std::endl;
                if (!j->getChampions().empty()) {
                    view->afficherChampions(j->getChampions());
                }
            }

            //le joueur choisit parmis les joueurs attaquables
            int index = view->demanderIndexJoueur(joueursAttaquables, -1);
            if (!validerIndex(index, joueursAttaquables.size(), "joueur")) {
                if (view->confirmerAnnulation(effet)) {
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            JoueurCible = joueursAttaquables.at(index);

            if(JoueurCible==nullptr){
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            if (JoueurCible->getChampions().empty()){
                std::string msg = "Le joueur " + JoueurCible->getNom() + " ne possède pas de champion à assommer.";
                view->afficherMessage(msg);
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            //on affiche les champions du joueur cible
            view->afficherChampions(JoueurCible->getChampions());
            int indexCarte = view->demanderIndexCarte().at(0);
            if (!validerIndex(indexCarte,JoueurCible->getChampions().size(),"champion")){
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            Champion* c = JoueurCible->getChampions().at(indexCarte);
            return new ChoixEffet(c,0);
        }

        case TypeEffet::PREPARE_CHAMPION:{
            Joueur* j = partie.getJoueurActuel();
            if (j->getChampions().empty()){
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return nullptr;
            }
            view->afficherMessage(msg);
            view->afficherChampions(j->getChampions());
            if(j==nullptr){
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            int indexCarte = view->demanderIndexCarte().at(0);
            if (!validerIndex(indexCarte,j->getChampions().size(),"champion")){
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            Champion* c = j->getChampions().at(indexCarte);
            return new ChoixEffet(c,0);
        }

        case TypeEffet::COMBAT_OU_VIE:{
            view->afficherMessage(msg);
            bool choix = view->demanderChoix(effet);
            return new ChoixEffet(partie.getJoueurActuel(),choix);
        }

        case TypeEffet::OR_OU_COMBAT: {
            view->afficherMessage(msg);
            bool choix = view->demanderChoix(effet);
            return new ChoixEffet(partie.getJoueurActuel(),choix);
        }

        case TypeEffet::COMBAT_PAR_CHAMPION:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::COMBAT_PAR_GARDE:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::COMBAT_PAR_ALLIE:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::VIE_PAR_CHAMPION:
            return new ChoixEffet(partie.getJoueurActuel(),0);

        case TypeEffet::OR_OU_VIE_PAR_CHAMPION:{
            view->afficherMessage(msg);
            bool choix = view->demanderChoix(effet);
            return new ChoixEffet(partie.getJoueurActuel(),choix);
        }
        case TypeEffet::DEFAUSSE_AU_DESSUS_DU_DECK:{
            if (partie.getJoueurActuel()->getDefausse()->estVide()) {
                return new ChoixEffet(nullptr, 0);
            }
            view->afficherMessage(msg);
            view ->afficherDefausse(partie.getJoueurActuel());
            int indexCarte = view->demanderIndexCarte().at(0);
            if (!validerIndex(indexCarte,partie.getJoueurActuel()->getDefausse()->getTaille(),"carte de la défausse")){
                if (view->confirmerAnnulation(effet)){
                    return new ChoixEffet(nullptr,0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            Carte* c = partie.getJoueurActuel()->getDefausse()->getCartes().at(indexCarte);
            return new ChoixEffet(c,0);
        }

        case TypeEffet::CHAMPION_DEFAUSSE_AU_DESSUS_DECK:
        {
            std::vector<Champion*> defausseChampions = partie.getJoueurActuel()->getDefausseChampions();
            if (defausseChampions.empty()) {
                return new ChoixEffet(nullptr, 0);
            }
            view->afficherMessage(msg);
            view->afficherDefausseChampion(partie.getJoueurActuel());
            int indexCarte = view->demanderIndexCarte().at(0);
            if (!validerIndex(indexCarte, defausseChampions.size(), "champion de la défausse")) {
                if (view->confirmerAnnulation(effet)) {
                    return new ChoixEffet(nullptr, 0);
                }
                return demanderChoixEffet(effet,partie,carteSource);
            }
            Champion* c = defausseChampions.at(indexCarte);
            return new ChoixEffet(c, 0);
        }
        case TypeEffet::PROCHAIN_ACHAT_ACTION_DESSUS_DECK:
            return new ChoixEffet(partie.getJoueurActuel(), 0);

        case TypeEffet::PROCHAIN_ACHAT_DANS_MAIN:
            return new ChoixEffet(partie.getJoueurActuel(), 0);

        case TypeEffet::SACRIFIER_SIOUI_AJOUT_COMBAT:{
            view->afficherMessage(msg);
            bool choix = view->demanderChoix(effet);
            if (choix == true){
                Joueur* j = partie.getJoueurActuel();
                std::vector<Carte*> cartesSacrifiables;
                std::vector<Carte*> championsEnJeu;
                std::vector<Carte*> cartesMain;
                std::vector<Carte*> cartesDefausse;
                // Champions en jeu
                for (Champion* champ : j->getChampions()) {
                    if (champ) {
                        championsEnJeu.push_back(static_cast<Carte*>(champ));
                        cartesSacrifiables.push_back(static_cast<Carte*>(champ));
                    }
                }
                // Cartes jouées
                for (int i = 0; i < j->getMain()->getTaille(); ++i) {
                    Carte* c = j->getMain()->getCarte(i);
                    if (c) {
                        cartesMain.push_back(c);
                        cartesSacrifiables.push_back(c);
                    }
                }
                // Cartes de la défausse
                for (Carte* c : j->getDefausse()->getCartes()) {
                    if (c) {
                        cartesDefausse.push_back(c);
                        cartesSacrifiables.push_back(c);
                    }
                }
                int totalCartes = championsEnJeu.size() + cartesMain.size() + cartesDefausse.size();
                int indexCarte = view->demanderIndexCarteSacrifier(championsEnJeu, cartesMain, cartesDefausse);

                if (!validerIndex(indexCarte, totalCartes, "carte à sacrifier")) {
                    if (view->confirmerAnnulation(effet)){
                        return new ChoixEffet(nullptr,0);
                    }
                    return demanderChoixEffet(effet,partie,carteSource);
                }
                Carte* c = cartesSacrifiables.at(indexCarte);
                return new ChoixEffet(c,0);
            }
            if (view->confirmerAnnulation(effet)){
                return new ChoixEffet(nullptr,0);
            }
            return demanderChoixEffet(effet,partie,carteSource);
        }

        case TypeEffet::PIOCHE_SIOUI_DEFAUSSE:{
            std::vector<Carte*>* cartesADefausser = new std::vector<Carte*>();
            view->afficherMessage(msg);
            for(int i=0; i<effet.getMontant1(); i++){
                bool choix = view->demanderChoix(effet);
                if (choix == true){
                    int indexCarte = view->demanderIndexCarte().at(0);
                    if (!validerIndex(indexCarte,partie.getJoueurActuel()->getMain()->getTaille(),"carte de la main")){
                        if (view->confirmerAnnulation(effet)){
                            cartesADefausser->push_back(nullptr);
                        }
                        return demanderChoixEffet(effet,partie,carteSource);
                    }
                    Carte* c = partie.getJoueurActuel()->getMain()->getCartes().at(indexCarte);
                    cartesADefausser->push_back(c);

                }else {
                    if (view->confirmerAnnulation(effet)){
                        cartesADefausser->push_back(nullptr);
                        return new ChoixEffet(cartesADefausser,false);
                    }
                    return demanderChoixEffet(effet,partie,carteSource);
                }
            }
            return new ChoixEffet(cartesADefausser,false);
        }
    }
    return nullptr;
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
        bool succes = partie->getJoueurActuel()->acheterDepuisMarche(slot, partie.get());
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
    Joueur* joueurActuel = partie.get()->getJoueurActuel();

    bool succes = partie->getGemmeDeFeu()->acheter(joueurActuel, partie.get());

    if (succes) {
        std::string message = "Gemme de Feu achetée avec succès !";
        view->afficherMessage(message);
    }
    
    return succes;
}

bool GameController::attaquerJoueur() {
    // Afficher les adversaires et leurs champions
    std::vector<Joueur*> joueursAttaquable;
    Joueur* cible = nullptr;
    Joueur* gauche = nullptr;
    Joueur* droite = nullptr;
    int idxActuel = partie->getIdxJoueurActuel();
    int nbJoueurs = partie->getJoueurs().size();

    //en mode classique on peut attaquer tous les joueurs
    if (partie->getModeJeu() == ModeJeu::CLASSIQUE){
        //on ajuoute tous les joueurs sauf soi-même
        for (size_t i = 0; i < partie->getJoueurs().size(); i++) {
            if (i != static_cast<size_t>(idxActuel)) {
                joueursAttaquable.push_back(partie->getJoueurs().at(i));
            }
        }
    }
    else {
        // on peut attaquer les joueurs à gauche et à droite
        gauche = partie->getJoueurs().at((idxActuel - 1 + nbJoueurs) % nbJoueurs);
        joueursAttaquable.push_back(gauche);

        if (nbJoueurs > 2) {
            droite = partie->getJoueurs().at((idxActuel + 1) % nbJoueurs);
            joueursAttaquable.push_back(droite);
        }
        
        
    }
    // Affichage des champions de chaque joueur attaquable avant la sélection
    if (joueursAttaquable.size()>1){
        for (Joueur* j : joueursAttaquable) {
            std::string precision = "";
            // si on est pas en mode classique, on précise la position
            if (partie->getModeJeu() != ModeJeu::CLASSIQUE){
                if (j == gauche) precision += " (à gauche)";
                else if (j == droite) precision += " (à droite)";
            }
            view->afficherJoueur(j);
            view->afficherMessage(precision);
            if (!j->getChampions().empty()) {
                view->afficherChampions(j->getChampions());
            }
        }
        int index = view->demanderIndexJoueur(joueursAttaquable, -1);
        if (!validerIndex(index, joueursAttaquable.size(), "joueur")) {
            return false;
        }
        cible = joueursAttaquable.at(index);
    }
    else {
        cible = joueursAttaquable.at(0);
    }
    
    int degatsRestants = partie->getTourCourant()->getDegats();
    try {
        // Si on attaque le joueur à droite : on ne peut attaquer que ses champions si on est pas en mode classique
        if (partie->getModeJeu() != ModeJeu::CLASSIQUE && cible == droite) {
            while (degatsRestants > 0 && !cible->getChampions().empty()) {
                {
                    std::string msgDroite = "Vous ne pouvez attaquer que les champions du joueur à droite.";
                    view->afficherMessage(msgDroite);
                }
                view->afficherChampions(cible->getChampions());
                std::vector<int> indexChamp = view->demanderIndexChampion(cible);
                std::vector<Champion*> championAttaque;
                for (int ind : indexChamp) {
                    if (!validerIndex(ind, cible->getChampions().size(), "champion")) continue;
                    championAttaque.push_back(cible->getChampions().at(ind));
                }
                if (championAttaque.empty()) break;
                for (auto champ : championAttaque) {
                    if (degatsRestants < champ->getVie()) continue;
                    degatsRestants -= champ->getVie();
                    partie->getTourCourant()->utiliserDegats(champ->getVie());
                    std::cout << "⚔️ " << champ->getVie() << " dégâts → " << champ->getNom() << std::endl;
                    std::cout << "💀 " << champ->getNom() << " éliminé !" << std::endl;
                    cible->assomerChampion(champ);
                }
            }
            // On ne peut jamais mettre de dégâts au joueur à droite
            return true;
        }
        // Si on attaque le joueur à gauche : logique normale
        // 1 : Si l'adversaire a un champion garde, on doit l'attaquer en premier
        while (cible->aGarde() && degatsRestants > 0) {
            bool attaquePossible = false;
            for (auto* garde : cible->getGardes()) {
                if (degatsRestants >= garde->getVie()) {
                    attaquePossible = true;
                    break;
                }
            }
            if (!attaquePossible) {
                std::string err = "❌ Vous n'avez pas assez de dégâts pour éliminer un garde. Action impossible.";
                view->afficherErreur(err);
                return false;
            }
            view->afficherChampions(cible->getChampions());
            std::string message = cible->getNom() + " a au moins un champion garde. Lequel voulez-vous attaquer?";
            view->afficherMessage(message);
            std::vector<int> indexChampions = view->demanderIndexChampion(cible);
            std::vector<Champion*> championAttaqués;
            for (int ind : indexChampions) {
                if (!validerIndex(ind, cible->getChampions().size(), "champion") && cible->getChampions().at(ind)->estGarde() == false) continue;
                championAttaqués.push_back(cible->getChampions().at(ind));
            }
            for (auto champ : championAttaqués) {
                if (degatsRestants < champ->getVie()) {
                    std::string err = "❌ Pas assez de dégâts pour ce garde (" + std::to_string(champ->getVie()) + " PV). Il vous reste " + std::to_string(degatsRestants) + " dégâts.";
                    view->afficherErreur(err);
                    continue;
                }
                degatsRestants -= champ->getVie();
                partie->getTourCourant()->utiliserDegats(champ->getVie());
                std::cout << "⚔️ " << champ->getVie() << " dégâts → " << champ->getNom() << std::endl;
                std::cout << "💀 " << champ->getNom() << " éliminé !" << std::endl;
                cible->assomerChampion(champ);
            }
        }
        // 2 : Attaquer les autres champions ou le joueur (si pas de gardes)
        while (degatsRestants > 0 && !cible->getChampions().empty()) {
            std::string choix = "Attaquer un champion (c) ou le joueur (j) ?";
            view->afficherMessage(choix);
            char reponse;
            std::cout << "Choix (c/j): ";
            std::cin >> reponse;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (reponse == 'c' || reponse == 'C') {
                view->afficherChampions(cible->getChampions());
                std::vector<int> indexChamp = view->demanderIndexChampion(cible);
                std::vector<Champion*> championAttaque;
                for (int ind : indexChamp) {
                    if (!validerIndex(ind, cible->getChampions().size(), "champion")) continue;
                    championAttaque.push_back(cible->getChampions().at(ind));
                }
                if (championAttaque.empty()) break;
                for (auto champ : championAttaque) {
                    if (degatsRestants < champ->getVie()) continue;
                    degatsRestants -= champ->getVie();
                    partie->getTourCourant()->utiliserDegats(champ->getVie());
                    std::cout << "⚔️ " << champ->getVie() << " dégâts → " << champ->getNom() << std::endl;
                    std::cout << "💀 " << champ->getNom() << " éliminé !" << std::endl;
                    cible->assomerChampion(champ);
                }
            }
            if ((reponse == 'j' || reponse == 'J') && cible->aGarde() == false) {
                if (cible->getPv() < degatsRestants) {
                    partie->getTourCourant()->utiliserDegats(cible->getPv());
                    degatsRestants -= cible->getPv();
                    cible->prendreDegats(cible->getPv());
                }else {
                    cible->prendreDegats(degatsRestants);
                    partie->getTourCourant()->utiliserDegats(degatsRestants);
                    degatsRestants = 0;
                }
                break;
            }
        }
        // 3. Dégâts restants au joueur (à gauche)
        if (degatsRestants > 0) {
            bool succes = false;
            std::string message;
            if (cible->getPv() < degatsRestants) {
                    partie->getTourCourant()->utiliserDegats(cible->getPv());
                    degatsRestants -= cible->getPv();
                    succes = cible->prendreDegats(cible->getPv());
                    message = "💔 " + cible->getNom() + " perd " + std::to_string(cible->getPv()) + " PV !";
                }else {
                    succes = cible->prendreDegats(degatsRestants);
                    partie->getTourCourant()->utiliserDegats(degatsRestants);
                    message = "💔 " + cible->getNom() + " perd " + std::to_string(degatsRestants) + " PV !";
                    degatsRestants = 0;
                }
            
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
    Joueur* joueurActuel = partie->getJoueurActuel();

    if (joueurActuel->getDefausse()->getTaille() == 0) {
        std::string message = "Votre défausse est vide.";
        view->afficherMessage(message);
        return false;
    }
    
    view->afficherDefausse(joueurActuel);
    view->attendreEntree();
    return true;
}

bool GameController::consulterPileSacrifice() {

    if (partie->getCartesSacrifie()->getTaille() == 0) {
        std::string message = "Votre pile de sacrifice est vide.";
        view->afficherMessage(message);
        return false;
    }

    view->afficherPileSacrifice(partie.get());
    view->attendreEntree();
    return true;
}

bool GameController::utiliserEffetSacrifice() {
    Joueur* joueurActuel = partie->getJoueurActuel();
    std::vector<Carte*> cartesEffetSacrifice;
    // Constructions du vecteur des cartes avec effetSacrifice pour les cartes de la main
    for (int i = 0; i < joueurActuel->getMain()->getTaille(); ++i) {
        Carte* c = joueurActuel->getMain()->getCarte(i);
        if (!c->getEffetsSacrifice().empty()) {
            cartesEffetSacrifice.push_back(c);
        }
    }
    // idem mais pour les champions en jeu
    for (Champion* champ : joueurActuel->getChampions()) {
        if (champ && !champ->getEffetsSacrifice().empty()) {
            cartesEffetSacrifice.push_back(static_cast<Carte*>(champ));
        }
    }
        if (cartesEffetSacrifice.empty()) {
            std::string message = "Aucune carte avec effetSacrifice dans votre main ou parmi vos champions.";
            view->afficherErreur(message);
            return false;
        }
        //choix de la carte à sacrifier
        view->afficherCartesEffetSacrifice(cartesEffetSacrifice);
        int indexCarte = view->demanderIndexCarteEffetSacrifice();
        if (!validerIndex(indexCarte, cartesEffetSacrifice.size(), "carte à sacrifier")) {
            return false;
        }
        Carte* carteChoisie = cartesEffetSacrifice.at(indexCarte);

        //choix des cibles des effets de sacrifice
        std::vector<ChoixEffet*> cibles = choisirCible(carteChoisie,carteChoisie->getEffetsSacrifice());

        //activation des effets de sacrifice
        carteChoisie->sacrifier(cibles, partie.get());
        //enlèvement de la carte sacrifiée du jeu et ajout dans la pile de carte sacrifiée de la partie
        joueurActuel->sacrifier(carteChoisie, partie.get());


        std::string message = "Vous avez sacrifié la carte: " + carteChoisie->getNom();
        view->afficherMessage(message);

        //sacrifice réussi
        return true;
}

void GameController::finirTour() {
    Joueur* joueurActuel = partie->getJoueurActuel();
    if(joueurActuel->getNombreCartesJouables() > 0){
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
    
    // Sauvegarder automatiquement la partie après chaque tour
    HRealms::Serialization::SaveManager::sauvegarderPartie(partie.get(), "autosave.json");
}

void GameController::toggleGodMode() {
    if (!partie->getGodMode()->actif) {
        // GodMode inactif → demander mot de passe
        std::string motDePasse = view->demanderMotDePasseGodMode();
        
        if (motDePasse != "quarantedeux") {
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
                Deck* deckMarche = partie->getMarche()->getDeckMarche();
                if (deckMarche->getTaille() == 0) {
                    std::string message = "Le deck du marché est vide.";
                    view->afficherErreur(message);
                    break;
                }
                
                int idx = view->demanderIndexDeck(deckMarche);
                if (idx >= 0 && idx < deckMarche->getTaille()) {
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
    view->afficherPartie(partie.get());
    view->afficherScores(partie->getJoueurs());
    afficherMarche();
    afficherPileGemmeDeFeu();
    afficherMain();
    if(!partie->getJoueurActuel()->getChampions().empty()){
        afficherChampions();
    }
    
}

void GameController::afficherMain() {
    Joueur* joueurActuel = partie->getJoueurActuel();
    view->afficherMain(joueurActuel);
}

void GameController::afficherMarche() {
    Marche* marche = partie->getMarche();
    view->afficherMarche(marche);
}

void GameController::afficherPileGemmeDeFeu() {
    std::vector<Carte*> pileGemmes = partie->getGemmeDeFeu()->getPile()->getCartes();
    view->afficherPileGemmeDeFeu(pileGemmes);
}

void GameController::afficherChampions() {
    Joueur* joueurActuel = partie->getJoueurActuel();
    view->afficherChampions(joueurActuel->getChampions());
}

void GameController::afficherChampionsDefausseAdversaire() {
    std::vector<Joueur*> joueurs = partie->getJoueurs();
    
    // Filtrer pour ne montrer que les adversaires
    std::vector<Joueur*> adversaires;
    for (auto& j : joueurs) {
        if (j->getNom() != partie->getJoueurActuel()->getNom()) {
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
        for (auto* j : joueurs) {
            if (j->getNom() == adversaires.at(index)->getNom()) {
                view->afficherDefausse(j);

                if (!(j->getChampions().empty())) {
                    view->afficherChampions(j->getChampions());
                }
                
                view->attendreEntree();
                return;
            }
        }
    }
    // S'il n'y a qu'un seul adversaire
    else{
        Joueur* j = adversaires.at(0);
        view->afficherDefausse(j);
        if (!(j->getChampions().empty())) {
            view->afficherChampions(j->getChampions());
        }
        view->attendreEntree();
    }
}

Joueur* GameController::finirPartie() {
    if (partie && partie->estTerminee()) {
        Joueur* gagnant = partie->getGagnant();
        if(gagnant!=nullptr){
            view->afficherGagnant(gagnant);
            jeuEnCours = false;
            return gagnant;
        }
        else{
            jeuEnCours = false;
            return nullptr;
        }
    }
    jeuEnCours = false;
    return nullptr;
}

Joueur* GameController::getJoueurParIndex(int index){
    std::vector<Joueur*> joueurs = partie->getJoueurs();
    if (!validerIndex(index, joueurs.size(), "joueur")) {
        return nullptr;
    }
    return joueurs.at(index);
}

bool GameController::validerIndex(int index, int max, const std::string& type) {
    if (index < 0 || index >= max) {
        std::string message = "Index de " + type + " invalide. Doit être entre 1 et " + std::to_string(max) + ".";
        view->afficherErreur(message);
        return false;
    }
    return true;
}

void GameController::gererErreur(std::string& erreur) {
    view->afficherErreur(erreur);
    view->attendreEntree();
}