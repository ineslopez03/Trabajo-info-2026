#include "Unicornio.h"

std::string Unicornio::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Fawkes.png" : "imagenes/HP/Dementor.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/HanSolo.png" : "imagenes/SW/Palpatine.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Unicorn.png" : "imagenes/Classic/Basilisk.png";
    }
}

Unicornio::Unicornio(Bando b, std::string skin)
    : PiezaTerrestre(12, 5, 5, 4, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 4;
}

void Unicornio::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}