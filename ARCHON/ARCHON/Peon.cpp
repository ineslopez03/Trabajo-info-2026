#include "Peon.h"
#include <iostream>

std::string obtenerRutaPeon(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Dobby.png" : "imagenes/HP/Scabbers.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/R2D2.png" : "imagenes/SW/Stormtrooper.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Knight.png" : "imagenes/Classic/Goblin.png";
    }
}

Peon::Peon(Bando b, std::string skin)
    : PiezaTerrestre(8, 2, 3, 0, PiezaTerrestre::obtenerTexturaDesdeRuta(obtenerRutaPeon(b, skin)), b)
{
    this->rangoMovimiento = 3;
}

void Peon::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    if (skin == "HARRY_POTTER" && bando == Bando::LUZ) {
        sf::Texture& sheet = Pieza::obtenerTexturaDesdeRuta("imagenes/HP/Arena/Harry_Sheet.png");
        sprite.setTexture(sheet);

        // Extracción del rectángulo correspondiente a la pose 'Idle'
        sprite.setTextureRect(sf::IntRect({ 15, 252 }, { 28, 42 }));

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setPosition(pos);

        // Escala adaptada a las proporciones del pixel art
        sprite.setScale({ mirandoDerecha ? 2.5f : -2.5f, 2.5f });

        ventana.draw(sprite);
    }
    else {
        // Ejecución del comportamiento gráfico por defecto
        PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
    }
}