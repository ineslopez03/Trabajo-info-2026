#include "Unicornio.h"

std::string Unicornio::obtenerRuta(Bando b, std::string skin) {//asigna la imagen segun la tematica
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Unicornio.png" : "imagenes/HP/Basilisco.png";//skin de harry potter
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Lando.png" : "imagenes/SW/Bossk.png";//skin de star wars
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Unicornio.png" : "imagenes/Classic/Basilisco.png";//skin clasico
    }
}

Unicornio::Unicornio(Bando b, std::string skin)
    : PiezaTerrestre(12, 5, 5, 4, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b) {//estadisticas base
    this->rangoMovimiento = 3;//se mueve hasta tres casillas
}