#pragma once 
#include <SFML/Graphics.hpp> 

class Tablero; //declaracion adelantada para evitar bucles de dependencias
class Pieza; //declaracion adelantada

//resuelve el resultado de los combates y gestiona el movimiento de piezas en el mapa
class GestorCombate {
public:
    //que pasa en el tablero cuando alguien gana una pelea en la arena
    void procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal, Tablero* tablero);

    //guarda la pieza muerta en el cementerio por si usan el hechizo revive mas tarde
    void registrarMuerte(Pieza* p, Tablero* tablero);

    //busca donde estaba parado el perdedor y vacia esa casilla
    void eliminarPiezaDelMapa(Pieza* p, Tablero* tablero);

    //mueve al ganador a la casilla donde se lio la pelea (si es que era el atacante)
    void moverPiezaACasilla(Pieza* p, sf::Vector2i destino, Tablero* tablero);
};