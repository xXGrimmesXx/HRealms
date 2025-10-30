#pragma once
#include <nlohmann/json.hpp>
#include "../model/carte.h"
#include "../model/champion.h"
#include "../model/carteAction.h"
#include "../model/carteObjet.h"
#include "../model/joueur.h"
#include "../model/partie.h"
#include "../model/effet.h"
#include "../model/pile.h"
#include "../model/deck.h"
#include "../model/marche.h"
#include "../model/gemmeDeFeu.h"
#include "../model/tour.h"

using json = nlohmann::json;

namespace HRealms {
namespace Serialization {

// ========== EFFET ==========
json effetToJson(const Effet& effet);
Effet jsonToEffet(const json& j);

// ========== CARTE ==========
// Sérialiser une carte (polymorphique)
json carteToJson(const Carte* carte);

// Désérialiser une carte (factory pattern)
Carte* jsonToCarte(const json& j);

// ========== PILE ==========
json pileToJson(const Pile* pile);
Pile* jsonToPile(const json& j);

// ========== DECK ==========
json deckToJson(Deck* deck);
Deck* jsonToDeck(const json& j);

// ========== MAIN ===========
json mainToJson(Main* main);
Main* jsonToMain(const json& j);

// ========== JOUEUR ==========
json joueurToJson(const Joueur& joueur);
Joueur* jsonToJoueur(const json& j);

// ========== MARCHE ==========
json marcheToJson(const Marche* marche);
Marche* jsonToMarche(const json& j);

// ========== GEMME DE FEU ==========
json gemmeDeFeuToJson(const GemmeDeFeu* gemmeDeFeu);
GemmeDeFeu* jsonToGemmeDeFeu(const json& j);

// ========== TOUR ==========
json tourToJson(const Tour* tour);
Tour* jsonToTour(const json& j, const std::vector<Joueur*>& joueurs);

// ========== PARTIE ==========
json partieToJson(Partie* partie);
Partie* jsonToPartie(const json& j);

} // namespace Serialization
} // namespace HRealms
