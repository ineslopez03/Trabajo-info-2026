#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Obstaculos {
private:
    struct Orbe {
        sf::CircleShape cuerpo;
        sf::CircleShape nucleo;
        float radio;
        sf::Vector2f posicion;
    };

    std::vector<Orbe> lista_orbes;
    sf::Clock relojCrecimiento;
    const int MAX_OBSTACULOS = 22;
    const float RADIO_BASE = 18.f;

    // Renombrado para cumplir el estándar ASCII
    void agregarParDeOrbes(const sf::Vector2f& posAtacante, const sf::Vector2f& posDefensor);
    bool verificarSolapamiento(sf::Vector2f nuevaPos, float nuevoRadio);

public:
    Obstaculos(const sf::Vector2f& p1, const sf::Vector2f& p2);

    void actualizar(float dt, const sf::Vector2f& p1, const sf::Vector2f& p2);
    void dibujar(sf::RenderWindow& ventana) const;
    void reiniciar(const sf::Vector2f& p1, const sf::Vector2f& p2);

    bool hayColisionCircular(const sf::Vector2f& posEntidad, float radioEntidad) const;
};