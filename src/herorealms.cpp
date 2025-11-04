#include "consoleView.h"
#include "gameController.h"
#include <memory>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>

void enableWindowsConsoleColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

int main() {
    #ifdef _WIN32
        enableWindowsConsoleColors();
    #endif
    try {
        // Créer la vue console
        ConsoleView* view = new ConsoleView();
        
        // Créer le contrôleur avec la vue
        GameController controller(view);
        
        // Lancer le jeu
        controller.lancerJeu();
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur fatale: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}