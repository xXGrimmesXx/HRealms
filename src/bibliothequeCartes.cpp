#include "bibliothequeCartes.h"

Deck BibliothequeCartes::creerDeckDepart() {
    Deck deck;

    // Cartes or/rubis
    for(int i = 0; i < 7; ++i){
        deck.ajouterDessous(CarteObjet("Or", 0, Faction::AUCUNE).ajouterEffetPrimaire( Effet(TypeEffet::AJOUT_OR, 1, 0, TypeCible::SOI)));
    }
    deck.ajouterDessous(CarteObjet("Rubis", 0, Faction::AUCUNE).ajouterEffetPrimaire( Effet(TypeEffet::AJOUT_OR, 2, 0, TypeCible::SOI)));

    // Cartes épée courte/dague
    deck.ajouterDessous(CarteObjet("Épée courte", 0, Faction::AUCUNE).ajouterEffetPrimaire( Effet(TypeEffet::AJOUT_COMBAT, 2, 0, TypeCible::SOI)));
    deck.ajouterDessous(CarteObjet("Dague", 0, Faction::AUCUNE).ajouterEffetPrimaire( Effet(TypeEffet::AJOUT_COMBAT, 1, 0, TypeCible::SOI)));

    // Mélange et retour du deck
    deck.melanger();
    return deck;
}

