#include "Peon.h"
#include <iostream>

sf::Texture& Peon::obtenerTexturaHP(Bando b) {
    // static asegura que la textura viva durante todo el programa
    static sf::Texture tLuz;
    static sf::Texture tOsc;
    static bool cargadaLuz = false;
    static bool cargadaOsc = false;

    if (b == Bando::LUZ) {
        if (!cargadaLuz) {
            if (!tLuz.loadFromFile("imagenes/Dobby.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Dobby.png" << std::endl;
            }
            cargadaLuz = true;
        }
        return tLuz;
    }
    else {
        if (!cargadaOsc) {
            if (!tOsc.loadFromFile("imagenes/Scabbers.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Scabbers.png" << std::endl;
            }
            cargadaOsc = true;
        }
        return tOsc;
    }
}

Peon::Peon(Bando b)
    : PiezaTerrestre(8, 2, 2, 0, obtenerTexturaHP(b), b)
{
    this->rangoMovimiento = 3; // El Peón camina 3 casillas
    sprite.setTexture(obtenerTexturaHP(b));
}