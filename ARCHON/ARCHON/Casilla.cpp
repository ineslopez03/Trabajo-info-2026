#include "Casilla.h"
#include<iostream>
#include <SFML/Graphics.hpp>

Casilla::Casilla(int _x, int _y)
    : x(_x), y(_y), piezaOcupante(nullptr) {//Constructor: guarda las coordenadas y deja la casilla vacía
    esPuntoDePoder = ((x == 4 && y == 4) || (x == 0 && y == 4) || (x == 4 && y == 0) || (x == 4 && y == 8) || (x == 8 && y == 4));
    //Los 5 puntos de poder son el centro y los 4 extremos de la cruz central.

     esOscilante = (((x == 4 && y <= 8) || (y == 4 && (x > 0 && x < 8)))
        || x == 3 && y == 0 || x == 2 && y == 1 || x == 1 && y == 2 || x == 0 && y == 3
        || x == 5 && y == 0 || x == 6 && y == 1 || x == 7 && y == 2 || x == 8 && y == 3
        || y == 5 && x == 0 || y == 6 && x == 1 || y == 7 && x == 2 || y == 8 && x == 3
        || y == 8 && x == 5 || y == 7 && x == 6 || y == 6 && x == 7 || y == 5 && x == 8);
    
    Color = ColorCasilla::NEGRO;//Color por defecto al crear la casilla.
}
Casilla::~Casilla() {}

void Casilla::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    sf::RectangleShape cuadrado(sf::Vector2f(tamano, tamano));
	float posX = x * tamano; 
	float posY = y * tamano;
    cuadrado.setPosition({ posX, posY });
    //Crea el cuadrado de la casilla y lo coloca en pantalla según sus coordenadas multiplicadas
    //por el tamaño de la celda.

   sf::Color colorRelleno; 

 
   if (esOscilante) {
       int fase = turno %6;
       switch (fase) {
       case 0:Color = ColorCasilla::NEGRO;break;
       case 1:Color = ColorCasilla::GRIS_OSCURO;break;
       case 2:Color = ColorCasilla::GRIS_CLARO;break;
       case 3:Color = ColorCasilla::BLANCO;break;
       case 4:Color = ColorCasilla::GRIS_CLARO;break;
       case 5:Color = ColorCasilla::GRIS_OSCURO;break;
       }
       //Las casillas oscilantes recorren un ciclo de 6 fases:
       // Negro->Gris_oscuro->Gris_claro->Blanco->Gris_claro->Gris_oscuro->Negro
       //El turno actual determina en qué fase están

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
       //Traduce el enum de color a un color SFML real para pintar
   }
   else {
       if (x < 4) {
           colorRelleno = ((x + y) % 2 == 0) ? sf::Color::White : sf::Color::Black;
           
       }
       else {
           colorRelleno = ((x + y) % 2 == 1) ? sf::Color::White : sf::Color::Black;
       }//Casillas no oscilantes: patrón de ajedrez clásico pero con la paridad invertida para que quede simétrico
       
       if (colorRelleno == sf::Color::White) {
           Color = ColorCasilla::BLANCO;
       }
       else { Color = ColorCasilla::NEGRO; }
   }
  
    cuadrado.setFillColor(colorRelleno);
    ventana.draw(cuadrado);
    //Sincroniza el enum interno con el color visual y dibuja la casilla base

    if (esPuntoDePoder) {
        float proporcion = 0.55f;
        sf::RectangleShape cuadrado(sf::Vector2f(tamano * proporcion, tamano * proporcion));
       float posX = x * tamano+((tamano-tamano*proporcion)/2);  
       float posY = y * tamano + ((tamano - tamano * proporcion) / 2);
       cuadrado.setPosition({ posX, posY });
	   colorRelleno = sf::Color(218, 165, 32);
		cuadrado.setFillColor(colorRelleno);
        ventana.draw(cuadrado);
    }// Si es punto de poder,dibuja un cuadrado dorado centrado de un 55% del tamaño de la casilla encima

    sf::Color colorBorde = (colorRelleno == sf::Color::Black) ? sf::Color(60, 60, 60) : sf::Color(100, 100, 100, 150);
    //Borde sutil
    sf::RectangleShape borde(sf::Vector2f(tamano, tamano));
    borde.setPosition(sf::Vector2f(posX, posY));
    borde.setFillColor(sf::Color::Transparent);
    borde.setOutlineThickness(-1.0f);
    borde.setOutlineColor(colorBorde);
    ventana.draw(borde);
    //Linea de luz superior(efecto visual de relieve)
    sf::RectangleShape luzSuperior(sf::Vector2f(tamano, 2.0f));
    luzSuperior.setPosition(sf::Vector2f(posX, posY));
    luzSuperior.setFillColor(sf::Color(255, 255, 255, 40)); 
    ventana.draw(luzSuperior);
    //Dos capas decorativas: un borde fino y una línea blanca semitransparente arriba para simular un efecto 3D
    if (this == seleccionada) {
        sf::RectangleShape highlight(sf::Vector2f(tamano - 4, tamano - 4));
        highlight.setPosition(sf::Vector2f(posX, posY));
        highlight.setOutlineThickness(2.0f);
        highlight.setOutlineColor(sf::Color::Green);
        highlight.setFillColor(sf::Color::Transparent);
        ventana.draw(highlight);
    }//Si esta casilla es seleccionada por el jugador, dibuja un borde verde encima
    if (piezaOcupante != nullptr) {
        piezaOcupante->dibujar(ventana, seleccionada, turno, tamano);
    }
    }//Delega el dibujado de la pieza a la propia pieza(polimorfismo)

void Casilla::setPieza(Pieza* p) {
    this->piezaOcupante = p;
    if (p != nullptr) {
        p->setPosicion(this); //Le dice a la pieza dónde está
    }
}
//Asigna una pieza a la casilla y le notifica su posición. Si p es nullptr, simplemente vacía la casilla
