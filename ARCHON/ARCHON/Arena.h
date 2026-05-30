#pragma once
#include "InterfazUsuario.h"
#include "Proyectiles.h"
#include "Pieza.h"
#include "Obstaculos.h"
#include "GraficosArena.h"
#include "ControladorPelea.h"
#include "MotorFisicasArena.h"
#include <list>
#include <SFML/Graphics.hpp>
#include <string>

class Arena : public InterfazUsuario {
private:
    Pieza* piezaIzquierda;
    Pieza* piezaDerecha;
    std::list<Proyectiles*> lista_proyectiles;
    std::list<EfectoOnda> lista_ondas; // Ahora se usa la estructura importada del MotorFisicas

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

    bool combateFinalizado = false;
    float temporizadorSalida = 2.0f;
    sf::Font fuenteVictoria;
    sf::Text textoVictoria;

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
    Pieza* getGanador() { return (piezaIzquierda->getVidaBase() > 0) ? piezaIzquierda : piezaDerecha; }
    Pieza* getPerdedor() { return (piezaIzquierda->getVidaBase() <= 0) ? piezaIzquierda : piezaDerecha; }

    bool isTransicionLista() const { return combateFinalizado && temporizadorSalida <= 0.f; }

   
};