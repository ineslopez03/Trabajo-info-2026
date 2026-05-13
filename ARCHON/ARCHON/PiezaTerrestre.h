#pragma once
#include "Pieza.h"

class PiezaTerrestre : public Pieza {
public:
    PiezaTerrestre(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)
        : Pieza(_v, _d, _vm, _va, _tex, _b) {
    }

    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) override;
    bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) override;

    // Nueva firma para el renderizado por defecto en el estado de combate
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;
};