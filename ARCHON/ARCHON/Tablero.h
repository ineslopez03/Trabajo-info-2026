#pragma once
#include"InterfazUsuario.h"
#include"Casilla.h"
#include<SFML/Graphics.hpp>

class Tablero :public InterfazUsuario {
private:
	Casilla* matriz[9][9];
	Casilla* origenSeleccionado;
	bool primerClicRealizado;
	sf::RenderWindow& ventanaJuego;
	sf::Clock relojOscilacion;
	float anguloRotacion = 0.0f;
	bool estaenRango(Casilla* origen, Casilla* destino);
	bool hayCombatePendiente = false;
	Pieza* atacante = nullptr;
	Pieza* defensor = nullptr;
	float tiempoAcumuladoOscilacion = 0.0f;
	int turnosContados = 0;
	const float Duracion_fase = 2.0f;
public:
	Tablero(sf::RenderWindow& ventana);
	~Tablero() override;
	void procesarEntrada() override;
	void dibujarPantalla() override;
	void inicializarTablero();
	void moverPieza(Casilla* origen, Casilla* destino);
	bool esMovimientoValido(Casilla* origen, Casilla* destino);
	bool esAtaqueValido(Casilla* origen, Casilla* destino);
	bool comprobarVictoria();
	bool estaEnRango(Casilla* origen, Casilla* destino);
	void gestionarTurno(Casilla* origen, Casilla* destino);
	bool getHaycombate() const { return hayCombatePendiente; }
	void resetCombate() { hayCombatePendiente = false; atacante = nullptr; defensor = nullptr; }
	Pieza* getAtacante() const { return atacante; }
	Pieza* getDefensor() const { return defensor; }
	void rotarTablero();

};