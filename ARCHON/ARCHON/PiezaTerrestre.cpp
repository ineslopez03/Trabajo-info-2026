#include "PiezaTerrestre.h"
#include "Casilla.h"
#include <iostream>

void PiezaTerrestre::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {//metodo de render en tablero
    if (posicion == nullptr) return;//seguridad

    if (this->estaEncarcelada()) {//si esta bajo efecto de imprison
        sprite.setColor(sf::Color(100, 100, 255, 180));//tinte azulado semitransparente
    }
    else {
        sprite.setColor(sf::Color::White);//color por defecto
    }

    float posX = (float)posicion->getX() * tamano;//calcula posicion en pixeles
    float posY = (float)posicion->getY() * tamano;//calcula posicion en pixeles
    sf::Vector2f centroCasilla = { posX + tamano / 2.f, posY + tamano / 2.f };//centro geometrico
    sf::FloatRect bounds = sprite.getLocalBounds();//medidas de la textura

    if (bounds.size.x > 0.1f) {//si la textura existe
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });//centra el pivote
        sprite.setPosition(centroCasilla);//coloca en el centro de la celda
        float escala = (tamano * 0.8f) / bounds.size.x;//escala para ocupar el 80% del hueco
        sprite.setScale({ escala, escala });//aplica la escala
        ventana.draw(sprite);//lanza a pantalla
    }
}

bool PiezaTerrestre::mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) {//validacion de logica de movimiento
    if (!origen || !destino) return false;//seguridad de punteros

    if (this->estaEncarcelada()) {//si no se puede mover por magia
        std::cout << "DEBUG: La pieza esta encarcelada y no puede moverse." << std::endl;//traza de error
        return false;//deniega el movimiento
    }

    if (destino->estaOcupada() && destino->getPieza()->getBando() == this->bando) {//fuego amigo
        return false;//no puede pisar a los suyos
    }

    int x1 = origen->getX();//coordenada origen x
    int y1 = origen->getY();//coordenada origen y
    int x2 = destino->getX();//coordenada destino x
    int y2 = destino->getY();//coordenada destino y
    int diffX = std::abs(x2 - x1);//distancia x
    int diffY = std::abs(y2 - y1);//distancia y

    if (diffX != 0 && diffY != 0) return false;//regla archon terrestre: solo recto, no diagonal

    int distancia = diffX + diffY;//distancia total
    if (distancia > this->rangoMovimiento) return false;//no puede superar su stat de movilidad

    int dirX = (x2 > x1) ? 1 : (x2 < x1 ? -1 : 0);//vector unitario direccion x
    int dirY = (y2 > y1) ? 1 : (y2 < y1 ? -1 : 0);//vector unitario direccion y
    int currX = x1 + dirX;//paso a paso x
    int currY = y1 + dirY;//paso a paso y

    while (currX != x2 || currY != y2) {//recorre el trayecto
        if (matriz[currX][currY]->estaOcupada()) {//si hay alguien en el camino
            return false;//no puede saltar piezas
        }
        currX += dirX;//avanza x
        currY += dirY;//avanza y
    }
    return true;//el camino es valido
}

void PiezaTerrestre::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {//render en combate
    sprite.setTexture(textura);//garantizamos que el sprite apunte a la textura original
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { (int)textura.getSize().x, (int)textura.getSize().y }));//reseteamos el rect

    if (this->estaEncarcelada()) {//mantenemos la consistencia visual del estado alterado
        sprite.setColor(sf::Color(100, 100, 255, 180));//azulada
    }
    else {
        sprite.setColor(sf::Color::White);//normal
    }

    sf::FloatRect bounds = sprite.getLocalBounds();//leemos el tamaño
    if (bounds.size.x > 0.1f) {//si existe
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });//centramos
        sprite.setPosition(pos);//posicion dada por el motor de fisicas
        float escala = 65.f / bounds.size.x;//forzamos un tamaño visual estandarizado para la arena
        sprite.setScale({ mirandoDerecha ? escala : -escala, escala });//invertimos el eje x si mira a la izquierda
        ventana.draw(sprite);//pintamos
    }
}