#include "Elemental.h"
#include <iostream>


static std::string obtenerRutaRango(std::string skin) {
    if (skin == "HARRY_POTTER") {
        return "imagenes/HP/invocacion.png";
    }
    else if (skin == "STAR_WARS") {
        return "imagenes/SW/invocacion.png";
    }
    else {
        return "imagenes/Classic/invocacion.png";
    }
}


Rango::Rango(Bando b, std::string skin)
    : PiezaVoladora(30, 10, 3, 1, PiezaVoladora::obtenerTexturaDesdeRuta(obtenerRutaRango(skin)), b)
{
    this->rangoMovimiento = 3;
    this->factorEscalaExtra = 2.0f;
    textura.setSmooth(true);
}


