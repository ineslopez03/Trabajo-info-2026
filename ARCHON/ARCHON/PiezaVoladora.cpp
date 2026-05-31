#include "PiezaVoladora.h"
#include "Casilla.h"
#include <cmath>
#include <algorithm>
#include <iostream>

void PiezaVoladora::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {//render en el tablero
    if (posicion == nullptr) return;//seguridad

    if (this->estaEncarcelada()) {//hechizo de prision
        sprite.setColor(sf::Color(100, 100, 255, 180));//azul
    }
    else {
        sprite.setColor(sf::Color::White);//normal
    }

    float destinoX = (float)posicion->getX() * tamano;//coordenadas logicas x
    float destinoY = (float)posicion->getY() * tamano;//coordenadas logicas y
    sf::Vector2f posDestino = { destinoX + tamano / 2.f, destinoY + tamano / 2.f };//centro de su casilla

    sf::Vector2f posActual = sprite.getPosition();//interpolacion para movimiento suave
    sf::Vector2f direccion = posDestino - posActual;//vector direccion
    float distancia = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);//modulo del vector

    if (distancia > 1.0f) {//si aun no ha llegado
        sprite.move(direccion * 0.015f);//avanza un porcentaje de la distancia (lerp)
    }
    else {
        sprite.setPosition(posDestino);//lo ancla si ya esta muy cerca
    }

    static sf::Clock clock;//reloj estatico para animacion
    float offset = std::sin(clock.getElapsedTime().asSeconds() * 3.f) * 4.f;//efecto de levitacion con onda senoidal
    sf::Vector2f posVisual = sprite.getPosition();//guardamos la posicion lerp
    sprite.move({ 0, offset });//aplicamos la onda

    sf::FloatRect bounds = sprite.getLocalBounds();//limites
    if (bounds.size.x > 0.1f) {//si es valido
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });//centramos
        float escala = (tamano * 0.8f) / bounds.size.x;//escalado estandar
        sprite.setScale({ escala, escala });//aplica escala
        ventana.draw(sprite);//dibuja en pantalla
    }
    sprite.setPosition(posVisual);//restauramos sin el offset para el calculo del siguiente frame
}

bool PiezaVoladora::mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) {//validacion logica aerea
    if (!origen || !destino) return false;//punteros seguros

    if (this->estaEncarcelada()) {//restriccion por magia
        std::cout << "DEBUG: La pieza esta encarcelada y no puede moverse." << std::endl;//traza
        return false;//deniega
    }

    if (destino->estaOcupada() && destino->getPieza()->getBando() == this->bando) {//casilla amiga
        return false;//no se puede comer a si mismo
    }

    int x1 = origen->getX();//origen x
    int y1 = origen->getY();//origen y
    int x2 = destino->getX();//destino x
    int y2 = destino->getY();//destino y
    int diffX = std::abs(x2 - x1);//distancia x
    int diffY = std::abs(y2 - y1);//distancia y
    int distancia = std::max(diffX, diffY);//como vuela en diagonal, se toma el mayor desplazamiento como coste

    if (distancia > this->velMov || distancia == 0) {//si supera su rango
        return false;//fuera de alcance
    }

    return true;//las voladoras saltan obstaculos, no miramos colisiones intermedias
}

void PiezaVoladora::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {//render de combate
    sprite.setTexture(textura);//garantiza textura original
    sprite.setTextureRect(sf::IntRect({ 0, 0 }, { (int)textura.getSize().x, (int)textura.getSize().y }));//resetea

    if (this->estaEncarcelada()) {//consistencia visual de estado alterado
        sprite.setColor(sf::Color(100, 100, 255, 180));//azul
    }
    else {
        sprite.setColor(sf::Color::White);//normal
    }

    static sf::Clock clockA;//reloj estatico para animacion de arena
    float offset = std::sin(clockA.getElapsedTime().asSeconds() * 4.f) * 8.f;//onda senoidal mas marcada
    sf::Vector2f posAnimada = { pos.x, pos.y + offset };//le sumamos el efecto a la fisica

    sf::FloatRect bounds = sprite.getLocalBounds();//limites
    if (bounds.size.x > 0.1f) {//si existe
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });//centro
        sprite.setPosition(posAnimada);//colocamos con onda
        float escala = (65.f * factorEscalaExtra) / bounds.size.x;//escala estandar multiplicada por su parametro individual
        sprite.setScale({ mirandoDerecha ? escala : -escala, escala });//rotacion de espejo
        ventana.draw(sprite);//dibuja
    }
}