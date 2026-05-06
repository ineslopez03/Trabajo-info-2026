#pragma once
#include <SFML/Graphics.hpp>
#include "Pieza.h" 

class Proyectiles {
private:
    float x, y;
    int danyo;
    float velocidad;
    sf::Vector2f direccion;
    Bando bandoOrigen;
    sf::CircleShape forma;

public:
    // Asegúrate de que los tipos coincidan con los de SFML 3
    Proyectiles(float _x, float _y, int _danyo, float _vel, sf::Vector2f _dir, Bando _bando);
    ~Proyectiles();

    void mover();
    bool comprobarImpacto(Pieza* objetivo);
    void dibujar(sf::RenderWindow& ventana); // Debe llevar el sf:: adelante

    sf::Vector2f getPosicion() const { return { x, y }; }
};
