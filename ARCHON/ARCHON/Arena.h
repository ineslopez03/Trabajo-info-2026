#pragma once
#include "InterfazUsuario.h"
#include "Proyectiles.h"
#include "Pieza.h"
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

    sf::Vector2f posIzquierda;
    sf::Vector2f posDerecha;

    sf::Clock relojArena;
    std::string skinArena;

    bool teclaDisparoIzquierdaLibre = true;
    bool teclaDisparoDerechaLibre = true;

    // --- Motor del HUD y Cuenta Atrás ---
    sf::Font fuenteArena;
    sf::Text textoCuentaAtras;
    sf::Text textoEtiquetaIzq;
    sf::Text textoEtiquetaDer;
    int faseCuentaAtras;
    sf::Clock relojCuentaAtras;

    sf::RectangleShape marcoIzq, barraFondoIzq, barraVidaIzq;
    sf::RectangleShape marcoDer, barraFondoDer, barraVidaDer;

public:
    Arena(Pieza* p1, Pieza* p2, const std::string& skin);
    ~Arena() override;

    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;
    void iniciarBatalla(Pieza* p1, Pieza* p2);
    void gestionarColisiones();
};