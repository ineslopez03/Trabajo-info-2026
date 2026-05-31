#include "Elemental.h"
#include <iostream>

// Función libre que devuelve la ruta de imagen del Elemental según el skin
std::string Elemental::obtenerRuta(Bando bando, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return "imagenes/HP/invocacion.png";// Imagen de invocación para skin HP
    }
    else if (skin == "STAR_WARS") {
        return "imagenes/SW/invocacion.png";// Imagen de invocación para skin SW
    }
    else {
        return "imagenes/Classic/invocacion.png";// Imagen de invocación para skin clásico
    }
}

// Constructor: crea el Elemental con estadísticas potentes (es una criatura de un solo uso)
Elemental::Elemental(Bando b, std::string skin)
    : PiezaVoladora(30, 10, 3, 1, PiezaVoladora::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{// vida=30, daño=10, velMov=3, velAta=1; muy resistente y dañino pero lento para atacar
    this->rangoMovimiento = 3; // Se mueve hasta 3 casillas en cualquier dirección
    this->factorEscalaExtra = 2.0f;// Se dibuja el doble de grande que una pieza normal en la arena
    textura.setSmooth(true); // Suavizado de textura para que se vea bien a mayor escala
}