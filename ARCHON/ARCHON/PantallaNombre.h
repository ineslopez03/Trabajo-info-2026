#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Constantes.h"
#include "InterfazUsuario.h"

class MotorArchon;

class PantallaNombre :public InterfazUsuario {
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

    void guardarEnRanking();//metodo para guardar el nombre ingresado en el ranking, se llama al finalizar la pantalla de ingreso de nombre
public:
    PantallaNombre(Bando ganador);//constructor que recibe el bando ganador para configurar el mensaje de victoria
    void procesarEntrada(sf::RenderWindow& ventana, MotorArchon* motor) override;//metodo para procesar las entradas del usuario
    void dibujarPantalla(sf::RenderWindow& ventana) override;//metodo para dibujar la pantalla de ingreso de nombre
};