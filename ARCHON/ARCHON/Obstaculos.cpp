#include "Obstaculos.h"
#include <random>
#include <cmath>

Obstaculos::Obstaculos(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    reiniciar(p1, p2);
}

void Obstaculos::reiniciar(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    lista_orbes.clear();
    relojCrecimiento.restart();

    for (int i = 0; i < 3; ++i) {
        agregarParDeOrbes(p1, p2); // Llamada corregida
    }
}

bool Obstaculos::verificarSolapamiento(sf::Vector2f nuevaPos, float nuevoRadio) {
    for (const auto& orbe : lista_orbes) {
        float dx = nuevaPos.x - orbe.posicion.x;
        float dy = nuevaPos.y - orbe.posicion.y;
        float distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia < (nuevoRadio + orbe.radio + 5.f)) return true;
    }
    return false;
}

// Declaración corregida
void Obstaculos::agregarParDeOrbes(const sf::Vector2f& posAtacante, const sf::Vector2f& posDefensor) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(100.f, 350.f);
    std::uniform_real_distribution<float> distY(100.f, 700.f);

    int intentos = 0;
    while (intentos < 50) {
        float x = distX(gen);
        float y = distY(gen);
        sf::Vector2f nuevaPos(x, y);

        float dAtacante = std::hypot(x - posAtacante.x, y - posAtacante.y);
        float dDefensor = std::hypot(x - posDefensor.x, y - posDefensor.y);

        if (!verificarSolapamiento(nuevaPos, RADIO_BASE) && dAtacante > 60.f && dDefensor > 60.f) {
            Orbe o;
            o.radio = RADIO_BASE;
            o.posicion = nuevaPos;

            o.cuerpo.setRadius(RADIO_BASE);
            o.cuerpo.setOrigin({ RADIO_BASE, RADIO_BASE });
            o.cuerpo.setPosition(nuevaPos);
            o.cuerpo.setFillColor(sf::Color(70, 130, 180, 180));
            o.cuerpo.setOutlineThickness(2.f);
            o.cuerpo.setOutlineColor(sf::Color::White);

            o.nucleo.setRadius(RADIO_BASE * 0.4f);
            o.nucleo.setOrigin({ RADIO_BASE * 0.4f, RADIO_BASE * 0.4f });
            o.nucleo.setPosition(nuevaPos);
            o.nucleo.setFillColor(sf::Color::Cyan);

            lista_orbes.push_back(o);

            Orbe simetrico = o;
            simetrico.posicion.x = 800.f - x;
            simetrico.cuerpo.setPosition(simetrico.posicion);
            simetrico.nucleo.setPosition(simetrico.posicion);
            lista_orbes.push_back(simetrico);
            break;
        }
        intentos++;
    }
}

void Obstaculos::actualizar(float dt, const sf::Vector2f& p1, const sf::Vector2f& p2) {
    if (relojCrecimiento.getElapsedTime().asSeconds() >= 5.0f) {
        if (lista_orbes.size() >= MAX_OBSTACULOS) {
            reiniciar(p1, p2);
        }
        else {
            agregarParDeOrbes(p1, p2); // Llamada corregida
        }
        relojCrecimiento.restart();
    }
}

bool Obstaculos::hayColisionCircular(const sf::Vector2f& posEntidad, float radioEntidad) const {
    for (const auto& orbe : lista_orbes) {
        float dx = posEntidad.x - orbe.posicion.x;
        float dy = posEntidad.y - orbe.posicion.y;
        float distancia = std::sqrt(dx * dx + dy * dy);
        if (distancia < (orbe.radio + radioEntidad)) return true;
    }
    return false;
}

void Obstaculos::dibujar(sf::RenderWindow& ventana) const {
    for (const auto& orbe : lista_orbes) {
        ventana.draw(orbe.cuerpo);
        ventana.draw(orbe.nucleo);
    }
}