#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "InterfazUsuario.h"
#include "GestorArchivos.h"
#include "Pieza.h"
#include "Jugador.h"
#include "JugadorHumano.h"


class Tablero;
enum class EstadoJuego { MENU, TABLERO, ARENA, FIN,INGRESAR_NOMBRE,RANKING };
class PantallaNombre;
class PantallaRanking;
class MotorArchon
{
private:
    sf::RenderWindow ventana;
    sf::Music MusicaFondo;
    EstadoJuego estadoActual;
    InterfazUsuario* pantallaActiva;
    GestorArchivos guardado;
    Tablero* miTablero;
    PantallaNombre* miPantallaNombre = nullptr;
    PantallaRanking* miPantallaRanking = nullptr;

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

