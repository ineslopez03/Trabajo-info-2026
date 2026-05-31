#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "InterfazUsuario.h"
#include "Pieza.h"

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
    bool ejecutando;//flag para salir del bucle del juego
    std::string skinActual;  // Guarda la skin seleccionada de menu para usarla en el tablero y la arena

public:
    MotorArchon();//constructor
    ~MotorArchon();//destructor

    void inicializar();//Inicializaciones
    void bucle();//bucle del juego

    void cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1 = nullptr, Pieza* p2 = nullptr, std::string skinSeleccionada = " ");
    //Metodo para cambiar de estado.
};