#pragma once 
#include <SFML/Graphics.hpp> 
#include "Pieza.h" 
#include <string> 

// clase general para el disparo de cualquier pieza
class Proyectiles {
private:
    float x, y; // donde viaja el proyectil
    int danyo; // cuantos puntos quita si da al rival
    float velocidad; // velocidad del laser
    sf::Vector2f direccion; // a donde apunta el laser
    Bando bandoOrigen; // guardamos el bando para no matarnos a nosotros mismos

    // el laser tiene dos partes
    sf::RectangleShape nucleo; // el nucleo es el centro del laser
    sf::RectangleShape resplandor; // brillo externo es el borde del laser

public:
    //recibe los datos desde la arena
    Proyectiles(float _x, float _y, int _danyo, float _vel, sf::Vector2f _dir, Bando _bando, std::string skin);

    // destructor vacio porque no se usa memoria dinamica
    ~Proyectiles();

    // se llama en cada frame para avanzar las coordenadas
    void mover(float dt);

    // dibuja el proyectil por encima del suelo
    void dibujar(sf::RenderWindow& ventana);

    // para la clase de arena
    sf::Vector2f getPosicion() const { return { x, y }; } // devuelve su posicion de hitbox
    Bando getBando() const { return bandoOrigen; } // dice si es de la luz o la oscuridad
    int getDanyo() const { return danyo; } // dice el daño que hace
};