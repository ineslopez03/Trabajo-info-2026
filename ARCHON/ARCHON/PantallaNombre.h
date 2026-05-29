#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constantes.h"
#include "InterfazUsuario.h"

class PantallaNombre:public InterfazUsuario {
private:
	sf::Font fuente;//para configurar los textos
	sf::Texture Fondo;//para el fondo de la pantalla de ingreso de nombre
	sf::Sprite SpriteFondo;//sprite para mostrar el fondo
    sf::Text textoTitulo;      
    sf::Text textoInstrucciones;
    sf::Text textoNombre;       
    sf::Text textoBotonEnter;
    std::string nombreIngresado;
	Bando bandoGanador;//para saber que mensaje mostrar segun el bando ganador
	bool terminado;//flag para indicar que se ha terminado de ingresar el nombre y se puede transitar al menu
    void guardarEnRanking();//metodo para guardar el nombre ingresado en el ranking, se llama al finalizar la pantalla de ingreso de nombre
public:
	PantallaNombre(Bando ganador);//constructor que recibe el bando ganador para configurar el mensaje de victoria
    void procesarEntrada(sf::RenderWindow& ventana) override;//metodo para procesar las entradas del usuario
	void dibujarPantalla(sf::RenderWindow& ventana) override;//metodo para dibujar la pantalla de ingreso de nombre
	bool esTransicionLista() const { return terminado; }//metodo para saber si se ha terminado de ingresar el nombre y se puede transitar al menu
};
