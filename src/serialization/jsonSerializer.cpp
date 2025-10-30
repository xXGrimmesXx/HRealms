#include "jsonSerializer.h"
#include "../model/bibliothequeCartes.h"
#include <stdexcept>
#include <iostream>

namespace HRealms {
namespace Serialization {

// ========== EFFET ==========
json effetToJson(const Effet& effet) {
    return json{
        {"typeEffet", static_cast<int>(effet.getTypeEffet())},
        {"montant1", effet.getMontant1()},
        {"montant2", effet.getMontant2()},
        {"typeCible", static_cast<int>(effet.getTypeCible())}
    };
}

Effet jsonToEffet(const json& j) {
    TypeEffet type = static_cast<TypeEffet>(j["typeEffet"].get<int>());
    int montant1 = j["montant1"];
    int montant2 = j.value("montant2", 0);
    TypeCible cible = static_cast<TypeCible>(j["typeCible"].get<int>());

    if (montant2 != -1) {
        return Effet(type, montant1, montant2, cible);
    }
    return Effet(type, montant1, cible);
}

// ========== CARTE ==========
json carteToJson(const Carte* carte) {
    if (!carte) {
        return nullptr;
    }
    
    json j;
    j["id"] = carte->getId();
    j["nom"] = carte->getNom();
    j["cout"] = carte->getCout();
    j["faction"] = static_cast<int>(carte->getFaction());
    j["est_jouee"] = carte->getEstJouee();
    
    // Sérialiser les effets primaires
    json effetsPrimaires = json::array();
    for (const auto& effet : carte->getEffetsPrimaire()) {
        effetsPrimaires.push_back(effetToJson(effet));
    }
    j["effets_primaires"] = effetsPrimaires;
    
    // Sérialiser les effets alliés
    json effetsAllie = json::array();
    for (const auto& effet : carte->getEffetsAllie()) {
        effetsAllie.push_back(effetToJson(effet));
    }
    j["effets_allie"] = effetsAllie;
    
    // Type discrimination pour polymorphisme
    if (const Champion* champ = dynamic_cast<const Champion*>(carte)) {
        j["type"] = "champion";
        j["garde"] = champ->estGarde();
        j["vie"] = champ->getVie();
        j["mobilise"] = champ->estMobilise();
        
        // Effets d'activation
        json effetsActiver = json::array();
        for (const auto& effet : champ->getEffetsActiver()) {
            effetsActiver.push_back(effetToJson(effet));
        }
        j["effets_activer"] = effetsActiver;
        
        // Effet de sacrifice
        json effetsSacrifice = json::array();
        for (const auto& effet : champ->getEffetsSacrifice()) {
            effetsSacrifice.push_back(effetToJson(effet));
        }
        j["effets_sacrifice"] = effetsSacrifice;
    } 
    else if (dynamic_cast<const CarteAction*>(carte)) {
        j["type"] = "action";
        
        // Effet de sacrifice pour les actions
        json effetsSacrifice = json::array();
        for (const auto& effet : carte->getEffetsSacrifice()) {
            effetsSacrifice.push_back(effetToJson(effet));
        }
        j["effets_sacrifice"] = effetsSacrifice;
    } 
    else if (dynamic_cast<const CarteObjet*>(carte)) {
        j["type"] = "objet";
        
        // Effet de sacrifice pour les objets
        json effetsSacrifice = json::array();
        for (const auto& effet : carte->getEffetsSacrifice()) {
            effetsSacrifice.push_back(effetToJson(effet));
        }
        j["effets_sacrifice"] = effetsSacrifice;
    }
    
    return j;
}

Carte* jsonToCarte(const json& j) {
    if (j.is_null()) {
        return nullptr;
    }
    
    std::string type = j["type"];
    std::string nom = j["nom"];
    int cout = j["cout"];
    Faction faction = static_cast<Faction>(j["faction"].get<int>());
    
    Carte* carte = nullptr;
    
    // Créer la carte selon son type
    if (type == "champion") {
        bool garde = j["garde"];
        int vie = j["vie"];
        carte = new Champion(nom, cout, faction, garde, vie);
        
        Champion* champ = static_cast<Champion*>(carte);
        
        // Restaurer l'état mobilisé
        if (j.contains("mobilise") && j["mobilise"]) {
            champ->setMobilise(true);
        }
        
        // Ajouter les effets d'activation
        if (j.contains("effets_activer")) {
            for (const auto& effetJson : j["effets_activer"]) {
                champ->ajouterEffetActiver(jsonToEffet(effetJson));
            }
        }
        
        // Ajouter les effets de sacrifice pour champions
        if (j.contains("effets_sacrifice")) {
            for (const auto& effetJson : j["effets_sacrifice"]) {
                champ->ajouterEffetSacrifice(jsonToEffet(effetJson));
            }
        }
    } 
    else if (type == "action") {
        carte = new CarteAction(nom, cout, faction);
        
        // Ajouter les effets de sacrifice pour actions
        if (j.contains("effets_sacrifice")) {
            for (const auto& effetJson : j["effets_sacrifice"]) {
                carte->ajouterEffetSacrifice(jsonToEffet(effetJson));
            }
        }
    } 
    else if (type == "objet") {
        carte = new CarteObjet(nom, cout, faction);
        
        // Ajouter les effets de sacrifice pour objets
        if (j.contains("effets_sacrifice")) {
            for (const auto& effetJson : j["effets_sacrifice"]) {
                carte->ajouterEffetSacrifice(jsonToEffet(effetJson));
            }
        }
    }
    
    if (!carte) {
        throw std::runtime_error("Type de carte inconnu: " + type);
    }
    
    // Restaurer l'ID
    if (j.contains("id")) {
        carte->setId(j["id"]);
    }
    
    // Ajouter les effets primaires
    if (j.contains("effets_primaires")) {
        for (const auto& effetJson : j["effets_primaires"]) {
            carte->ajouterEffetPrimaire(jsonToEffet(effetJson));
        }
    }
    
    // Ajouter les effets alliés
    if (j.contains("effets_allie")) {
        for (const auto& effetJson : j["effets_allie"]) {
            carte->ajouterEffetAllie(jsonToEffet(effetJson));
        }
    }
    
    // Restaurer l'état "jouée"
    if (j.contains("est_jouee") && j["est_jouee"]) {
        carte->setEstJouee(true);
    }
    
    return carte;
}

// ========== PILE ==========
json pileToJson(const Pile* pile) {
    json j = json::array();
    
    for (int i = 0; i < pile->getTaille(); i++) {
        Carte* carte = pile->getCartes().at(i);
        j.push_back(carteToJson(carte));
    }
    
    return j;
}

Pile* jsonToPile(const json& j) {
    Pile* pile = new Pile();
    
    // Remplir avec les nouvelles cartes
    for (const auto& carteJson : j) {
        Carte* carte = jsonToCarte(carteJson);
        if (carte) {
            pile->ajouter(carte);
        }
    }
    
    return pile;
}

// ========== DECK ==========
json deckToJson(Deck* deck) {
    json j = json::array();
    
    for (int i = 0; i < deck->getTaille(); i++) {
        Carte* carte = deck->getCartes().at(i);
        j.push_back(carteToJson(carte));
    }
    
    return j;
}

Deck* jsonToDeck(const json& j) {
    Deck* deck = new Deck();
    
    // Remplir avec les nouvelles cartes
    for (const auto& carteJson : j) {
        Carte* carte = jsonToCarte(carteJson);
        if (carte) {
            deck->ajouterDessus(carte);
        }
    }
    
    return deck;
}

json mainToJson (Main * main){
    json j = json::array();
    for (auto c : main->getCartes()){
        j.push_back(carteToJson(c));
    }
    return j;
}

Main* jsonToMain(const json& j){
    Main* main = new Main();
    
    // Remplir avec les nouvelles cartes
    for (const auto& carteJson : j) {
        Carte* carte = jsonToCarte(carteJson);
        if (carte) {
            main->ajouter(carte);
        }
    }
    
    return main;
}

// ========== JOUEUR ==========
json joueurToJson(Joueur& joueur) {
    json j;
    
    j["nom"] = joueur.getNom();
    j["pv"] = joueur.getPv();
    j["prochain_achat_action_dessus_deck"] = joueur.getProchainAchatActionDessusDeck();
    j["prochain_achat_dans_main"] = joueur.getProchainAchatDansMain();
    
    // Sérialiser les piles
    j["main"] = mainToJson(joueur.getMain());
    j["deck"] = deckToJson(joueur.getDeck());
    j["defausse"] = pileToJson(joueur.getDefausse());
    
    // Champions en jeu
    json champions = json::array();
    for (size_t i = 0; i < joueur.getChampions().size(); i++) {
        champions.push_back(carteToJson(joueur.getChampions()[i]));
    }
    j["champions"] = champions;
    
    // Gardes
    json gardes = json::array();
    for (size_t i = 0; i < joueur.getGardes().size(); i++) {
        gardes.push_back(carteToJson(joueur.getGardes()[i]));
    }
    j["gardes"] = gardes;
    
    return j;
}

Joueur* jsonToJoueur(const json& j) {
    std::string nom = j["nom"];
    Joueur* joueur = new Joueur(nom);
    
    // Restaurer les PV
    int pv = j["pv"];
    joueur->setPv(pv);
    
    // Restaurer les flags d'achat
    if (j.contains("prochain_achat_action_dessus_deck")) {
        joueur->setProchainAchatActionDessusDeck(j["prochain_achat_action_dessus_deck"]);
    }
    if (j.contains("prochain_achat_dans_main")) {
        joueur->setProchainAchatDansMain(j["prochain_achat_dans_main"]);
    }
    
    // Restaurer les decks
    if (j.contains("main")) {
        Main* main = jsonToMain(j["main"]);
        joueur->setMain(main);
        delete main;
    }
    if (j.contains("deck")) {
        Deck* deck = jsonToDeck(j["deck"]);
        joueur->setDeck(deck);
        delete deck;
    }
    if (j.contains("defausse")) {
        Pile* defausse = jsonToPile(j["defausse"]);
        joueur->setDefausse(defausse);
        delete defausse;
    }
    
    // Restaurer les champions
    if (j.contains("champions")) {
        for (const auto& champJson : j["champions"]) {
            Carte* carte = jsonToCarte(champJson);
            if (Champion* champ = dynamic_cast<Champion*>(carte)) {
                joueur->ajouterChampion(champ);
            }
        }
    }
    
    return joueur;
}

// ========== MARCHE ==========
json marcheToJson(Marche* marche) {
    json j;
    
    // Sérialiser le deck du marché
    j["deck_marche"] = deckToJson(marche->getDeckMarche());
    
    // Sérialiser la ligne du marché
    json ligne = json::array();
    for (const auto& carte : marche->getLigne()) {
        ligne.push_back(carteToJson(carte));
    }
    j["ligne"] = ligne;
    
    return j;
}

Marche* jsonToMarche(const json& j) {
    Marche* marche = new Marche();
    
    // Restaurer le deck du marché
    if (j.contains("deck_marche")) {
        Deck* deckMarche = jsonToDeck(j["deck_marche"]);
        marche->setDeckMarche(deckMarche);
        // Vider le conteneur temporaire avant de le supprimer (cartes transférées)
        while (deckMarche->getTaille() > 0) {
            deckMarche->piocher();
        }
        delete deckMarche;
    }
    
    // Restaurer la ligne du marché
    if (j.contains("ligne")) {
        std::vector<Carte*> ligne;
        for (const auto& carteJson : j["ligne"]) {
            Carte* carte = jsonToCarte(carteJson);
            if (carte) {
                ligne.push_back(carte);
            }
        }
        marche->setLigne(ligne);
    }
    
    return marche;
}

// ========== GEMME DE FEU ==========
json gemmeDeFeuToJson(const GemmeDeFeu* gemmeDeFeu) {
    json j;
    
    // Sérialiser la pile de gemmes de feu
    j["pile"] = pileToJson(gemmeDeFeu->getPile());
    
    return j;
}

GemmeDeFeu* jsonToGemmeDeFeu(const json& j) {
    GemmeDeFeu* gemmeDeFeu = new GemmeDeFeu();
    
    // Restaurer la pile de gemmes de feu
    if (j.contains("pile")) {
        Pile* pile = jsonToPile(j["pile"]);
        gemmeDeFeu->setPile(pile);
        // Vider le conteneur temporaire avant de le supprimer (cartes transférées)
        while (pile->getTaille() > 0) {
            pile->prendreDessus();
        }
        delete pile;
    }
    
    return gemmeDeFeu;
}

// ========== TOUR ==========
json tourToJson(const Tour* tour) {
    if (!tour) {
        return nullptr;
    }
    
    json j;
    
    // Sauvegarder l'index du joueur (on retrouvera le joueur dans la liste des joueurs de la partie)
    j["joueur_nom"] = tour->getJoueur()->getNom();
    j["reserve_or"] = tour->getOr();
    j["reserve_degats"] = tour->getDegats();
    
    return j;
}

Tour* jsonToTour(const json& j, const std::vector<Joueur*>& joueurs) {
    if (j.is_null()) {
        return nullptr;
    }
    
    // Retrouver le joueur par son nom
    std::string joueurNom = j["joueur_nom"];
    Joueur* joueur = nullptr;
    for (auto j_ptr : joueurs) {
        if (j_ptr->getNom() == joueurNom) {
            joueur = j_ptr;
            break;
        }
    }
    
    if (!joueur) {
        return nullptr;
    }
    
    // Créer le tour
    Tour* tour = new Tour(joueur);
    
    // Restaurer les réserves
    if (j.contains("reserve_or")) {
        tour->setOr(j["reserve_or"]);
    }
    if (j.contains("reserve_degats")) {
        tour->setDegats(j["reserve_degats"]);
    }
    
    return tour;
}

// ========== PARTIE ==========
json partieToJson(Partie* partie) {
    json j;
    
    j["nb_tour"] = partie->getNbTour();
    j["idx_joueur_actuel"] = partie->getIdxJoueurActuel();
    
    // Joueurs
    json joueurs = json::array();
    for (const auto& joueur : partie->getJoueurs()) {
        joueurs.push_back(joueurToJson(*joueur));
    }
    j["joueurs"] = joueurs;
    
    // Pile de sacrifice
    j["cartes_sacrifie"] = pileToJson(partie->getCartesSacrifie());
    
    // Gemme de feu (complète avec toute la pile)
    j["gemme_de_feu"] = gemmeDeFeuToJson(partie->getGemmeDeFeu());

    // GodMode
    j["god_mode_actif"] = partie->getGodMode()->actif;

    // Marché (deck et ligne)
    j["marche"] = marcheToJson(partie->getMarche());
    
    // Tour courant
    j["tour_courant"] = tourToJson(partie->getTourCourant());
    
    return j;
}

Partie* jsonToPartie(const json& j) {
    Partie* partie = new Partie();
    
    // Restaurer les joueurs
    if (j.contains("joueurs")) {
        for (const auto& joueurJson : j["joueurs"]) {
            Joueur* joueur = jsonToJoueur(joueurJson);
            partie->ajouterJoueur(joueur);
        }
    }
    
    // Restaurer l'état de la partie
    if (j.contains("nb_tour")) {
        partie->setNbTour(j["nb_tour"]);
    }
    
    if (j.contains("idx_joueur_actuel")) {
        partie->setIdxJoueurActuel(j["idx_joueur_actuel"]);
    }
    
    // Restaurer les cartes sacrifiées
    if (j.contains("cartes_sacrifie")) {
        Pile* cartesSacrifie = jsonToPile(j["cartes_sacrifie"]);
        partie->setCartesSacrifie(cartesSacrifie);
        delete cartesSacrifie;
    }
    
    // Restaurer les gemmes de feu (complète avec toute la pile)
    if (j.contains("gemme_de_feu")) {
        GemmeDeFeu* gemmeDeFeu = jsonToGemmeDeFeu(j["gemme_de_feu"]);
        partie->setGemmeDeFeu(gemmeDeFeu);
        // Note: On ne supprime pas gemmeDeFeu car il a été copié par valeur dans setGemmeDeFeu
        delete gemmeDeFeu;
    }
    
    // Restaurer GodMode
    if (j.contains("god_mode_actif")) {
        partie->setGodMode(j["god_mode_actif"]);
    }
    
    // Restaurer le marché (deck et ligne)
    if (j.contains("marche")) {
        Marche* marche = jsonToMarche(j["marche"]);
        partie->setMarche(marche);
        // Note: On ne supprime pas marche car la propriété a été transférée à partie
    }
    
    // Restaurer le tour courant
    if (j.contains("tour_courant") && !j["tour_courant"].is_null()) {
        Tour* tour = jsonToTour(j["tour_courant"], partie->getJoueurs());
        partie->setTourCourant(tour);
    }
    
    return partie;
}

} // namespace Serialization
} // namespace HRealms
