#include "PiezaTerrestre.h"
#include "Casilla.h"
#include <iostream>

void PiezaTerrestre::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    if (posicion == nullptr) return;

    if (this->estaEncarcelada()) {
        // Tinte azulado y un poco transparente para que parezca un bloque de hielo/magia
        sprite.setColor(sf::Color(100, 100, 255, 180));
    }
    else {
        // Color original
        sprite.setColor(sf::Color::White);
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

    // --- BLOQUE PARA HECHIZO IMPRISON ---
    // Si la pieza está encarcelada, no permitimos que se ejecute ninguna lógica de movimiento
    if (this->estaEncarcelada()) {
        std::cout << "DEBUG: La pieza esta encarcelada y no puede moverse." << std::endl;
        return false;
    }

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

void PiezaTerrestre::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    // 1. Garantizamos que el sprite apunte a la textura original del tablero por si heredó algún estado previo.
    sprite.setTexture(textura);
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { (int)textura.getSize().x, (int)textura.getSize().y }));

    // 2. Mantenemos la consistencia visual del estado alterado (Imprison) si entrase a combatir paralizada.
    if (this->estaEncarcelada()) {
        sprite.setColor(sf::Color(100, 100, 255, 180));
    }
    else {
        sprite.setColor(sf::Color::White);
    }

    sf::FloatRect bounds = sprite.getLocalBounds();

    if (bounds.size.x > 0.1f) {
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setPosition(pos);

        // 3. Cálculo del factor de escala. 
        // Forzamos un tamaño visual de aproximadamente 100 píxeles para que destaquen en la arena 800x800.
        float escala = 65.f / bounds.size.x;

        // 4. Transformación matricial: Invertimos el eje X si la pieza debe mirar hacia la izquierda (defensor).
        sprite.setScale({ mirandoDerecha ? escala : -escala, escala });

        ventana.draw(sprite);
    }
}