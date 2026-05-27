#include "Arquero.h"

std::string Arquero::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Harry.png" : "imagenes/HP/Dolores.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Hansolo.png" : "imagenes/SW/Bobbafet.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Archon.png" : "imagenes/Classic/Manticora.png";
    }
}

Arquero::Arquero(Bando b, std::string skin)
    : PiezaTerrestre(8, 3, 3, 3, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 3;
}

void Arquero::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}