#pragma once
#include "PiezaTerrestre.h"

class Gigante : public PiezaTerrestre {
private:
    static std::string obtenerRuta(Bando b, std::string skin);
public:
    Gigante(Bando b, std::string skin);

    // Sobrescritura para inyectar la hoja de sprites en combate
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;
};