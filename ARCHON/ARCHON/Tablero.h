#pragma once
#include "InterfazUsuario.h"
#include "Casilla.h"
#include <SFML/Graphics.hpp>
#include <optional>

class Tablero : public InterfazUsuario {
private:
    Casilla* matriz[9][9];
    sf::View vistaEstatica;
    float tamCasilla;

   
    Bando turnoActual;
    bool primerClicRealizado;
    Casilla* origenSeleccionado;

    
    bool hayCombatePendiente;
    Pieza* atacante;
    Pieza* defensor;
    int turnosContados;
    

public:
    Tablero(); 
    Tablero(float _tamano);
    virtual ~Tablero() override; //

    void inicializarTablero(); //
    void procesarEntrada(sf::RenderWindow& ventanaJuego); //
    void dibujarPantalla(sf::RenderWindow& ventanaJuego); //

    void gestionarTurno(Casilla* casillaClic);
    bool esMovimientoValido(Casilla* origen, Casilla* destino); //
    bool esAtaqueValido(Casilla* origen, Casilla* destino); //

    bool getHaycombate() const { return hayCombatePendiente; }
    Pieza* getAtacante() const { return atacante; }
    Pieza* getDefensor() const { return defensor; }
    void resetCombate(); //
};