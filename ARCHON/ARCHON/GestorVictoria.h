#pragma once
#include <SFML/Graphics.hpp>
class Tablero;
// Gestiona las tres condiciones de victoria y la animación de fin de partida
class GestorVictoria {
public:
    int verificarVictoria(Tablero* tablero);// comprueba si hay ganador
    void actualizarFaseVictoria(Tablero* tablero, float dt);// avanza la cinemática
    void dibujarFaseVictoria(Tablero* tablero, sf::RenderWindow& ventana);// dibuja la pantalla final
};