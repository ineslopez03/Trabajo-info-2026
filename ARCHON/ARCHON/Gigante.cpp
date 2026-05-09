#include "Gigante.h"
#include <iostream>

sf::Texture& Gigante::obtenerTexturaHP(Bando b) {
    // static asegura que la textura viva durante todo el programa
    static sf::Texture tLuz;
    static sf::Texture tOsc;
    static bool cargadaLuz = false;
    static bool cargadaOsc = false;

    if (b == Bando::LUZ) {
        if (!cargadaLuz) {
            if (!tLuz.loadFromFile("imagenes/Hagrid.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Dobby.png" << std::endl;
            }
            cargadaLuz = true;
        }
        return tLuz;
    }
    else {
        if (!cargadaOsc) {
            if (!tOsc.loadFromFile("imagenes/Troll.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Scabbers.png" << std::endl;
            }
            cargadaOsc = true;
        }
        return tOsc;
    }
}

Gigante::Gigante(Bando b)
    : PiezaTerrestre(15, 5, 2, 0, obtenerTexturaHP(b), b)
{
    this->rangoMovimiento = 2; // El Gigante camina 2 casillas
    sprite.setTexture(obtenerTexturaHP(b));
}