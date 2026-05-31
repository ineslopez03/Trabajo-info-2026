#pragma once
#include "Pieza.h"

class PiezaTeletransporte : public Pieza {//hereda de base
public:
    PiezaTeletransporte(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)//constructor
        : Pieza(_v, _d, _vm, _va, _tex, _b) {
    }

    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) override;//renderizado teletransporte
    bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) override;//movimiento omnidireccional instantaneo
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;//render arena
};