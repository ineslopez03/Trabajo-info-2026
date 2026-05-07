#include "Peon.h"
#include <iostream>

// Implementación de la carga de textura
sf::Texture& Peon::obtenerTexturaHP(Bando b) {
    static sf::Texture tLuz, tOsc;
    static bool cargado = false;
    if (!cargado) {
        if (!tLuz.loadFromFile("imagenes/Dobby.png")) std::cerr << "Error: dobby.png no encontrado\n";
        if (!tOsc.loadFromFile("imagenes/Scabbers.png")) std::cerr << "Error: rata.png no encontrado\n";
        cargado = true;
    }
    return (b == Bando::LUZ) ? tLuz : tOsc;
}

// Implementación del constructor
Peon::Peon(Bando b)
    : PiezaTerrestre(8, 2, 2, 0, obtenerTexturaHP(b), b)
{
    // Aquí puedes ajustar algo del sprite si fuera necesario
}
