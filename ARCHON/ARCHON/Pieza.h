#pragma once

class Casilla;

class Pieza {
protected:
    int vida;
    int danio;
    int velMov;
    int velAta;
    Casilla* posicion; 

public:
    Pieza(int _v, int _d, int _vm, int _va)
        : vida(_v), danio(_d), velMov(_vm), velAta(_va), posicion(nullptr) {
    }

    virtual ~Pieza() {}

    virtual void mover() = 0;
    virtual void atacar() = 0;
    virtual void dibujar(sf::RenderWindow& ventana) = 0;
    void setPosicion(Casilla* c) { posicion = c; }
    Casilla* getPosicion() { return posicion; }
};