#pragma once
#include "InterfazUsuario.h"
#include "Proyectiles.h"
#include "Pieza.h"
#include <list>
#include <SFML/Graphics.hpp>
#include <string>

class Arena : public InterfazUsuario {
private:
    // Punteros para identificar las piezas por bando/posición
    Pieza* piezaIzquierda; // Normalmente Bando::LUZ
    Pieza* piezaDerecha;   // Normalmente Bando::OSCURIDAD

    std::list<Proyectiles*> lista_proyectiles;
    sf::Texture texturaFondoArena;
    sf::Sprite* spriteFondoArena;

    // Vectores de posición independientes
    sf::Vector2f posIzquierda;
    sf::Vector2f posDerecha;

    sf::Clock relojArena;
    std::string skinArena;

    // Variables para controlar la cadencia de disparo (teclas libres)
    bool teclaDisparoIzquierdaLibre = true;
    bool teclaDisparoDerechaLibre = true;

public:
    Arena(Pieza* p1, Pieza* p2, const std::string& skin);
    ~Arena() override;

    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;
    void iniciarBatalla(Pieza* p1, Pieza* p2);
    void gestionarColisiones();
};