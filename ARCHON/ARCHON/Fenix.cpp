#include "Fenix.h"

std::string Fenix::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Placeholder_Fenix.png" : "imagenes/HP/Placeholder_Dragon.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Placeholder_Fenix.png" : "imagenes/SW/Placeholder_Dragon.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Phoenix.png" : "imagenes/Classic/Dragon.png";
    }
}

Fenix::Fenix(Bando b, std::string skin)
    : PiezaVoladora(16, 8, 4, 1, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 5;
}

void Fenix::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaVoladora::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}