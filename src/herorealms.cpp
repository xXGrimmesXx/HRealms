#include "model/Partie.h"

int main() {
    Partie partie;
    partie.commencer();
    while (!partie.estTerminee()) {
        partie.jouerTour();
    }
    return 0;
}
