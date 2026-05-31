#pragma once 
#include <SFML/Graphics.hpp> 
#include <string> 
#include "Pieza.h" 

//separa el dibujo de la interfaz (barras de vida) de la logica de la arena
class GraficosArena {
private:
    sf::Font fuenteArena; // fuente que usa para todos los textos
    sf::Text textoCuentaAtras; // texto para el "3, 2, 1" inicial
    sf::Text textoEtiquetaIzq, textoEtiquetaDer; // titulos "jugador 1" y "jugador 2"
    sf::RectangleShape marcoIzq, barraFondoIzq, barraVidaIzq; // los 3 rectangulos que forman la barra de salud izquierda (borde, fondo oscuro y vida verde)
    sf::RectangleShape marcoDer, barraFondoDer, barraVidaDer; // los 3 rectangulos para la barra del lado derecho

  
    const Pieza* refPiezaIzq = nullptr; // puntero de lectura a la pieza 1 para saber cuanta vida tiene
    const Pieza* refPiezaDer = nullptr; // puntero de lectura a la pieza 2 para saber cuanta vida tiene

public:
    // constructor para configurar tamaños, colores y posiciones de los rectangulos
    GraficosArena();

    
    void vincularPiezas(const Pieza* pIzq, const Pieza* pDer); // le pasa las piezas que van a pelear para que las lea

    
    void actualizar(int faseCuenta); // calcula los porcentajes de salud y recorta las barras verdes
    void dibujar(sf::RenderWindow& ventana, bool mostrarCuentaAtras) const; // pinta la interfaz entera por encima de la arena
};