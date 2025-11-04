#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include "jsonSerializer.h"
#include "partie.h"

using json = nlohmann::json;

namespace HRealms {
namespace Serialization {

class SaveManager {
public:
    // Sauvegarder une partie dans un fichier
    static bool sauvegarderPartie(Partie* partie, const std::string& nomFichier);
    
    // Charger une partie depuis un fichier
    static Partie* chargerPartie(const std::string& nomFichier);
    
    // Lister les sauvegardes disponibles
    static std::vector<std::string> listerSauvegardes();
    
    // Supprimer une sauvegarde
    static bool supprimerSauvegarde(const std::string& nomFichier);
    
private:
    static const std::string DOSSIER_SAVE;
    static std::string obtenirCheminComplet(const std::string& nomFichier);
    static bool creerDossierSavesSiNecessaire();
};

} // namespace Serialization
} // namespace HRealms
