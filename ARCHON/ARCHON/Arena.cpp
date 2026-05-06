#include "Arena.h"

Arena::Arena(Pieza* p1, Pieza* p2) {
    iniciarBatalla(p1, p2);
    fondoArena.setSize(sf::Vector2f(800.f, 800.f));
    fondoArena.setFillColor(sf::Color(25, 25, 25));
}

Arena::~Arena() {
    for (auto p : lista_proyectiles) delete p;
    lista_proyectiles.clear();
}

void Arena::iniciarBatalla(Pieza* p1, Pieza* p2) {
    atacante = p1;
    defensor = p2;
    posAtacante = sf::Vector2f(100.f, 400.f);
    posDefensor = sf::Vector2f(700.f, 400.f);
}

void Arena::procesarEntrada(sf::RenderWindow& ventana) {
    float dt = relojArena.restart().asSeconds();

    // Ejemplo de movimiento de piezas en Arena
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) posAtacante.y -= 400.f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) posAtacante.y += 400.f * dt;

    // Creación dinámica de un proyectil (Composición)
    static bool teclaLibre = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (teclaLibre) {
            lista_proyectiles.push_back(new Proyectiles(posAtacante.x, posAtacante.y, 10, 8.0f, { 1.f, 0.f }, atacante->getBando()));
            teclaLibre = false;
        }
    }
    else {
        teclaLibre = true;
    }

    // Actualizar cada proyectil de la lista
    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) {
        (*it)->mover();

        if ((*it)->getPosicion().x > 800 || (*it)->getPosicion().x < 0) {
            delete* it;
            it = lista_proyectiles.erase(it);
        }
        else {
            ++it;
        }
    }

    gestionarColisiones();
}

void Arena::gestionarColisiones() {
    // Aquí se invoca comprobarImpacto() según el esquema
}

void Arena::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.draw(fondoArena);
    if (atacante) atacante->dibujar(ventana, posAtacante.x, posAtacante.y, 50.f);
    if (defensor) defensor->dibujar(ventana, posDefensor.x, posDefensor.y, 50.f);

    for (auto p : lista_proyectiles) {
        p->dibujar(ventana);
    }
}