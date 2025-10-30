#pragma once
#include <string>
#include <vector>
#include <sstream>
#include "../../model/carte.h"
#include "../../model/champion.h"
#include "../../model/carteObjet.h"
#include "../../model/carteAction.h"

class CardRenderer {
public:
    static constexpr int CARD_WIDTH = 30;
    static constexpr int CARD_HEIGHT = 15;
    
    // Codes couleurs
    static constexpr const char* RESET = "\033[0m";
    static constexpr const char* RED = "\033[31m";
    static constexpr const char* GREEN = "\033[32m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* BLUE = "\033[34m";
    static constexpr const char* MAGENTA = "\033[35m";
    static constexpr const char* CYAN = "\033[36m";
    static constexpr const char* WHITE = "\033[37m";
    static constexpr const char* GRAY = "\033[90m";
    static constexpr const char* BOLD = "\033[1m";
    static constexpr const char* NORMAL = "\033[22m";
    
    static std::vector<std::string> renderCard(const Carte* carte, int index = -1);
    static std::vector<std::string> renderEmptySlot(int index);
    static void printCardsRow(const std::vector<std::vector<std::string>>& cards);
    static void printCards(const std::vector<Carte*> cartes, const int cardsPerRow = 5);
    
private:
    static std::string getFactionColor(Faction faction);
    static std::string getTypeSymbol(const Carte* carte);
    static std::vector<std::string> truncateText(const std::string& text, int maxLength);
    static std::string centerText(const std::string& text, int width);
    static std::string formatEffect(const Effet effet);
    static std::string getFactionSymbol(Faction faction);
};