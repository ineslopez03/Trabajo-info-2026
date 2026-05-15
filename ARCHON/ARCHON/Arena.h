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
    Pieza* piezaIzquierda;
    Pieza* piezaDerecha;
    std::list<Proyectiles*> lista_proyectiles;

    sf::Texture texturaFondoArena;
    sf::Sprite* spriteFondoArena;
    sf::Vector2f posIzquierda, posDerecha;

    sf::Clock relojArena, relojCuentaAtras;
    std::string skinArena;
    int faseCuentaAtras;
    bool teclaDisparoIzquierdaLibre = true;
    bool teclaDisparoDerechaLibre = true;

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