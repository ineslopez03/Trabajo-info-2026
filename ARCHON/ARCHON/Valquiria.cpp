#include "Valquiria.h"

std::string Valquiria::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Hermion.png" : "imagenes/HP/Bellatrix.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Leia.png" : "imagenes/SW/Morador.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Valkiria.png" : "imagenes/Classic/Banshee.png";
    }
}

Valquiria::Valquiria(Bando b, std::string skin)
    : PiezaVoladora(10, 4, 6, 3, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{
    this->rangoMovimiento = 3;
}

void Valquiria::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaVoladora::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}