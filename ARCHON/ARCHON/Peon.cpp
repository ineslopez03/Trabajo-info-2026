#include "Peon.h"
#include <iostream>

std::string obtenerRutaPeon(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Carpeta HP + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/HP/Dobby.png" : "imagenes/HP/Scabbers.png";
    }
    else if (skin == "STAR_WARS") {
        // Carpeta SW + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/SW/R2D2.png" : "imagenes/SW/Stormtrooper.png";
    }
    else {
        // Carpeta CLASSIC + nombres estándar
        return (b == Bando::LUZ) ? "imagenes/Classic/Knight.png" : "imagenes/Classic/Goblin.png";
    }
}

Peon::Peon(Bando b, std::string skin)
    : PiezaTerrestre(8, 2, 3, 0, PiezaTerrestre::obtenerTexturaDesdeRuta(obtenerRutaPeon(b, skin)), b)
{
    this->rangoMovimiento = 3;
}