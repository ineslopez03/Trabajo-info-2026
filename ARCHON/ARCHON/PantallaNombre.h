#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constantes.h"
class PantallaNombre {
private:
    sf::Font fuente;
    sf::Text textoTitulo;      
    sf::Text textoInstrucciones;
    sf::Text textoNombre;       
    sf::Text textoBotonEnter;
    std::string nombreIngresado;
    Bando bandoGanador;
    bool terminado;
    void guardarEnRanking();
public:
    PantallaNombre(Bando ganador);
    void procesarEntrada(sf::RenderWindow& ventana);
    void dibujar(sf::RenderWindow& ventana);
    bool esTransicionLista() const { return terminado; }
};
