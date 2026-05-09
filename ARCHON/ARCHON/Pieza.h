#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

enum class Bando { LUZ, OSCURIDAD };
class Casilla;
class Jugador;

class Pieza {
protected:
    int vida, danio, velMov, velAta;
    Bando bando;
    sf::Texture& textura;
    sf::Sprite sprite;
    Casilla* posicion;
    int rangoMovimiento;
public:
    Pieza(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)
        : vida(_v), danio(_d), velMov(_vm), velAta(_va), bando(_b),
        textura(_tex), sprite(_tex), posicion(nullptr), rangoMovimiento(0) {
    }

    virtual ~Pieza() {}

    virtual void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) = 0;

    void setPosicion(Casilla* c) { posicion = c; }
    Casilla* getPosicion() { return posicion; }
    Bando getBando() { return bando; }

    virtual void setJugador(Jugador* j) {}
    virtual bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9])=0;
};