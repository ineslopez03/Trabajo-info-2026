#pragma once
#include "InterfazUsuario.h"
#include <SFML/Graphics.hpp>
enum class OpcionesMenu{PRINCIPAL,SELECCION_MODO,SELECCION_SKIN};

class MenuPrincipal:public InterfazUsuario{

	struct boton{
		sf::RectangleShape forma;
		sf::Text texto;
	};
public:

	MenuPrincipal();
	~MenuPrincipal() override;
	void procesarEntrada(sf::RenderWindow& ventana) override;
	void dibujarPantalla(sf::RenderWindow& ventana) override;


};

