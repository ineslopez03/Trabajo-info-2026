#pragma once
#include <SFML/Graphics.hpp>
class Tablero;
class Pieza;
// Resuelve el resultado de los combates y gestiona el movimiento de piezas en el mapa
class GestorCombate {
public:
    void procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal, Tablero* tablero);
    void registrarMuerte(Pieza* p, Tablero* tablero); // guarda la pieza en el cementerio
    void eliminarPiezaDelMapa(Pieza* p, Tablero* tablero);// vacía la casilla del perdedor
    void moverPiezaACasilla(Pieza* p, sf::Vector2i destino, Tablero* tablero);// mueve el ganador
};