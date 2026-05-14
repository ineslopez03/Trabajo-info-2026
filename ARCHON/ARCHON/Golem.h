#pragma once
#include "PiezaTerrestre.h"
#include <string>

class Golem : public PiezaTerrestre {
private:
    static std::string obtenerRuta(Bando b, std::string skin);
public:
    Golem(Bando b, std::string skin);
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;
};