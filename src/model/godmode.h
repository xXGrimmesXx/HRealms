#pragma once

class GodMode {
public:
    bool actif = false;
    void activer() { actif = true; }
    void desactiver() { actif = false; }
};
