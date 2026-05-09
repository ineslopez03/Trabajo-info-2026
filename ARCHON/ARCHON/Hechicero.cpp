#include "Hechicero.h"
#include <iostream>

sf::Texture& Hechicero::obtenerTexturaHP(Bando b) {
    static sf::Texture tLuz;
    static sf::Texture tOsc;
    static bool cargadaLuz = false;
    static bool cargadaOsc = false;

    if (b == Bando::LUZ) {
        if (!cargadaLuz) {
            if (!tLuz.loadFromFile("imagenes/Dumbledore.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Dumbledore.png" << std::endl;
            }
            cargadaLuz = true;
        }
        return tLuz;
    }
    else {
        if (!cargadaOsc) {
            
            if (!tOsc.loadFromFile("imagenes/Voldemort.png")) {
                std::cerr << "ERROR: No se encuentra imagenes/Voldemort.png" << std::endl;
            }
            cargadaOsc = true;
        }
        return tOsc;
    }
}

Hechicero::Hechicero(Bando b)
    : PiezaTeletransporte(20, 8, 5, 2, obtenerTexturaHP(b), b)
{
    this->velMov = 5;
    sprite.setTexture(obtenerTexturaHP(b));
    for (int i = 0; i < 7; i++) {
        hechizosDisponibles[i] = true;
    }
}