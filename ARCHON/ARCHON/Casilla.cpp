#include "Casilla.h"
#include<iostream>
#include <SFML/Graphics.hpp>

Casilla::Casilla(int _x, int _y, bool _osc)
    : x(_x), y(_y), esOscilante(_osc), piezaOcupante(nullptr) {
    ColorOscilante = ColorCasilla::NEGRO;
}//constructor de la casilla, la pieza ocupante esta vacia por defecto
Casilla::~Casilla() {}
void Casilla::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    sf::RectangleShape cuadrado(sf::Vector2f(tamano, tamano));//se pinta el cuadrado de tamaño tamano
	float posX = x * tamano; //se le asigna la posicion en x multiplicando la coordenada x por el tamaño de la casilla
	float posY = y * tamano;//se le asigna la posicion en y multiplicando la coordenada y por el tamaño de la casilla
    cuadrado.setPosition({ posX, posY });// se le asigna finalmente la posicion al cuadrado
    
    bool esPuntoPoder = ((x == 4 && y == 4)|| (x == 0 && y == 4) || (x == 4 && y == 0) || (x == 4 && y == 8) || (x == 8 && y == 4));
    int fase = turno%4;
    switch (fase) {//se debe asegurar que turno empieze en 0 y se reinicie cuando llegue a 3
    case 0: 
        ColorOscilante = ColorCasilla::NEGRO;
        break;
    case 1:
        ColorOscilante = ColorCasilla::GRIS_OSCURO;
        break;
    case 2:
        ColorOscilante = ColorCasilla::GRIS_CLARO;
        break;
    case 3:
        ColorOscilante = ColorCasilla::BLANCO;
        break;
    }
   
   sf::Color colorRelleno;

   if (esOscilante) {
       switch (ColorOscilante)
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
       colorRelleno = ((x + y) % 2 == 0) ? sf::Color::White : sf::Color::Black;   
   }
  
    cuadrado.setFillColor(colorRelleno);
    ventana.draw(cuadrado);

    if (esPuntoPoder) {
        float proporcion = 0.55;//proporcion para reducir el tamaño del cuadrado del punto de poder
        sf::RectangleShape cuadrado(sf::Vector2f(tamano * proporcion, tamano * proporcion));//Lo pinto mas pequeñ
       float posX = x * tamano+((tamano-tamano*proporcion)/2); // se le desplaza a la derecha LA MITAD del espacio que queda entre el cuadrado del fondo y el dorado
       float posY = y * tamano + ((tamano - tamano * proporcion) / 2);//lo mismo
       cuadrado.setPosition({ posX, posY });// se le asigna finalmente la posicion al cuadrado
	   colorRelleno = sf::Color(218, 165, 32);//color dorado
		cuadrado.setFillColor(colorRelleno);// se le asigna el color dorado
        ventana.draw(cuadrado);// le digo que me lo pinte(se lo digo después de que pinte los demás cuadrados para
        //que quede superpuesto 
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
    if (piezaOcupante) {
        piezaOcupante->dibujar(ventana, posX, posY, tamano);
    }
}