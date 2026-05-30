#include "Unicornio.h"

std::string Unicornio::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Unicornio.png" : "imagenes/HP/Basilisco.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Lando.png" : "imagenes/SW/Bossk.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Unicornio.png" : "imagenes/Classic/Basilisco.png";
    }
}

Unicornio::Unicornio(Bando b, std::string skin)
    : PiezaTerrestre(12, 5, 5, 4, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 3;
}

void Unicornio::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}