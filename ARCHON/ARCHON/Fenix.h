#pragma once
#include "PiezaVoladora.h"
#include <string>

class Fenix : public PiezaVoladora {//Hereda de pieza voladora
private:
    // Devuelve la ruta de imagen según bando y skin
    static std::string obtenerRuta(Bando b, std::string skin);
public:
    Fenix(Bando b, std::string skin);// Constructor del Fénix
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;
    // Sobrescribe el dibujado en arena (delega a PiezaVoladora)
};