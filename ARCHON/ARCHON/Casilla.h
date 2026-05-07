#pragma once
#include "Pieza.h" 
#include<SFML/Graphics.hpp>
enum class ColorCasilla { NEGRO, GRIS_OSCURO, GRIS_CLARO, BLANCO };

class Casilla {
private:
    int x, y;  
    bool esOscilante; 
    bool esPuntoDePoder;
    Pieza* piezaOcupante; 
    ColorCasilla Color;
public:
    Casilla(int _x, int _y); 
    ~Casilla();
    void setPieza(Pieza* p);
    
    Pieza* getPieza() { return piezaOcupante; } 
    bool estaOcupada() { return piezaOcupante != nullptr; }
    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano);
	int getX() const { return x; }
	int getY() const { return y; }
	int getColor() const { return static_cast<int>(Color); }
   bool getEsPuntoDePoder() const { return esPuntoDePoder; } 
   bool getEsOscilante() const { return esOscilante; } 
};