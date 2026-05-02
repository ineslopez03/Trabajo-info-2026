#pragma once
#include "InterfazUsuario.h"
#include "Pieza.h"
#include "Proyectil.h"
#include <SFML/Graphics.hpp>
#include <list>

class Arena : public InterfazUsuario {
private:
    sf::RenderWindow& ventanaArena;
    Pieza* atacante;
    Pieza* defensor;
    std::list<Proyectil*> proyectiles;

    sf::Texture texturaFondo;
    sf::Sprite fondo;

    // Relojes para controlar la cadencia de fuego (cooldown)
    sf::Clock relojAtacante;
    sf::Clock relojDefensor;

public:
    // El constructor recibe la ventana y las dos piezas que van a luchar
    Arena(sf::RenderWindow& ventana, Pieza* p1, Pieza* p2);
    ~Arena() override;

    void procesarEntrada() override;
    void dibujarPantalla() override;

    void gestionarColisiones();
    bool combateFinalizado(); // Indica al motor si alguien ha muerto
};
