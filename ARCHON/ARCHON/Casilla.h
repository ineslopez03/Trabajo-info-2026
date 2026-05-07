#pragma once
#include "Pieza.h" 
#include<SFML/Graphics.hpp>
enum class ColorCasilla { NEGRO, GRIS_OSCURO, GRIS_CLARO, BLANCO };// posibles color de las casillas.

class Casilla {
private:
    int x, y; //coordenadas de la matriz del tablero 
    bool esOscilante; // con esto definimos si queremos que la casilla cambie de color o no 
    bool esPuntoDePoder;//con esto definimos si es punto de poder
    Pieza* piezaOcupante; // puntero a la pieza que va a ocupar la casilla (puede ser nullptr si vacia)
    ColorCasilla Color;//con esto guardamos el color de la casilla
public:
    Casilla(int _x, int _y); //constructor de la casilla
    ~Casilla(); //destructor de la casilla 
    void setPieza(Pieza* p) { // metodo set, pasamos un puntero de la pieza que la va a ocupar.
        //Asigna el puntero al atributo privado
        piezaOcupante = p;
    }
    Pieza* getPieza() { return piezaOcupante; } // metodo get para saber que pieza exactamente ocupa esta casilla
    bool estaOcupada() { return piezaOcupante != nullptr; }// si alguna pieza ocupa la casilla devuelve 1
    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano);// se dibuja la casilla
	int getX() const { return x; }//me devuelve la coordenada x
	int getY() const { return y; }//me devuelve la coordenada y
	int getColor() const { return static_cast<int>(Color); }//me devuelve el color de la casilla en formato int para poder usarlo en el tablero
   bool getEsPuntoDePoder() const { return esPuntoDePoder; } // me devuelve si es punto de poder o no
   bool getEsOscilante() const { return esOscilante; } // me devuelve si es oscilante o no
};