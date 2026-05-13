#include "Gigante.h"
#include <iostream>

std::string obtenerRutaGigante(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Hagrid.png" : "imagenes/HP/Troll.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Chewbacca.png" : "imagenes/SW/Rancor.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Golem.png" : "imagenes/Classic/Troll.png";
    }
}

Gigante::Gigante(Bando b, std::string skin)
    : PiezaTerrestre(15, 5, 2, 0, PiezaTerrestre::obtenerTexturaDesdeRuta(obtenerRutaGigante(b, skin)), b)
{
    this->rangoMovimiento = 2;
}

void Gigante::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    if (skin == "HARRY_POTTER" && bando == Bando::LUZ) {
        sf::Texture& sheet = Pieza::obtenerTexturaDesdeRuta("imagenes/HP/Arena/Hagrid_Sheet.png");
        sprite.setTexture(sheet);

        // Extracción del rectángulo correspondiente a la pose 'Stand'
        sprite.setTextureRect(sf::IntRect({ 10, 10 }, { 60, 80 }));

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setPosition(pos);

        sprite.setScale({ mirandoDerecha ? 1.8f : -1.8f, 1.8f });

        ventana.draw(sprite);
    }
    else {
        // Ejecución del comportamiento gráfico por defecto
        PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
    }
}