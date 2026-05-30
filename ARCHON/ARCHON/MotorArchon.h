#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "InterfazUsuario.h"
#include "Pieza.h"
#include "Jugador.h"
#include "JugadorHumano.h"

class Tablero;

enum class EstadoJuego { MENU, TABLERO, ARENA, FIN, INGRESAR_NOMBRE, RANKING };

class PantallaNombre;
class PantallaRanking;

class MotorArchon
{
private:
    sf::RenderWindow ventana;//Ventana del juego
    sf::Music MusicaFondo;//MUSICA
    EstadoJuego estadoActual;//variable de estado del juego para gestionar las pantallas
    InterfazUsuario* pantallaActiva;//puntero a la pantalla activa, para llamar a sus metodos de dibujo y procesar entrada
    Tablero* miTablero;//se crea un puntero a tablero para mantenerlo vivo durante el juego y no perderlo al cambiar de estado
    Jugador* jugador1;//se declara a los jugadores como punteros(porque estaba pensado que el jugador fuera humano o IA)
    Jugador* jugador2;//upcasting
    bool ejecutando;//flag para salir del bucle del juego
    std::string skinActual;  // Guarda la skin seleccionada de menu para usarla en el tablero y la arena
    sf::Vector2i casillaDestinoCombate;//

    // Subsistema de transiciones diferidas para blindaje de RAM
    bool hayCambioPendiente;
    EstadoJuego estadoPendiente;
    Pieza* p1Pendiente;
    Pieza* p2Pendiente;
    std::string skinPendiente;

    void aplicarCambioEstado();

public:
    MotorArchon();//constructor
    ~MotorArchon();//destructor

    void inicializar();//Inicializaciones
    void bucle();//bucle del juego

    // Metodo para encolar el cambio de estado de forma segura.
    void encolarCambioEstado(EstadoJuego nuevoEstado, Pieza* p1 = nullptr, Pieza* p2 = nullptr, std::string skinSeleccionada = "");

    Tablero* getTablero() const { return miTablero; }
    void purgarTablero();
};

