#include "Casilla.h"
#include<iostream>
#include <SFML/Graphics.hpp>

Casilla::Casilla(int _x, int _y)
    : x(_x), y(_y), piezaOcupante(nullptr) {
    bool esPuntoPoder = ((x == 4 && y == 4) || (x == 0 && y == 4) || (x == 4 && y == 0) || (x == 4 && y == 8) || (x == 8 && y == 4));
	
    if (esPuntoPoder) esPuntoDePoder = true; else esPuntoDePoder = false;

    bool lasOscilantes = (((x == 4 && y <= 8) || (y == 4 && (x > 0 && x < 8)))//aquí defino la cruz
        || x == 3 && y == 0 || x == 2 && y == 1 || x == 1 && y == 2 || x == 0 && y == 3
        || x == 5 && y == 0 || x == 6 && y == 1 || x == 7 && y == 2 || x == 8 && y == 3//parte supuerior
        || y == 5 && x == 0 || y == 6 && x == 1 || y == 7 && x == 2 || y == 8 && x == 3
        || y == 8 && x == 5 || y == 7 && x == 6 || y == 6 && x == 7 || y == 5 && x == 8);

    if (lasOscilantes) esOscilante = true; else esOscilante = false;

    Color = ColorCasilla::NEGRO;
}


Casilla::~Casilla() {

}


void Casilla::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    sf::RectangleShape cuadrado(sf::Vector2f(tamano, tamano));
	float posX = x * tamano; 
	float posY = y * tamano;
    cuadrado.setPosition({ posX, posY });
    
   sf::Color colorRelleno; 

 
   if (esOscilante) {
       int fase = turno % 4;
       switch (fase) {
       case 0:Color = ColorCasilla::NEGRO;break;
       case 1:Color = ColorCasilla::GRIS_OSCURO;break;
       case 2:Color = ColorCasilla::GRIS_CLARO;break;
       case 3:Color = ColorCasilla::BLANCO;break;
       }

       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
           Color = ColorCasilla::GRIS_CLARO;
       }

       switch (Color)
       {
       case ColorCasilla::NEGRO:
       colorRelleno = sf::Color::Black; 
       break;
       case ColorCasilla::GRIS_OSCURO:
        colorRelleno = sf::Color(45, 45, 45);
           
        break;
       case ColorCasilla::GRIS_CLARO:
       colorRelleno = sf::Color(140, 140, 140); 
       
       break;  
       case ColorCasilla::BLANCO:
       colorRelleno = sf::Color::White; 

       break;
       }

       
   }
   else {
       if (x < 4) {
           colorRelleno = ((x + y) % 2 == 0) ? sf::Color::White : sf::Color::Black;
           
       }
       else {
           colorRelleno = ((x + y) % 2 == 1) ? sf::Color::White : sf::Color::Black;
       }
       if (colorRelleno == sf::Color::White) {
           Color = ColorCasilla::BLANCO;
       }
       else { Color = ColorCasilla::NEGRO; }
   }
  
    cuadrado.setFillColor(colorRelleno);
    ventana.draw(cuadrado);

    if (esPuntoDePoder) {
        
	   colorRelleno = sf::Color(218, 165, 32);
		cuadrado.setFillColor(colorRelleno);
        ventana.draw(cuadrado);
    }

    sf::Color colorBorde = (colorRelleno == sf::Color::Black) ? sf::Color(60, 60, 60) : sf::Color(100, 100, 100, 150);

    sf::RectangleShape borde(sf::Vector2f(tamano, tamano));
    borde.setPosition(sf::Vector2f(posX, posY));
    borde.setFillColor(sf::Color::Transparent);
    borde.setOutlineThickness(-1.0f);
    borde.setOutlineColor(colorBorde);
    ventana.draw(borde);

    sf::RectangleShape luzSuperior(sf::Vector2f(tamano, 2.0f));
    luzSuperior.setPosition(sf::Vector2f(posX, posY));
    luzSuperior.setFillColor(sf::Color(255, 255, 255, 40)); 
    ventana.draw(luzSuperior);

    // --- SELECCIÓN ---
    if (this == seleccionada) {
        sf::RectangleShape highlight(sf::Vector2f(tamano - 4, tamano - 4));
        highlight.setPosition(sf::Vector2f(posX, posY));
        highlight.setOutlineThickness(2.0f);
        highlight.setOutlineColor(sf::Color::Green);
        highlight.setFillColor(sf::Color::Transparent);
        ventana.draw(highlight);
    }
    if (piezaOcupante != nullptr) {
        piezaOcupante->dibujar(ventana, seleccionada, turno, tamano);
    }
    }

void Casilla::setPieza(Pieza* p) {
    this->piezaOcupante = p;
    if (p != nullptr) {
        p->setPosicion(this); 
    }
}