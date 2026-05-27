#include "Golem.h"

std::string Golem::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Hagrid.png" : "imagenes/HP/Troll.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Chewbacca.png" : "imagenes/SW/Guardiarojo.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Golem.png" : "imagenes/Classic/Troll.png";
    }
}

Golem::Golem(Bando b, std::string skin)
    : PiezaTerrestre(18, 7, 2, 1, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 2;
}

void Golem::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}