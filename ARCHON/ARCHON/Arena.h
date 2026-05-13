#pragma once
#include "InterfazUsuario.h"
#include "Proyectiles.h"
#include "Pieza.h"
#include <list>
#include <SFML/Graphics.hpp>
#include <string>

class Arena : public InterfazUsuario {
private:
    Pieza* atacante;
    Pieza* defensor;
    std::list<Proyectiles*> lista_proyectiles;

    sf::Texture texturaFondoArena;
    sf::Sprite* spriteFondoArena;  // Usamos puntero para construir más tarde

    sf::Vector2f posAtacante;
    sf::Vector2f posDefensor;
    sf::Clock relojArena;

public:
    Arena(Pieza* p1, Pieza* p2, const std::string& skin);
    ~Arena() override;

    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;

    void iniciarBatalla(Pieza* p1, Pieza* p2);
    void gestionarColisiones();
};
