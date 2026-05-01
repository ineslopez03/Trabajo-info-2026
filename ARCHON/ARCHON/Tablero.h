#pragma once
#include"InterfazUsuario.h"
#include"Casilla.h"
#include<SFML/Graphics.hpp>

class Tablero :public InterfazUsuario {
private:
	Casilla* matriz[9][9];
	Casilla* origenSeleccionado;
	bool primerClicRealiazado;
	sf::RenderWindow& ventanaJuego;
public:
	Tablero(sf::RenderWindow& ventana);
	~Tablero() override;
	void procesarEntrada() override;
	void dibujarPantalla() override;
	void ininicializarTablero();
	void moverPieza(Casilla* origen, Casilla* destino);
	bool comprobarMovimientoValido(Casilla* origen, Casilla* destino);
	bool comprobarAtaqueValido(Casilla* origen, Casilla* destino);
	bool comprobarVictoria();
};