#include "Hechicero.h"
#include <iostream>

std::string Hechicero::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Dumbledore.png" : "imagenes/HP/Voldemort.png";
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Yoda.png" : "imagenes/SW/Palpatin.png";
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Hechicero.png" : "imagenes/Classic/Hechicera.png";
    }
}

Hechicero::Hechicero(Bando b, std::string skin)
    : PiezaTeletransporte(20, 8, 5, 2, Pieza::obtenerTexturaDesdeRuta(Hechicero::obtenerRuta(b, skin)), b)
{
    this->velMov = 5;
    for (int i = 0; i < 7; i++) {
        hechizosDisponibles[i] = true;
    }
}

void Hechicero::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    if (skin == "HARRY_POTTER" && bando == Bando::LUZ) {
        sf::Texture& sheet = Pieza::obtenerTexturaDesdeRuta("imagenes/HP/Arena/Dumbledore_Sheet.png");
        sprite.setTexture(sheet);
        sprite.setTextureRect(sf::IntRect({ 5, 5 }, { 40, 60 }));

        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setPosition(pos);

        // Escala reducida para la arena
        sprite.setScale({ mirandoDerecha ? 1.3f : -1.3f, 1.3f });
        ventana.draw(sprite);
    }
    else {
        PiezaTeletransporte::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
    }
}