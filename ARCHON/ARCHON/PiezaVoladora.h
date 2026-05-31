#pragma once
#include "Pieza.h"

class PiezaVoladora : public Pieza {//hereda de pieza abstracta
protected:
    float factorEscalaExtra = 1.0f;//por si algun hijo (como el elemental) debe verse mas grande
public:
    PiezaVoladora(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)//constructor
        : Pieza(_v, _d, _vm, _va, _tex, _b) {
    }

    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) override;//renderizado flotante
    bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) override;//movimiento aereo libre
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;//render en combate
};