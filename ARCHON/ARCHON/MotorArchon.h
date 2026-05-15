#pragma once
#include <SFML/Graphics.hpp>
#include "InterfazUsuario.h"
#include "GestorArchivos.h"
#include "Pieza.h"
#include "Jugador.h"
#include "JugadorHumano.h"
#include "JugadorIA.h"

class Tablero;
enum class EstadoJuego { MENU, TABLERO, ARENA, FIN };

class MotorArchon
{
private:
    sf::RenderWindow ventana;

    EstadoJuego estadoActual;
    InterfazUsuario* pantallaActiva;
    GestorArchivos guardado;
    Tablero* miTablero;

    Jugador* jugador1;
    Jugador* jugador2;

    bool ejecutando;

    std::string skinActual;  // Guardar skin seleccionada
    sf::Vector2i casillaDestinoCombate;
public:
    MotorArchon();
    ~MotorArchon();

    void inicializar();
    void bucle();

    void cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1 = nullptr, Pieza* p2 = nullptr, std::string skinSeleccionada = " ");
};

