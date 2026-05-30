#pragma once
#include <SFML/Graphics.hpp>
class Tablero;

class GestorVictoria {
public:
    int verificarVictoria(Tablero* tablero);
    void actualizarFaseVictoria(Tablero* tablero, float dt);
    void dibujarFaseVictoria(Tablero* tablero, sf::RenderWindow& ventana);
};