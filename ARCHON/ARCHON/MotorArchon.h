#pragma once
#include <SFML/Graphics.hpp>
#include "InterfazUsuario.h"
#include "GestorArchivos.h"
#include "Pieza.h"
#include "Jugador.h"
#include "JugadorHumano.h"
#include "JugadorIA.h"

// Se hace enum class para que no se puedan comparar con enteros, y para que no haya confusión con otros estados de otras partes del programa
enum class EstadoJuego { MENU, TABLERO, ARENA, FIN };

class MotorArchon
{
private:
	sf::RenderWindow ventana; // Creo la ventana que voy a ir pasando 

	EstadoJuego estadoActual; // Variable para almacenar/conocer el estado actual del juego
	InterfazUsuario* pantallaActiva; // Puntero general para la pantalla activa, que se asignará a cada pantalla concreta según el estado del juego
	GestorArchivos guardado; // Objeto que guardará los datos del juego.

	// Composición con la clase base abstracta Jugador según el esquema de clases
	Jugador* jugador1; // Punteros que apuntan a los dos jugadores sean reales o IA.
	Jugador* jugador2;

	bool ejecutando; // En caso de estar jugando vale 1, si terminamos/salimos vale 0.

public:
	MotorArchon(); // Constructor genérico 
	~MotorArchon(); // Destructor genérico 

	void inicializar(); // Inicializamos todo lo necesario para el juego.
	void bucle(); // Es el bucle principal del juego, se procesan entradas, se dibuja, etc.

	// Si se da la condición de cambiar de estado llamamos a esta función y gestiona el cambio 
	// Permite opcionalmente pasar punteros a piezas para las transiciones a la Arena de combate
	void cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1 = nullptr, Pieza* p2 = nullptr);
};
