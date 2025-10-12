#include "champion.h"
#include "joueur.h"
#include "partie.h"


void Champion::mobiliser(Joueur& j, Partie& p) {
    if (!mobilise) {
        mobilise = true;
        for (const Effet& effet : effetsActiver) {
            effet.appliquer(this, &j, p);
        }
    } else {
        throw std::runtime_error("Champion already mobilized");
    }
}