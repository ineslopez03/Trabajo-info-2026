#pragma once
#include "InterfazUsuario.h"
#include "Jugador.h"
#include "GestorArchivos.h"

#include <SFML/Graphics.hpp>


enum class EstadoJuego { MENU, TABLERO, ARENA, FIN };//Se hace enum class para que no se puedan comparar con enteros, y para que no haya confusión con otros estados de otras partes del programa

class MotorArchon
{
private:
	sf::RenderWindow ventana; //Creo la ventana que voy a ir pasando 

	EstadoJuego estadoActual; //Variable para almacenar/conocer el estado actual del juego
	InterfazUsuario* pantallaActiva; //puntero general para la pantalla activa, que se asignará a cada pantalla concreta según el estado del juego
	GestorArchivos guardado;//objeto que guardará los datos del juego.
	Jugador* jugador1;
	Jugador* jugador2;
	//punteros que apuntan a los dos jugadores sean reales o IA.

	bool ejecutando; //EN caso de estar jugando vale 1, si terminamos/salimos vale 0.
public:
	MotorArchon();//constructor generico 
	~MotorArchon(); //destructor generico 

	void inicializar();//inicializamos todo lo necesario para el juego.
	void bucle();// es el bucle principal del juego, se procesan entradas, se dibuja, etc.
	void cambiarEstado(EstadoJuego nuevoEstado);//si se da la condición de cambiar de estado llamamos a esta funcion y gestiona el cambio.