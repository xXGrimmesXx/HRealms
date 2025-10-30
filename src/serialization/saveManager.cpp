#include "saveManager.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace HRealms {
namespace Serialization {

const std::string SaveManager::DOSSIER_SAVE = "saves";

std::string SaveManager::obtenirCheminComplet(const std::string& nomFichier) {
    std::string fichier = nomFichier;
    
    // Ajouter l'extension .json si elle n'est pas présente
    if (fichier.size() < 5 || fichier.substr(fichier.size() - 5) != ".json") {
        fichier += ".json";
    }
    
    return DOSSIER_SAVE + "/" + fichier;
}

bool SaveManager::creerDossierSavesSiNecessaire() {
    try {
        std::cout << "[DEBUG] Vérification du dossier: " << DOSSIER_SAVE << std::endl;
        
        if (!fs::exists(DOSSIER_SAVE)) {
            std::cout << "[DEBUG] Le dossier n'existe pas. Tentative de création..." << std::endl;
            bool resultat = fs::create_directory(DOSSIER_SAVE);
            
            if (resultat) {
                std::cout << "[DEBUG] Dossier créé avec succès!" << std::endl;
            } else {
                std::cerr << "[ERREUR] Échec de la création du dossier!" << std::endl;
            }
            
            return resultat;
        }
        
        std::cout << "[DEBUG] Le dossier existe déjà." << std::endl;
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "[ERREUR] Exception lors de la création du dossier saves: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::sauvegarderPartie(Partie* partie, const std::string& nomFichier) {
    std::cout << "[DEBUG] Début de sauvegarderPartie..." << std::endl;
    
    if (!partie) {
        std::cerr << "[ERREUR] Partie nulle!" << std::endl;
        return false;
    }
    
    std::cout << "[DEBUG] Partie valide, création du dossier..." << std::endl;
    
    // Créer le dossier saves s'il n'existe pas
    if (!creerDossierSavesSiNecessaire()) {
        std::cerr << "[ERREUR] Impossible de créer le dossier saves!" << std::endl;
        return false;
    }
    
    std::cout << "[DEBUG] Dossier prêt, début de la sérialisation..." << std::endl;
    
    try {
        // Sérialiser la partie en JSON
        json j = partieToJson(partie);
        
        std::cout << "[DEBUG] Sérialisation réussie, taille JSON: " << j.dump().size() << " caractères" << std::endl;
        
        // Obtenir le chemin complet du fichier
        std::string cheminComplet = obtenirCheminComplet(nomFichier);
        
        std::cout << "[DEBUG] Chemin complet du fichier: " << cheminComplet << std::endl;
        
        // Écrire dans le fichier
        std::ofstream fichier(cheminComplet);
        if (!fichier.is_open()) {
            std::cerr << "[ERREUR] Impossible d'ouvrir le fichier " << cheminComplet << std::endl;
            return false;
        }
        
        std::cout << "[DEBUG] Fichier ouvert, écriture en cours..." << std::endl;
        
        // Écrire le JSON avec indentation pour la lisibilité
        fichier << j.dump(4);
        fichier.close();
        
        std::cout << "[SUCCESS] Partie sauvegardée avec succès dans: " << cheminComplet << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "[ERREUR] Exception lors de la sauvegarde: " << e.what() << std::endl;
        return false;
    }
}

Partie* SaveManager::chargerPartie(const std::string& nomFichier) {
    try {
        // Obtenir le chemin complet du fichier
        std::string cheminComplet = obtenirCheminComplet(nomFichier);
        
        // Vérifier que le fichier existe
        if (!fs::exists(cheminComplet)) {
            std::cerr << "Erreur: le fichier " << cheminComplet << " n'existe pas" << std::endl;
            return nullptr;
        }
        
        // Lire le fichier
        std::ifstream fichier(cheminComplet);
        if (!fichier.is_open()) {
            std::cerr << "Erreur: impossible d'ouvrir le fichier " << cheminComplet << std::endl;
            return nullptr;
        }
        
        // Parser le JSON
        json j;
        fichier >> j;
        fichier.close();
        
        // Désérialiser la partie
        Partie* partie = jsonToPartie(j);
        
        if (partie) {
            std::cout << "Partie chargée avec succès depuis: " << cheminComplet << std::endl;
        } else {
            std::cerr << "Erreur lors de la désérialisation de la partie" << std::endl;
        }
        
        return partie;
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement: " << e.what() << std::endl;
        return nullptr;
    }
}

std::vector<std::string> SaveManager::listerSauvegardes() {
    std::vector<std::string> sauvegardes;
    
    try {
        // Vérifier que le dossier existe
        if (!fs::exists(DOSSIER_SAVE)) {
            return sauvegardes; // Retourner une liste vide
        }
        
        // Parcourir les fichiers du dossier
        for (const auto& entry : fs::directory_iterator(DOSSIER_SAVE)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                sauvegardes.push_back(entry.path().filename().string());
            }
        }
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erreur lors du listage des sauvegardes: " << e.what() << std::endl;
    }
    
    return sauvegardes;
}

bool SaveManager::supprimerSauvegarde(const std::string& nomFichier) {
    try {
        std::string cheminComplet = obtenirCheminComplet(nomFichier);
        
        if (!fs::exists(cheminComplet)) {
            std::cerr << "Erreur: le fichier " << cheminComplet << " n'existe pas" << std::endl;
            return false;
        }
        
        fs::remove(cheminComplet);
        std::cout << "Sauvegarde supprimée: " << cheminComplet << std::endl;
        return true;
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Erreur lors de la suppression: " << e.what() << std::endl;
        return false;
    }
}

} // namespace Serialization
} // namespace HRealms
