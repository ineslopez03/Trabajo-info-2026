#pragma once
#include "Pieza.h" 
#include<SFML/Graphics.hpp>
enum class ColorCasilla { NEGRO, GRIS_OSCURO, GRIS_CLARO, BLANCO };// posibles color de las casillas.

class Casilla {
private:
    int x, y; //coordenadas de la matriz del tablero 
    bool esOscilante; // con esto definimos si queremos que la casilla cambie de color o no 
    Pieza* piezaOcupante; // puntero a la pieza que va a ocupar la casilla (puede ser nullptr si vacia)
    ColorCasilla ColorOscilante;
public:
    Casilla(int _x, int _y); //constructor de la casilla
    ~Casilla(); //destructor de la casilla 
    void setPieza(Pieza* p) { // metodo set, pasamos un puntero de la pieza que la va a ocupar.
        //Asigna el puntero al atributo privado
        piezaOcupante = p;
        // Si la pieza no es nula, actualizamos también la posición de la pieza
        if (p != nullptr) p->setPosicion(this);//si el puntero no esta vacío le pasamos a la pieza las coordenadas de esta casilla
    }
    Pieza* getPieza() { return piezaOcupante; } // metodo get para saber que pieza exactamente ocupa esta casilla
    bool estaOcupada() { return piezaOcupante != nullptr; }// si alguna pieza ocupa la casilla devuelve 1
    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano);// se dibuja la casilla
	int getX() const { return x; }//me devuelve la coordenada x
	int getY() const { return y; }//me devuelve la coordenada y
    
};