#pragma once
#include "InterfazUsuario.h"
#include "Proyectiles.h"
#include "Pieza.h"
#include "Obstaculos.h"
#include "GraficosArena.h"
#include <list>
#include <SFML/Graphics.hpp>
#include <string>

class Arena : public InterfazUsuario {
private:
    // Estructura de renderizado para el área de efecto (Hitbox visual)
    struct EfectoOnda {
        sf::CircleShape forma;
        float radio;
        float opacidad;
        sf::Color colorBando;
    };

    Pieza* piezaIzquierda;
    Pieza* piezaDerecha;
    std::list<Proyectiles*> lista_proyectiles;
    std::list<EfectoOnda> lista_ondas; // Gestor de partículas de choque

    sf::Texture texturaFondoArena;
    sf::Sprite* spriteFondoArena;
    sf::Vector2f posIzquierda, posDerecha;
    sf::Clock relojArena, relojCuentaAtras;
    std::string skinArena;
    int faseCuentaAtras;
    bool teclaDisparoIzquierdaLibre = true;
    bool teclaDisparoDerechaLibre = true;

    float tiempoRestanteCooldownIzq = 0.f;
    float tiempoRestanteCooldownDer = 0.f;

    Obstaculos obstaculos;
    GraficosArena graficos;
    Pieza* atacanteOriginal;

public:
    Arena(Pieza* p1, Pieza* p2, const std::string& skin, Pieza* atacante);
    ~Arena() override;

    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;
    void iniciarBatalla(Pieza* p1, Pieza* p2);
    void gestionarColisiones();

    Pieza* getPiezaIzquierda() { return piezaIzquierda; }
    Pieza* getPiezaDerecha() { return piezaDerecha; }
    Pieza* getPiezaAtacanteReal() const { return atacanteOriginal; }
};