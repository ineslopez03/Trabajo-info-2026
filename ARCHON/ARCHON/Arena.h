#pragma once
#include "InterfazUsuario.h" // Herencia de InterfazUsuario
#include "Proyectiles.h"     // Composición con Proyectiles
#include "Pieza.h"
#include <list>

class Arena : public InterfazUsuario {
private:
    Pieza* atacante;
    Pieza* defensor;
    std::list<Proyectiles*> lista_proyectiles; // Refleja list_Proyectiles del esquema

    sf::RectangleShape fondoArena;
    sf::Vector2f posAtacante;
    sf::Vector2f posDefensor;
    sf::Clock relojArena;

public:
    Arena(Pieza* p1, Pieza* p2);
    ~Arena() override;

    // Métodos de InterfazUsuario
    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;

    // Métodos de Arena del esquema
    void iniciarBatalla(Pieza* p1, Pieza* p2);
    void gestionarColisiones();
};