Deck BibliothequeCartes::creerDeckMarche() {
    Deck deck;
    
    // === FACTION IMPERIALE ===
    deck.ajouterDessous(
        (new Champion("Arkus, Imperial Dragon", 8, Faction::IMPERIALE, true, 6))
            ->ajouterEffetActiver( Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
            ->ajouterEffetActiver( Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
            ->ajouterEffetAllie( Effet(TypeEffet::AJOUT_VIE, 6, TypeCible::SOI))
    );

    deck.ajouterDessous(
       (new  CarteAction("Close Ranks", 3, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
            // +2 combat per champion in play -> TODO: effet complexe
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_VIE, 6, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Command", 5, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_VIE, 4, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
       (new Champion("Darian, War Mage", 4, Faction::IMPERIALE, 5, false))
            // may gain 3 combat or 4 health -> TODO choice
            ->ajouterEffetActiver(Effet(TypeEffet::COMBAT_OU_VIE, /*3 ou 4*/, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Domination", 7, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 6, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_VIE, 6, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
            // Ally: Prepare a champion -> TODO
    );

    deck.ajouterDessous(
        (new Champion("Cristov, the Just", 5, Faction::IMPERIALE, 5, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_VIE, 3, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Kraka, High Priest", 6, Faction::IMPERIALE, 6, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_VIE, 2, TypeCible::SOI))
            ->ajouterEffetActiver(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_VIE, 2, TypeCible::SOI))
            // effetAllie : 2 damage for each champion -> TODO: scale
    );

    deck.ajouterDessous(
        (new Champion("Man-at-Arms", 3, Faction::IMPERIALE, 4, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            // +1 combat per other guard -> TODO scaling
    );

    deck.ajouterDessous(
        (new Champion("Man-at-Arms", 3, Faction::IMPERIALE, 4, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            // +1 combat per other guard -> TODO scaling
    );

    deck.ajouterDessous(
        (new Champion("Master Weyan", 4, Faction::IMPERIALE, 4, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            // +1 per other champion -> TODO
    );

    deck.ajouterDessous(
        (new CarteAction("Rally the Troops", 4, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_VIE, 5, TypeCible::SOI))
            // Ally: Prepare a champion -> TODO
    );

    deck.ajouterDessous(
        (new CarteAction("Recruit", 2, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_VIE, 3, TypeCible::SOI))
            // +1 health per champion -> TODO scaling
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_OR, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Recruit", 2, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_VIE, 3, TypeCible::SOI))
            // +1 health per champion -> TODO scaling
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_OR, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Recruit", 2, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_VIE, 3, TypeCible::SOI))
            // +1 health per champion -> TODO scaling
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_OR, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Tithe Priest", 2, Faction::IMPERIALE, 3, false))
            ->ajouterEffetActiver(Effet(TypeEffet::OR_OU_VIE, /*3 ou 1 par champion en jeu*/, TypeCible::SOI))
            // TODO : Là ça combine le pb de scaling et de choix
    );

    deck.ajouterDessous(
        (new Champion("Tithe Priest", 2, Faction::IMPERIALE, 3, false))
            ->ajouterEffetActiver(Effet(TypeEffet::OR_OU_VIE, /*3 ou 1 par champion en jeu*/, TypeCible::SOI))
            // TODO : Là ça combine le pb de scaling et de choix
    );

    deck.ajouterDessous(
        (new CarteAction("Taxation", 1, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_VIE, 6, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Taxation", 1, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_VIE, 6, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Taxation", 1, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_VIE, 6, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Word of Power", 6, Faction::IMPERIALE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::PIOCHER, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_VIE, 5, TypeCible::SOI))
            ->ajouterEffetSacrifice(Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
    );

    // === GUILDE ===
    deck.ajouterDessous(
        (new Champion("Borg, Ogre Mercenary", 6, Faction::GUILDE, 6, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Bribe", 3, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            // Ally: put next action acquired this turn on top of deck -> TODO complex
            // Donc genre un typeEffet AJOUTER_DESSUS_DECK ?
    );

    deck.ajouterDessous(
        (new CarteAction("Bribe", 3, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            // Ally: put next action acquired this turn on top of deck -> TODO complex
            // Donc genre un typeEffet AJOUTER_DESSUS_DECK ?
    );

    deck.ajouterDessous(
        (new CarteAction("Bribe", 3, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            // Ally: put next action acquired this turn on top of deck -> TODO complex
            // Donc genre un typeEffet AJOUTER_DESSUS_DECK ?
    );

    deck.ajouterDessous(
        (new CarteAction("Death Threat", 3, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 1, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::ASSOMMER_CHAMPION, 0, TypeCible::CHAMPION))
    );

    deck.ajouterDessous(
        (new CarteAction("Deception", 5, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
            // ->ajouterEffetAllie(Effet(TypeEffet::ACQUERIR_CARTE_EN_MAIN, 0, TypeCible::SOI))
            // Ally : put next acquired into hand -> represent as ACQUERIR_CARTE_EN_MAIN w/ TODO
    );

    deck.ajouterDessous(
        (new CarteAction("Fire Bomb", 8, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 8, TypeCible::SOI))
            ->ajouterEffetPrimaire(Effet(TypeEffet::ASSOMMER_CHAMPION, 0, TypeCible::CHAMPION))
            ->ajouterEffetPrimaire(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
            ->ajouterEffetSacrifice(Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Hit Job", 4, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 7, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::ASSOMMER_CHAMPION, 0, TypeCible::CHAMPION))
    );

    deck.ajouterDessous(
        (new CarteAction("Intimidation", 2, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Intimidation", 2, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Myros, Guild Mage", 5, Faction::GUILDE, 3, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Parov, the Enforcer", 5, Faction::GUILDE, 5, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Profit", 1, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Profit", 1, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Profit", 1, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Rake, Master Assassin", 7, Faction::GUILDE, 7, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
            ->ajouterEffetActiver(Effet(TypeEffet::ASSOMMER_CHAMPION, 0, TypeCible::CHAMPION))
            // may stun target champion -> TODO option to stun
            // On doit vraiment faire une diff entre ASSOMMER_CHAMPION et PEUT_ASSOMMER_CHAMPION ??
            // (ya pas le "may" dans les autres cartes)
    );

    deck.ajouterDessous(
        (new Champion("Rasmus, the Smuggler", 4, Faction::GUILDE, 5, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            // Ally: put next action acquired this turn on top of deck -> TODO complex
            // Donc genre un typeEffet AJOUTER_DESSUS_DECK ?
    );

    deck.ajouterDessous(
       (new CarteAction("Smash and Grab", 6, Faction::GUILDE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 6, TypeCible::SOI))
            // effetPrimaire : You may put a card from discard on top of deck -> TODO
    );

    deck.ajouterDessous(
        (new Champion("Street Thug", 3, Faction::GUILDE, 4, false))
            ->ajouterEffetActiver(Effet(TypeEffet::OR_OU_COMBAT, 1, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Street Thug", 3, Faction::GUILDE, 4, false))
            ->ajouterEffetActiver(Effet(TypeEffet::OR_OU_COMBAT, 1, 2, TypeCible::SOI))
    );

    // === NECROS ===
    deck.ajouterDessous(
        (new Champion("Cult Priest", 3, Faction::NECROS, 4, false))
            ->ajouterEffetActiver(Effet(TypeEffet::OR_OU_COMBAT, 1, TypeCible::SOI)) // Pour une fois c'est 1 ou 1 donc c ok
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Cult Priest", 3, Faction::NECROS, 4, false))
            ->ajouterEffetActiver(Effet(TypeEffet::OR_OU_COMBAT, 1, TypeCible::SOI)) // Pour une fois c'est 1 ou 1 donc c ok
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Dark Energy", 4, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 7, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Dark Reward", 5, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard -> TODO (sacrifice trigger)
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 6, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Death Cultist", 2, Faction::NECROS, 3, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Death Cultist", 2, Faction::NECROS, 3, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Death Touch", 1, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard -> TODO (sacrifice trigger)
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Death Touch", 1, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard -> TODO (sacrifice trigger)
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Death Touch", 1, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard -> TODO (sacrifice trigger)
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Rayla, Endweaver", 4, Faction::NECROS, 4, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Influence", 2, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            ->ajouterEffetSacrifice(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Influence", 2, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            ->ajouterEffetSacrifice(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Influence", 2, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 3, TypeCible::SOI))
            ->ajouterEffetSacrifice(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Krythos, Master Vampire", 7, Faction::NECROS, 6, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard 
            // if you do, gain an additionnal 3 combat -> TODO (sacrifice trigger)
    );

    deck.ajouterDessous(
        (new CarteAction("Life Drain", 6, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 8, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard 
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Lys, the Unseen", 6, Faction::NECROS, 5, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard 
            // if you do, gain an additionnal 2 combat -> TODO (sacrifice trigger)
    );

    deck.ajouterDessous(
        (new CarteAction("The Rot", 3, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard 
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("The Rot", 3, Faction::NECROS))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
            // effetPrimaire : May sacrifice a card in your hand or discard 
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Tyrannor, the Devourer", 8, Faction::NECROS, 6, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
            // effetActiver : may sacrifice UP TO TWO cards in your hand or discard 
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Varrick, the Necromancer", 5, Faction::NECROS, 3, false))
            // effetActiver : Take a champion from your discard pile and put it on top of your deck.
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    // === WILD ===
    deck.ajouterDessous(
        (new Champion("Broelyn, Loreweaver", 4, Faction::SAUVAGE, 6, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            // Pas sûr à 100% de celle là
            ->ajouterEffetAllie(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
    );

    deck.ajouterDessous(
        (new Champion("Cron, the Berserker", 6, Faction::SAUVAGE, 6, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 5, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Dire Wolf", 5, Faction::SAUVAGE, 5, true))   
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Elven Curse", 3, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 6, TypeCible::SOI))
            // Encore pas sûr de celle là
            ->ajouterEffetPrimaire(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Elven Curse", 3, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 6, TypeCible::SOI))
            // Encore pas sûr de celle là
            ->ajouterEffetPrimaire(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Elven Gift", 2, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            // effetPrimaire : You may draw a card. If you do, discard a card -> TODO
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Elven Gift", 2, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            // effetPrimaire : You may draw a card. If you do, discard a card -> TODO
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Elven Gift", 2, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
            // effetPrimaire : You may draw a card. If you do, discard a card -> TODO
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Grak, Storm Giant", 8, Faction::SAUVAGE, 7, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 6, TypeCible::SOI))
            // effetActiver : You may draw a card. If you do, discard a card -> TODO
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
            // Ally: Draw a card, then discard a card -> TODO
    );

    deck.ajouterDessous(
        (new CarteAction("Nature's Bounty", 4, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_OR, 4, TypeCible::SOI))
            // Pas sûr de celle là non plus
            ->ajouterEffetAllie(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
            ->ajouterEffetSacrifice(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Orc Grunt", 3, Faction::SAUVAGE, 3, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new Champion("Orc Grunt", 3, Faction::SAUVAGE, 3, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            ->ajouterEffetAllie(Effet(TypeEffet::PIOCHER, 1, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Rampage", 6, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 6, TypeCible::SOI))
            // You may draw up to two cards, then discard that many -> TODO
    );

    deck.ajouterDessous(
        (new Champion("Torgen Rocksplitter", 7, Faction::SAUVAGE, 7, true))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 4, TypeCible::SOI))
            // Pas sûr
            ->ajouterEffetActiver(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
    );

    deck.ajouterDessous(
        (new CarteAction("Spark", 1, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            // Pas sûr
            ->ajouterEffetPrimaire(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Spark", 1, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            // Pas sûr
            ->ajouterEffetPrimaire(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Spark", 1, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI))
            // Pas sûr
            ->ajouterEffetPrimaire(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
            ->ajouterEffetAllie(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
    );

    deck.ajouterDessous(
        (new CarteAction("Wolf Form", 5, Faction::SAUVAGE))
            ->ajouterEffetPrimaire(Effet(TypeEffet::AJOUT_COMBAT, 8, TypeCible::SOI))
            // Pas sûr des 2 suivantes
            ->ajouterEffetPrimaire(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
            ->ajouterEffetSacrifice(Effet(TypeEffet::DEFAUSSER, 1, TypeCible::ADVERSAIRE))
    );

    deck.ajouterDessous(
        (new Champion("Wolf Shaman", 2, Faction::SAUVAGE, 4, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 1, TypeCible::SOI)) 
            // +1 per other Wild card -> TODO scaling
    );

    deck.ajouterDessous(
        (new Champion("Wolf Shaman", 2, Faction::SAUVAGE, 4, false))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 2, TypeCible::SOI))
            ->ajouterEffetActiver(Effet(TypeEffet::AJOUT_COMBAT, 1, TypeCible::SOI))
            // +1 per other Wild card -> TODO scaling
    );

    deck.melanger();
    return deck;
}

Pile BibliothequeCartes::creerPileGemmeDeFeu() {
    Pile pile;
    for (int i = 0; i < 16; ++i) {
        pile.ajouterDessus(new CarteObjet("Gemme de feu", 2, Faction::Neutre))
        .ajouterEffetPrimaire(new Effet(TypeEffet::AJOUT_OR, 2, TypeCible::SOI))
        .ajouterEffetSacrifice(new Effet(TypeEffet::AJOUT_COMBAT, 3, TypeCible::SOI));
    }
    return pile;
}