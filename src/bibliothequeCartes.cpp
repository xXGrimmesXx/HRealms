#include 'bibliothequeCartes.h'

Deck BibliothequeCartes::creerDeckDepart() {
    Deck deck;
    // Ajouter les cartes de départ au deck
    deck.ajouterDessous(new Carte("Soldat", 1, Faction::Neutre));
    return deck;
}