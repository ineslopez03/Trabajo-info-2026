#pragma once
#include "InterfazUsuario.h"
#include <SFML/Graphics.hpp>

class MenuPrincipal:public InterfazUsuario{

public:

	MenuPrincipal();
	~MenuPrincipal() override;
	void procesarEntrada(sf::RenderWindow& ventana) override;
	void dibujarPantalla(sf::RenderWindow& ventana) override;


};

