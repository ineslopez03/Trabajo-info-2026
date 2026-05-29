#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constantes.h"
#include "InterfazUsuario.h"

class PantallaNombre:public InterfazUsuario {
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
    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;
    bool esTransicionLista() const { return terminado; }
};
