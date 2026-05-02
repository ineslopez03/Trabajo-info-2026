#pragma once
#include "Pieza.h" 
#include<SFML/Graphics.hpp>
class Casilla {
private:
    int x, y;
    bool esOscilante;
    Pieza* piezaOcupante; 
    enum class ColorCasilla{NEGRO,GRIS_OSCURO,GRIS_CLARO,BLANCO};
public:
    Casilla(int _x, int _y, bool _osc);
    ~Casilla();
    void setPieza(Pieza* p) {
        piezaOcupante = p;
        // Si la pieza no es nula, actualizamos también la posición de la pieza
        if (p != nullptr) p->setPosicion(this);
    }

    Pieza* getPieza() { return piezaOcupante; }
    bool estaOcupada() { return piezaOcupante != nullptr; }
    void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, float tiempo, float tamano);
	int getX() const { return x; }
	int getY() const { return y; }
    
};