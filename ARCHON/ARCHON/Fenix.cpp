#include "Fenix.h"
#include <iostream>

sf::Texture& Fenix::obtenerTexturaHP(Bando b) {
    static sf::Texture tLuz;
    static sf::Texture tOsc;
    static bool cargadaLuz = false;
    static bool cargadaOsc = false;

    if (b == Bando::LUZ) {
        if (!cargadaLuz) {
            if (!tLuz.loadFromFile("imagenes/Fawkes.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Fawkes.png" << std::endl;
            }
            cargadaLuz = true;
        }
        return tLuz;
    }
    else {
        if (!cargadaOsc) {
            
            if (!tOsc.loadFromFile("imagenes/Dementor.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Dementor.png" << std::endl;
            }
            cargadaOsc = true;
        }
        return tOsc;
    }
}

Fenix::Fenix(Bando b)
    : PiezaVoladora(12, 4, 5, 2, obtenerTexturaHP(b), b)
{
    
    // El Fénix en Archon suele volar 5 casillas
    this->velMov = 5;
    sprite.setTexture(obtenerTexturaHP(b));
}