#pragma once
#include "Pieza.h" 
#include<SFML/Graphics.hpp>
#include"Constantes.h"
class Casilla {
private:
    int x, y;  //Coordenadas en el tablero(0-8)
    bool esOscilante=false; //Si cambia de color cada turno
    bool esPuntoDePoder=false;//Si es uno de los 5 nodos especiales
    Pieza* piezaOcupante; //Puntero a la pieza
    ColorCasilla Color;//Color actual de la casilla
public:
    Casilla(int _x, int _y); 
    ~Casilla();
    void setPieza(Pieza* p);
    
    Pieza* getPieza() { return piezaOcupante; } 
    bool estaOcupada() { return piezaOcupante != nullptr; }
    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano);
	int getX() const { return x; }
	int getY() const { return y; }
    ColorCasilla getColorActual()const { return Color; }
   bool getEsPuntoDePoder() const { return esPuntoDePoder; } 
   bool getEsOscilante() const { return esOscilante; } 
};