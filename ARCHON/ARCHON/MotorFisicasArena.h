#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "Pieza.h"
#include "Proyectiles.h"
#include "Obstaculos.h"

// Estructura de renderizado para el área de efecto (Hitbox visual)
struct EfectoOnda {
    sf::CircleShape forma;
    float radio;
    float opacidad;
    sf::Color colorBando;
};

class MotorFisicasArena {
public:
    static void aplicarFisicaMovimiento(Pieza* pieza, sf::Vector2f& pos, sf::Vector2f dir, sf::Vector2f posEnemigo, float dt, const Obstaculos& obstaculos);
    static void gestionarColisiones(Pieza* piezaIzq, Pieza* piezaDer, std::list<Proyectiles*>& listaProyectiles, const Obstaculos& obstaculos);

    // Módulo heurístico de autoapuntado sectorizado
    static sf::Vector2f calcularDireccionOrtogonal(sf::Vector2f posAtacante, sf::Vector2f posEnemigo);

    // Actualización y destrucción de malla visual
    static void propagarOndas(std::list<EfectoOnda>& listaOndas, float dt);
    static void generarOndaChoque(sf::Vector2f pos, Bando b, std::list<EfectoOnda>& listaOndas);
};