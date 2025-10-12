#include "view/console/consoleView.h"
#include "controller/gameController.h"
#include <memory>

int main() {
    try {
        // Créer la vue console
        ConsoleView view = ConsoleView();
        
        // Créer le contrôleur avec la vue
        GameController controller(&view);
        
        // Lancer le jeu
        controller.lancerJeu();
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}