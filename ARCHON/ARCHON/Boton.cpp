#include "Boton.h"
Boton::Boton(float x, float y, float ancho, float alto, std::string textoBoton, sf::Font& fuente)
{
	//idmensiones del boton
	fondo.setPosition({ x, y });//ubicacion del boton
	fondo.setSize(sf::Vector2f(ancho, alto));//tamaño del boton
	//coloracion del boton
	colorNormal = sf::Color(100, 100, 100);//color normal del boton
	colorEncima = sf::Color(150, 150, 150);//color del boton al pasar por encima
	fondo.setFillColor(colorNormal);//se pone el color 
	fondo.setOutlineThickness(2.0f);//grosor del borde, luego lo cambio si eso
	fondo.setOutlineColor(sf::Color::White);//de color blanco 
	//configuro el texto del boton
	texto.setFont(fuente);//selecciono la fuente
	texto.setString(textoBoton);//seteamos el texto 
	texto.setCharacterSize(24);//tamaño, se ajusta si hace falta
	texto.setFillColor(sf::Color::White);//por ahora blanco 

	//lo centro en el boton
	float PosXTexto = x + (ancho - texto.getLocalBounds().size.x) / 2.0f;
	//miro el ancho del texto y lo resto al ancho del boton, luego lo divido entre 2 para centrarlo
	float PosYTexto = y + (alto - texto.getLocalBounds().size.x) / 2.0f;//ajustar el texto en y si hace falta
	//lo mismo pero con el alto
	texto.setPosition({ PosXTexto ,PosYTexto });
}