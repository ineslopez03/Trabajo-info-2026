#include "PiezaVoladora.h"
#include "Casilla.h"
#include <cmath>
#include <algorithm>

void PiezaVoladora::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    if (posicion == nullptr) return;
    if (this->estaEncarcelada()) {
        // Tinte azulado y un poco transparente para que parezca un bloque de hielo/magia
        sprite.setColor(sf::Color(100, 100, 255, 180));
    }
    else {
        // Color original
        sprite.setColor(sf::Color::White);
    }
    // 1. Coordenadas de la casilla donde debería estar (Destino Lógico)
    float destinoX = (float)posicion->getX() * tamano;
    float destinoY = (float)posicion->getY() * tamano;
    sf::Vector2f posDestino = { destinoX + tamano / 2.f, destinoY + tamano / 2.f };

    // 2. Interpolación para el desplazamiento suave
    // Si la posición actual del sprite está lejos del destino, lo acercamos poco a poco
    sf::Vector2f posActual = sprite.getPosition();
    sf::Vector2f direccion = posDestino - posActual;
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    if (distancia > 1.0f) {
        // Velocidad del vuelo 
        sprite.move(direccion * 0.015f);
    }
    else {
        sprite.setPosition(posDestino);
    }

    // 3. Efecto de levitación (se mantiene mientras vuela y cuando está quieta)
    static sf::Clock clock;
    float offset = std::sin(clock.getElapsedTime().asSeconds() * 3.f) * 4.f;

    // Dibujamos con un pequeño offset vertical para que parezca que flota
    sf::Vector2f posVisual = sprite.getPosition();
    sprite.move({ 0, offset });

    sf::FloatRect bounds = sprite.getLocalBounds();
    if (bounds.size.x > 0.1f) {
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        float escala = (tamano * 0.8f) / bounds.size.x;
        sprite.setScale({ escala, escala });
        ventana.draw(sprite);
    }

    // Restauramos la posición sin el offset para el siguiente frame
    sprite.setPosition(posVisual);
}

bool PiezaVoladora::mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) {
    if (!origen || !destino) return false;
    // --- BLOQUE PARA HECHIZO IMPRISON ---
    // Si la pieza está encarcelada, no permitimos que se ejecute ninguna lógica de movimiento
    if (this->estaEncarcelada()) {
        std::cout << "DEBUG: La pieza esta encarcelada y no puede moverse." << std::endl;
        return false;
    }
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