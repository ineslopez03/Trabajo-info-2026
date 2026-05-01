#include "Casilla.h"
#include<iostream>
#include <SFML/Graphics.hpp>
Casilla::Casilla(int _x, int _y, bool _osc)
	: x(_x), y(_y), esOscilante(_osc), piezaOcupante(nullptr) {}
Casilla::~Casilla() {}
void Casilla::dibujar(sf::RenderWindow& ventana)
{
	float tamano = 50.0f;
	sf::RectangleShape cuadrado(sf::Vector2f(tamano, tamano));
	cuadrado.setPosition(sf::Vector2f(x * tamano, y * tamano));
	if((x+y) % 2 == 0)
		cuadrado.setFillColor(sf::Color(200, 200, 200));
	else
		cuadrado.setFillColor(sf::Color(50, 50, 50));

if (esOscilante) {
	cuadrado.setOutlineThickness(2);
	cuadrado.setOutlineColor(sf::Color::Yellow);
}
ventana.draw(cuadrado);
if (piezaOcupante != nullptr) {
	piezaOcupante->dibujar(ventana); 
}