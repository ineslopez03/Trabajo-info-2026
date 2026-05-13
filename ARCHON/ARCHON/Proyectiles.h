#pragma once
#include <SFML/Graphics.hpp>
#include "Pieza.h"
#include <string>

class Proyectiles {
private:
    float x, y;
    int danyo;
    float velocidad;
    sf::Vector2f direccion;
    Bando bandoOrigen;

    // Doble geometría para efecto de iluminación
    sf::RectangleShape nucleo;
    sf::RectangleShape resplandor;

public:
    Proyectiles(float _x, float _y, int _danyo, float _vel, sf::Vector2f _dir, Bando _bando, std::string skin);
    ~Proyectiles();

    void mover(float dt);
    void dibujar(sf::RenderWindow& ventana);

    sf::Vector2f getPosicion() const { return { x, y }; }
    Bando getBando() const { return bandoOrigen; }
    int getDanyo() const { return danyo; }
};