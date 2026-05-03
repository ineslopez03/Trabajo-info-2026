#pragma once
#include <SFML/Graphics.hpp>
class InterfazUsuario
{
public:
	virtual  ~InterfazUsuario() {} //Hacemos un destructor virtual para que borre todo del hijo

	virtual void procesarEntrada(sf::RenderWindow& ventana) = 0; //cada clase hija deberá implementar esta clase a su manera

	virtual void dibujarPantalla(sf::RenderWindow& ventana) = 0; //cada clase hija deberá implementar esta clase a su manera
};

