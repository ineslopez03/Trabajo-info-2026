#include "Arena.h"

Arena::Arena(sf::RenderWindow& ventana, Pieza* p1, Pieza* p2)
    : ventanaArena(ventana), atacante(p1), defensor(p2) {

    // Posicionamos las piezas en extremos opuestos de la arena
    if (atacante) {
        // Suponiendo un tamaño de ventana de 800x600
        // En la arena, la posición es absoluta (píxeles), no por casillas
    }

    // Cargar fondo temático según la normativa (opcional)
    // texturaFondo.loadFromFile("assets/background_arena.png");
    // fondo.setTexture(texturaFondo);
}

Arena::~Arena() {
    // Limpiamos los proyectiles que quedaron en vuelo
    for (auto p : proyectiles) {
        delete p;
    }
    proyectiles.clear();
}

void Arena::procesarEntrada() {
    sf::Event evento;
    while (ventanaArena.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed)
            ventanaArena.close();
    }

    // Lógica de ataque (Ejemplo: Tecla J para Atacante, L para Defensor)
    // Se usa el atributo velAtq de la clase Pieza de tu UML
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        if (relojAtacante.getElapsedTime().asSeconds() > 1.0f / atacante->getVelAtq()) {
            // proyectiles.push_back(new Proyectil(...));
            relojAtacante.restart();
        }
    }

    // Movimiento de las piezas en la arena
    // Aquí podrías llamar a un método p1->moverEnArena() diferente al del tablero
}

void Arena::gestionarColisiones() {
    auto it = proyectiles.begin();
    while (it != proyectiles.end()) {
        Proyectil* p = *it;
        p->mover();

        bool impacto = false;
        // Comprobar si el proyectil impacta al oponente[cite: 2]
        if (p->comprobarImpacto(defensor)) {
            defensor->recibirDanyo(p->getDanyo());
            impacto = true;
        }

        // Eliminar si impacta o sale de pantalla
        if (impacto || p->fueraDeLimites(800, 600)) {
            delete p;
            it = proyectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Arena::dibujarPantalla() {
    ventanaArena.draw(fondo);

    // Dibujamos las piezas (usando el método dibujar de Pieza que ya tienes)[cite: 2]
    // Nota: Necesitarás ajustar las coordenadas para la vista de la arena
    atacante->dibujar(ventanaArena, 100, 300, 50);
    defensor->dibujar(ventanaArena, 700, 300, 50);

    for (auto p : proyectiles) {
        p->dibujar(ventanaArena);
    }

    // Gestión de colisiones en cada frame
    gestionarColisiones();
}

bool Arena::combateFinalizado() {
    // Según normativa: El combate termina cuando una pieza muere
    // Suponiendo que tienes un método getVida() en Pieza
    // return (atacante->getVida() <= 0 || defensor->getVida() <= 0);
    return false; // Implementar lógica de vida
}