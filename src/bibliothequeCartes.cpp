#include 'bibliothequeCartes.h'

Deck BibliothequeCartes::creerDeckDepart() {
    Deck deck;

    // Cartes or/rubis
    for(int i = 0; i < 7; ++i){
        deck.ajouterDessous(new Carte("Or", 0, Faction::Neutre)).ajouterEffetPrimaire(new Effet(TypeEffet::AJOUT_OR, 1, TypeCible::SOI));
    }
    deck.ajouterDessous(new Carte("Rubis", 0, Faction::Neutre)).ajouterEffetPrimaire(new Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI));

    // Cartes épée courte/dague
    deck.ajouterDessous(new Carte("Épée courte", 0, Faction::Neutre)).ajouterEffetPrimaire(new Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI));
    deck.ajouterDessous(new Carte("Dague", 0, Faction::Neutre)).ajouterEffetPrimaire(new Effet(TypeEffet::AJOUT_COMBAT, 1, TypeCible::SOI));

    // Mélange et retour du deck
    deck.melanger();
    return deck;
}

Deck BibliothequeCartes::creerDeckMarche() {
    Deck deck;


}

Pile BibliothequeCartes::creerPileGemmeDeFeu() {
    Pile pile;
    for (int i = 0; i < 16; ++i) {
        pile.ajouterDessus(new Carte("Gemme de feu", 2, Faction::Neutre))
        .ajouterEffetPrimaire(new Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
        .ajouterEffetSacrifice(new Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI));
    }
    return pile;
}