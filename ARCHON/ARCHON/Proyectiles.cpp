#include "Proyectiles.h"
#include <cmath> // Requerido para la función matemática std::atan2

Proyectiles::Proyectiles(float _x, float _y, int _danyo, float _vel, sf::Vector2f _dir, Bando _bando, std::string skin)
    : x(_x), y(_y), danyo(_danyo), velocidad(_vel), direccion(_dir), bandoOrigen(_bando)
{
    sf::Color colorBase;
    sf::Color colorLuz;

    // Diferenciación algorítmica de la temática visual
    if (skin == "STAR_WARS") {
        colorBase = sf::Color::White;
        colorLuz = (bandoOrigen == Bando::LUZ) ? sf::Color(0, 150, 255, 150) : sf::Color(255, 0, 0, 150);
        nucleo.setSize({ 25.f, 4.f });
        resplandor.setSize({ 35.f, 12.f });
    }
    else if (skin == "HARRY_POTTER") {
        colorBase = (bandoOrigen == Bando::LUZ) ? sf::Color(255, 100, 100) : sf::Color(100, 255, 100);
        colorLuz = (bandoOrigen == Bando::LUZ) ? sf::Color(220, 20, 60, 180) : sf::Color(0, 255, 0, 180);
        nucleo.setSize({ 15.f, 6.f });
        resplandor.setSize({ 22.f, 16.f });
    }
    else { // ARCHON Clásico
        colorBase = sf::Color::White;
        colorLuz = (bandoOrigen == Bando::LUZ) ? sf::Color(255, 215, 0, 150) : sf::Color(138, 43, 226, 150);
        nucleo.setSize({ 18.f, 5.f });
        resplandor.setSize({ 28.f, 14.f });
    }

    nucleo.setFillColor(colorBase);
    resplandor.setFillColor(colorLuz);

    // Centrado de pivotes geométricos
    nucleo.setOrigin({ nucleo.getSize().x / 2.f, nucleo.getSize().y / 2.f });
    resplandor.setOrigin({ resplandor.getSize().x / 2.f, resplandor.getSize().y / 2.f });
    nucleo.setPosition({ x, y });
    resplandor.setPosition({ x, y });

    // Transformación afín: Cálculo del ángulo de rotación respecto al vector de desplazamiento
    float anguloRadianes = std::atan2(direccion.y, direccion.x);

    // Corrección C2664: Instanciación estricta de sf::Angle nativo de SFML 3.x
    nucleo.setRotation(sf::radians(anguloRadianes));
    resplandor.setRotation(sf::radians(anguloRadianes));
}

Proyectiles::~Proyectiles() {}

void Proyectiles::mover(float dt) {
    // Cálculo de trayectoria balística por diferencial de tiempo
    x += direccion.x * velocidad * dt;
    y += direccion.y * velocidad * dt;

    nucleo.setPosition({ x, y });
    resplandor.setPosition({ x, y });
}

void Proyectiles::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(resplandor);
    ventana.draw(nucleo);
}