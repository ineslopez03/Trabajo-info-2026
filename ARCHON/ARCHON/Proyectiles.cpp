#include "Proyectiles.h"

Proyectiles::Proyectiles(float _x, float _y, int _danyo, float _vel, sf::Vector2f _dir, Bando _bando)
    : x(_x), y(_y), danyo(_danyo), velocidad(_vel), direccion(_dir), bandoOrigen(_bando) {

    forma.setRadius(5.f);
    forma.setFillColor(sf::Color::Yellow);

    
    forma.setPosition({ x, y }); 
}

Proyectiles::~Proyectiles() {}

void Proyectiles::mover() {
    x += direccion.x * velocidad;
    y += direccion.y * velocidad;

    
    forma.setPosition({ x, y }); 
}

bool Proyectiles::comprobarImpacto(Pieza* objetivo) {
    if (!objetivo || objetivo->getBando() == bandoOrigen) return false;

    // Verificación de colisión simple por área
    sf::FloatRect bounds = forma.getGlobalBounds();
    // Aquí se integraría con la posición real de la pieza en la Arena
    return false;
}

void Proyectiles::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(forma);
}