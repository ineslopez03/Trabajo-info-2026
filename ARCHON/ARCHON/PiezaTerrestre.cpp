#include "PiezaTerrestre.h"
#include "Casilla.h"
#include <iostream>

void PiezaTerrestre::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    if (posicion == nullptr) {
        // Si ves esto en la consola, es que setPieza() no se llamó bien
        // std::cout << "DEBUG: Pieza sin posicion asignada!" << std::endl;
        return;
    }

    float posX = (float)posicion->getX() * tamano;
    float posY = (float)posicion->getY() * tamano;
    sf::Vector2f centroCasilla = { posX + tamano / 2.f, posY + tamano / 2.f };

    sf::FloatRect bounds = sprite.getLocalBounds();

    // Si la textura existe
    if (bounds.size.x > 0.1f) {
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setPosition(centroCasilla);
        float escala = (tamano * 0.8f) / bounds.size.x;
        sprite.setScale({ escala, escala });
        ventana.draw(sprite);
    }
    
}
bool PiezaTerrestre::mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) {
    if (!origen || !destino) return false;

    // 1. No podemos movernos a una casilla ocupada por nosotros mismos
    if (destino->estaOcupada() && destino->getPieza()->getBando() == this->bando) {
        return false;
    }

    int x1 = origen->getX();
    int y1 = origen->getY();
    int x2 = destino->getX();
    int y2 = destino->getY();

    int diffX = std::abs(x2 - x1);
    int diffY = std::abs(y2 - y1);

    // 2. Regla Archon Terrestre: Solo horizontal o vertical
    if (diffX != 0 && diffY != 0) return false;

    // 3. Validar el rango (usando la variable de la clase base)
    int distancia = diffX + diffY;
    if (distancia > this->rangoMovimiento) return false;

    // 4. No saltar sobre otras piezas (Camino despejado)
    int dirX = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);
    int dirY = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);

    int currX = x1 + dirX;
    int currY = y1 + dirY;

    while (currX != x2 || currY != y2) {
        if (matriz[currX][currY]->estaOcupada()) {
            return false; // Bloqueado
        }
        currX += dirX;
        currY += dirY;
    }

    return true;
}