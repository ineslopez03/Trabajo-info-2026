#include "Rango.h"
#include <iostream>

sf::Texture& Rango::obtenerTexturaHP(Bando b) {
    static sf::Texture tLuz;
    static sf::Texture tOsc;
    static bool cargadaLuz = false;
    static bool cargadaOsc = false;

    if (b == Bando::LUZ) {
        if (!cargadaLuz) {
            if (!tLuz.loadFromFile("imagenes/Ginny.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/.png" << std::endl;
            }
            cargadaLuz = true;
        }
        return tLuz;
    }
    else {
        if (!cargadaOsc) {

            if (!tOsc.loadFromFile("imagenes/Bellatrix.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/.png" << std::endl;
            }
            cargadaOsc = true;
        }
        return tOsc;
    }
}

Rango::Rango(Bando b)
    : PiezaVoladora(10, 3, 3, 1, obtenerTexturaHP(b), b)
{

  
    this->velMov = 3;
    sprite.setTexture(obtenerTexturaHP(b));
}