#pragma once
#include "Pieza.h"
#include<SFML/Graphics.hpp>
#include"Constantes.h"

class Casilla {
private:
    int x, y;  //Coordenadas en el tablero(0-8)
    bool esOscilante = false; //Si cambia de color cada turno
    bool esPuntoDePoder = false;//Si es uno de los 5 nodos especiales
    Pieza* piezaOcupante; //Puntero a la pieza
    ColorCasilla Color;//Color actual de la casilla
public:
    Casilla(int _x, int _y); // Constructor: recibe coordenadas
    ~Casilla();// Destructor
    void setPieza(Pieza* p);// Asigna una pieza a la casilla y notifica a la pieza su posición

    Pieza* getPieza() { return piezaOcupante; }  // Getter: puntero a la pieza ocupante
    bool estaOcupada() { return piezaOcupante != nullptr; } // True si hay una pieza en esta casilla
    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano);// Dibuja la casilla (color, decoración, pieza) en la ventana
    int getX() const { return x; }// Getter: coordenada X
    int getY() const { return y; }// Getter: coordenada Y
    ColorCasilla getColorActual()const { return Color; }// Getter: color actual de la casilla
    bool getEsPuntoDePoder() const { return esPuntoDePoder; } // Getter: ¿es punto de poder?
};