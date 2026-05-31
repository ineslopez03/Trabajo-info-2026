#include "PiezaTeletransporte.h"
#include "Casilla.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void PiezaTeletransporte::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {//render de tablero
    if (posicion == nullptr) return;//seguridad

    if (this->estaEncarcelada()) {//si esta bajo efecto imprison
        sprite.setColor(sf::Color(100, 100, 255, 180));//azulada
    }
    else {
        sprite.setColor(sf::Color::White);//normal
    }

    float posX = (float)posicion->getX() * tamano;//pixel x
    float posY = (float)posicion->getY() * tamano;//pixel y

    static sf::Clock clock;//reloj para animacion propia
    float offset = std::sin(clock.getElapsedTime().asSeconds() * 3.f) * 4.f;//efecto sutil de magia vibrante
    sf::Vector2f centroCasilla = { posX + tamano / 2.f, (posY + tamano / 2.f) + offset };//posicion con la onda

    sf::FloatRect bounds = sprite.getLocalBounds();//limites
    if (bounds.size.x > 0.1f) {//si es valido
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });//centro
        sprite.setPosition(centroCasilla);//anclamos a casilla
        float escala = (tamano * 0.8f) / bounds.size.x;//escala estandar
        sprite.setScale({ escala, escala });//aplicamos
        ventana.draw(sprite);//pintamos
    }
}

bool PiezaTeletransporte::mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) {//validacion de movimiento magico
    if (!origen || !destino) return false;//seguridad de memoria

    if (this->estaEncarcelada()) {//chequeo de magia bloqueadora
        std::cout << "DEBUG: La pieza esta encarcelada y no puede moverse." << std::endl;//traza log
        return false;//se le deniega el salto
    }

    if (destino->estaOcupada() && destino->getPieza()->getBando() == this->bando) {//fuego amigo
        return false;//no ataca a los suyos
    }

    int x1 = origen->getX();//origen x
    int y1 = origen->getY();//origen y
    int x2 = destino->getX();//destino x
    int y2 = destino->getY();//destino y
    int diffX = std::abs(x2 - x1);//diferencia x
    int diffY = std::abs(y2 - y1);//diferencia y
    int distancia = std::max(diffX, diffY);//distancia radial max

    if (distancia > this->velMov || distancia == 0) {//comprobacion de rango
        return false;//no llega
    }

    return true;//el teletransporte ignora todo el camino intermedio
}

void PiezaTeletransporte::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {//render combate
    sprite.setTexture(textura);//garantia de textura correcta
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { (int)textura.getSize().x, (int)textura.getSize().y }));//reseteo

    if (this->estaEncarcelada()) {//consistencia visual
        sprite.setColor(sf::Color(100, 100, 255, 180));//azul
    }
    else {
        sprite.setColor(sf::Color::White);//normal
    }

    sf::FloatRect bounds = sprite.getLocalBounds();//limites
    if (bounds.size.x > 0.1f) {//si hay grafico
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });//centro
        sprite.setPosition(pos);//colocacion fisica directa
        float escala = 65.f / bounds.size.x;//escalado logico arena
        sprite.setScale({ mirandoDerecha ? escala : -escala, escala });//espejo dinamico
        ventana.draw(sprite);//lanzamos a render
    }
}