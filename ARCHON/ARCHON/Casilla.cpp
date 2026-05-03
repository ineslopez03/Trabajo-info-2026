#include "Casilla.h"
#include<iostream>
#include <SFML/Graphics.hpp>
Casilla::Casilla(int _x, int _y, bool _osc)
	: x(_x), y(_y), esOscilante(_osc), piezaOcupante(nullptr) {}
Casilla::~Casilla() {}
void Casilla::dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, float tiempo, float tamano) {
    sf::RectangleShape cuadrado(sf::Vector2f(tamano, tamano));
    float posX = x * tamano;
    float posY = y * tamano;
    cuadrado.setPosition({ posX, posY });


    bool esPuntoPoder = (x == 0 || x == 4 || x == 8) && (y == 0 || y == 4 || y == 8);
    sf::Color colorRelleno;

    if (esPuntoPoder) {
        colorRelleno = sf::Color(218, 165, 32);
    }
    else {
        if (esOscilante) {
            int ciclo[] = { 0, 1, 2, 3, 3, 2, 1, 0 };
            int fase = (int)(tiempo / 2.0f) % 8;
            int intensidad = ciclo[fase];
            switch (intensidad) {
            case 0: colorRelleno = sf::Color::Black; break;
            case 1: colorRelleno = sf::Color(45, 45, 45); break;
            case 2: colorRelleno = sf::Color(140, 140, 140); break;
            case 3: colorRelleno = sf::Color::White; break;
            }
        }
        else {
            colorRelleno = ((x + y) % 2 == 0) ? sf::Color::White : sf::Color::Black;
        }
    }
    cuadrado.setFillColor(colorRelleno);
    ventana.draw(cuadrado);

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