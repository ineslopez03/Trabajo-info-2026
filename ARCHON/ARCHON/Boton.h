#pragma once
#include <SFML/Graphics.hpp>
class Boton
{
private:
	sf::RectangleShape fondo;
	sf::Text texto;
	sf::Color colorNormal;
	sf::Color colorEncima;
public:
	Boton(float x,float y,float ancho, float alto, std::string texto,sf:: Font& fuente );
	//simplemente le digo la forma del recuadro, el texto a contener y la fuente a usar

	void dibujar(sf::RenderWindow& ventana);
	void raton(sf::Vector2f posRaton);
	void setTexto( sf::Text nuevoTexto );
};

