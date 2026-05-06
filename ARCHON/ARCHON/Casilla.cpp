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
}//constructor de la casilla, la pieza ocupante esta vacia por defecto


Casilla::~Casilla() {

}


void Casilla::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) {
    sf::RectangleShape cuadrado(sf::Vector2f(tamano, tamano));//se elige el cuadrado de tamaño tamano
	float posX = x * tamano; //se le asigna la posicion en x multiplicando la coordenada x por el tamaño de la casilla
	float posY = y * tamano;//se le asigna la posicion en y multiplicando la coordenada y por el tamaño de la casilla
    cuadrado.setPosition({ posX, posY });// se le asigna finalmente la posicion al cuadrado
    
   sf::Color colorRelleno;//declaro el que va a ser el color de relleno 

   //lógica de como se va a pintar
   if (esOscilante) {
       int fase = turno % 4;// esto simplemente mira el resto de la operacion y el resto siempre va a ir de 0 a 3
       switch (fase) {//se debe asegurar que turno empieze en 0 y se reinicie cuando llegue a 3
       case 0:Color = ColorCasilla::NEGRO;break;
       case 1:Color = ColorCasilla::GRIS_OSCURO;break;
       case 2:Color = ColorCasilla::GRIS_CLARO;break;
       case 3:Color = ColorCasilla::BLANCO;break;
       }

       if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {//metodo para visualizar lo que estoy haciendo
           Color = ColorCasilla::GRIS_CLARO;
       }

       switch (Color)//segun el color que toque el color de relleno sera uno u otro
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
       if (x < 4) {//pintamos una mitad
           colorRelleno = ((x + y) % 2 == 0) ? sf::Color::White : sf::Color::Black;
           
       }
       else {//la otra mitad está invertida 
           colorRelleno = ((x + y) % 2 == 1) ? sf::Color::White : sf::Color::Black;
       }
       if (colorRelleno == sf::Color::White) {
           Color = ColorCasilla::BLANCO;
       }
       else { Color = ColorCasilla::NEGRO; }
   }
  
    cuadrado.setFillColor(colorRelleno);// aplico el color de relleno
    ventana.draw(cuadrado);// y lo pinto ahora

    if (esPuntoDePoder) {//en caso de que sea p. de poder le pinto un rectangulo dorado para saber cual es cual 
        float proporcion = 0.55f;//proporcion para reducir el tamaño del cuadrado del punto de poder
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