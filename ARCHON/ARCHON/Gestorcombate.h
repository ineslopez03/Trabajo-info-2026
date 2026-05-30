#pragma once
#include <SFML/Graphics.hpp>
class Tablero;
class Pieza;

class GestorCombate {
public:
    void procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal, Tablero* tablero);
    void registrarMuerte(Pieza* p, Tablero* tablero);
    void eliminarPiezaDelMapa(Pieza* p, Tablero* tablero);
    void moverPiezaACasilla(Pieza* p, sf::Vector2i destino, Tablero* tablero);
};