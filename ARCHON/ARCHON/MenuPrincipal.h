#pragma once
#include "InterfazUsuario.h"
#include <SFML/Graphics.hpp>
#include "Boton.h"
#include <vector>
enum class OpcionesMenu{PRINCIPAL,RANKING,SELECCION_MODO,IA_NODISPONIBLE,SELECCION_SKIN,};
//el menu principal mostrará: iniciar partida,cargar partida//no si muy complejo,ranking,instrucciones,salir.
//el menu de inciar partida pasará a mostrar: Jugador vs Jugador o Jugador vs IA.
//si se elige jugador vs IA saldrá un mensaje diciendo que no esta disponible y la opcion de volver.
//si se elige Jugador vs Jugador se mostrarán las tematicas con las que puede iniciar la partida
//en principio Archon normal, Harry Potter y Star Wars, luego si eso metemos mas
//si se elige instrucciones se abrirá un archivo que explique como jugar al juego.


class MenuPrincipal:public InterfazUsuario{
private:
	std::vector<Boton> botonesMenuPrincipal;//listas de botones para tener todo organizado.
	std::vector<Boton> botonesSeleccionModo;
	std::vector<Boton> botonesSeleccionSkin;
	std::vector<Boton> botonesRanking;
	std::vector<Boton> botonVolverIA;
	//meto banderas para leerlas en motorArchon
	bool iniciarJuego = false;
	bool contraIA = false;
	std::string skinSeleccionada;
	void inicializarBotones();//no hace falta que sea publico.

public:

	MenuPrincipal();
	~MenuPrincipal() override;
	void procesarEntrada(sf::RenderWindow& ventana) override;
	void dibujarPantalla(sf::RenderWindow& ventana) override;


};

