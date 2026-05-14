#pragma once
#include "PiezaVoladora.h"
#include <string>

class Valquiria : public PiezaVoladora {
private:
    static std::string obtenerRuta(Bando b, std::string skin);
public:
    Valquiria(Bando b, std::string skin);
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;
};