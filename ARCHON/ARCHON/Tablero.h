#pragma once
#include "InterfazUsuario.h"
#include "Casilla.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

class Tablero : public InterfazUsuario {
private:
    std::string skinActual;
    Casilla* matriz[9][9];
    sf::View vistaEstatica;
    float tamCasilla;

    Bando turnoActual;
    bool primerClicRealizado;
    Casilla* origenSeleccionado;
    Casilla* piezaAuxiliar = nullptr;

    bool hayCombatePendiente;
    Pieza* atacante;
    Pieza* defensor;
    int turnosContados;

    bool modoHechizoActivo = false;
    int hechizoSeleccionado = 0;
    bool hechizosLuzUsados[8] = { false };
    bool hechizosOscurosUsados[8] = { false };

public:
    Tablero();
    Tablero(float tam, std::string skin);
    virtual ~Tablero() override;

    void inicializarTablero();
    void procesarEntrada(sf::RenderWindow& ventanaJuego);
    void dibujarPantalla(sf::RenderWindow& ventanaJuego);

    void gestionarTurno(Casilla* casillaClic);
    bool esMovimientoValido(Casilla* origen, Casilla* destino);
    bool esAtaqueValido(Casilla* origen, Casilla* destino);

    bool getHaycombate() const { return hayCombatePendiente; }
    Pieza* getAtacante() const { return atacante; }
    Pieza* getDefensor() const { return defensor; }
    void resetCombate();

    void procesarMagia(Casilla* objetivo);
    void finalizarTurno();

    std::string getSkin() const { return skinActual; } // accesor para skin
};
