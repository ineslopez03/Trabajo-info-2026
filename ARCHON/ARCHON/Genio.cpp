#include "Genio.h"

std::string Genio::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Nick.png" : "imagenes/HP/Boggart.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Obiwan.png" : "imagenes/SW/IG.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Genio.png" : "imagenes/Classic/Shapeshifter.png";
    }
}

Genio::Genio(Bando b, std::string skin)
    : PiezaVoladora(14, 5, 5, 3, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 4;
}

void Genio::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaVoladora::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}