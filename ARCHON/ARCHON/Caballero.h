#pragma once
#include "PiezaTerrestre.h"

class Caballero : public PiezaTerrestre {//Hereda de pieza terrestre(movimiento horizontal/vertical)
private:
    static std::string obtenerRuta(Bando b, std::string skin); // Devuelve la ruta de imagen según bando y skin
public:
    Caballero(Bando b, std::string skin); // Constructor del Caballero
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;// Sobrescribe el dibujado en arena (delega a PiezaTerrestre)
};