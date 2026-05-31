#pragma once
#include "Pieza.h"

class PiezaTerrestre : public Pieza {//hereda de la base abstracta
public:
    PiezaTerrestre(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)//constructor basico
        : Pieza(_v, _d, _vm, _va, _tex, _b) {
    }

    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) override;//dibujado estandar en tablero
    bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) override;//movimiento horizontal y vertical
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;//dibujado en combate
};