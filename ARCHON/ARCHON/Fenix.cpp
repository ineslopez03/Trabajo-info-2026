#include "Fenix.h"

std::string Fenix::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Fawkes.png" : "imagenes/HP/Dementor.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Luke.png" : "imagenes/SW/Darthvader.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Fenix.png" : "imagenes/Classic/Dragon.png";
    }
}

Fenix::Fenix(Bando b, std::string skin)
    : PiezaVoladora(16, 8, 4, 1, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 4;
}

void Fenix::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaVoladora::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}