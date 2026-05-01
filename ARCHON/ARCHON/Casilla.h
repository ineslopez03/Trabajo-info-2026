#pragma once
#include "Pieza.h" 

class Casilla {
private:
    int x, y;
    bool esOscilante;
    Pieza* piezaOcupante; 

public:
    Casilla(int _x, int _y, bool _osc)
        : x(_x), y(_y), esOscilante(_osc), piezaOcupante(nullptr) {}

    void setPieza(Pieza* p) {
        piezaOcupante = p;
        // Si la pieza no es nula, actualizamos también la posición de la pieza
        if (p != nullptr) p->setPosicion(this);
    }

    Pieza* getPieza() { return piezaOcupante; }
    bool estaOcupada() { return piezaOcupante != nullptr; }
    void dibujar(sf::RenderWindow& ventana);
};