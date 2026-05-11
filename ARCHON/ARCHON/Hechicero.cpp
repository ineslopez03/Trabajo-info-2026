#include "Hechicero.h"
#include <iostream>

std::string Hechicero::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Dumbledore.png" : "imagenes/HP/Voldemort.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Yoda.png" : "imagenes/SW/Palpatine.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Wizard.png" : "imagenes/Classic/Sorceress.png";
    }
}

Hechicero::Hechicero(Bando b, std::string skin)
    : PiezaTeletransporte(20, 8, 5, 2, Pieza::obtenerTexturaDesdeRuta(Hechicero::obtenerRuta(b, skin)), b)
{
    this->velMov = 5;

    for (int i = 0; i < 7; i++) {
        hechizosDisponibles[i] = true;
    }
}