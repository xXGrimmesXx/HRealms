#include "view/console/consoleView.h"
#include "controller/gameController.h"
#include <memory>

int main() {
    try {
        // Créer la vue console
        std::unique_ptr<ConsoleView> view = std::make_unique<ConsoleView>();
        
        // Créer le contrôleur avec la vue
        GameController controller(view.get());
        
        // Lancer le jeu
        controller.lancerJeu();
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}