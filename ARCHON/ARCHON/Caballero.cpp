#include "Caballero.h"

std::string Caballero::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Dobby.png" : "imagenes/HP/Griphook.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Rebelde.png" : "imagenes/SW/Stormtrooper.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Caballero.png" : "imagenes/Classic/Goblin.png";
    }
}

Caballero::Caballero(Bando b, std::string skin)
    : PiezaTerrestre(10, 4, 4, 2, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b) {
    this->rangoMovimiento = 3;
}

void Caballero::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    // Lógica de escalado heredada del diseño anterior
    PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}