#include "PiezaTeletransporte.h"
#include "Casilla.h"
#include <cmath>
#include <algorithm> // Para std::max

void PiezaTeletransporte::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    if (posicion == nullptr) return;

    float posX = (float)posicion->getX() * tamano;
    float posY = (float)posicion->getY() * tamano;

    static sf::Clock clock;
    float offset = std::sin(clock.getElapsedTime().asSeconds() * 3.f) * 4.f;

    sf::Vector2f centroCasilla = { posX + tamano / 2.f, (posY + tamano / 2.f) + offset };
    sf::FloatRect bounds = sprite.getLocalBounds();

 
    if (bounds.size.x > 0.1f) {
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setPosition(centroCasilla);

        float escala = (tamano * 0.8f) / bounds.size.x;
        sprite.setScale({ escala, escala });
        ventana.draw(sprite);
    }
}

bool PiezaTeletransporte::mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) {
    if (!origen || !destino) return false;

    // 1. Validar que el destino no sea un aliado 
    if (destino->estaOcupada() && destino->getPieza()->getBando() == this->bando) {
        return false;
    }

    int x1 = origen->getX();
    int y1 = origen->getY();
    int x2 = destino->getX();
    int y2 = destino->getY();

    
    int diffX = std::abs(x2 - x1);
    int diffY = std::abs(y2 - y1);
    int distancia = std::max(diffX, diffY);

    
    if (distancia > this->velMov || distancia == 0) {
        return false;
    }

    return true;
}