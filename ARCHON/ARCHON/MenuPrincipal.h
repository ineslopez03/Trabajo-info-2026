#pragma once
#include "InterfazUsuario.h"
#include <SFML/Graphics.hpp>
#include "Boton.h"
#include <vector>
#include <string>

enum class OpcionesMenu {//Menús
    PRINCIPAL,
    RANKING,
    SELECCION_MODO,
    SELECCION_SKIN,
    MANUAL_GRAFICO
};

class MenuPrincipal : public InterfazUsuario {
private:
    //variable para gestionar el estado del menu
    OpcionesMenu EstadoInterno;
    //Banderas
    bool IniciarJuego;
    //variables para configurar el sprite
    sf::Texture FondoMenu, FondoHarryPotter, FondoStarWars, FondoArchon;
    sf::Sprite SpriteFondo;
    sf::Text Titulos;
    sf::Font FuenteMenu;
    std::string SkinSeleccionada;

    //vectores de botones para cada menu
    std::vector<Boton> BotonesMenuPrincipal;
    std::vector<Boton> BotonesSeleccionModo;
    std::vector<Boton> BotonesSeleccionSkin;
    std::vector<Boton> BotonesRanking;
    std::vector<Boton> BotonesManual;
	//vectores de texto para el manual
    std::vector<sf::Text> lineasReglas;
    std::vector<sf::Text> lineasHechizos;
    //Métodos privados
	void cargarTextoManual();//metodo para cargar el texto del manual desde un archivo de texto
	void inicializarBotones();//inicializa los botones del menú y les asigna los parámetros

public:
    MenuPrincipal();//constructor 
	~MenuPrincipal() override; //Destructor virtual 

    void procesarEntrada(sf::RenderWindow& ventana) override;//procesa las entradas(clicks) del usuario y actualiza el menu, banderas,fondo,color de botones
	void dibujarPantalla(sf::RenderWindow& ventana) override;//se encarga de pintar el fondo,titulos,botones,manual,segun el estado del menú

    // Getters para la comunicación con MotorArchon
    bool getIniciarJuego() const { return IniciarJuego; }//si se selecciona la skin se empieza el juego
    std::string getSkinSeleccionada() const { return SkinSeleccionada; }//para obtener la skin que se ha seleccionado 
    bool getVerRanking() const { return EstadoInterno == OpcionesMenu::RANKING; }//para saber si se ha seleccionado el ranking
};