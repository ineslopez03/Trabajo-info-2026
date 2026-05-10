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
	sf::Font FuenteMenu;//fuente para los botones, se carga en el constructor
	OpcionesMenu EstadoInterno;//para saber en que menu estoy y mostrar los botones correspondientes

	sf::Texture FondoMenu;//mi textura
	sf::Sprite SpriteFondo;//el que va a dibujar el fondo

	std::vector<Boton> BotonesMenuPrincipal;//listas de botones para tener todo organizado.
	std::vector<Boton> BotonesSeleccionModo;
	std::vector<Boton> BotonesSeleccionSkin;
	std::vector<Boton> BotonesRanking;
	std::vector<Boton> BotonVolverIA;
	//meto banderas para leerlas en motorArchon
	bool IniciarJuego;
	bool ContraIA;
	std::string SkinSeleccionada;
	void inicializarBotones();//no hace falta que sea publico.

public:

	MenuPrincipal();
	~MenuPrincipal() override;
	void procesarEntrada(sf::RenderWindow& ventana) override;
	void dibujarPantalla(sf::RenderWindow& ventana) override;
	//getters para usar en motorArchon
	bool getIniciarJuego() const { return IniciarJuego; };
	bool getContraIA() const { return ContraIA; };
	std::string getSkinSeleccionada() const { return SkinSeleccionada; };
};

