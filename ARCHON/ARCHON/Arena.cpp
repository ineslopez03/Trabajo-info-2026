#include "Arena.h"
#include <iostream>

Arena::Arena(Pieza* p1, Pieza* p2, const std::string& skin) : spriteFondoArena(nullptr) {
    iniciarBatalla(p1, p2);

    std::string rutaFondo;
    if (skin == "ARCHON") {
        rutaFondo = "imagenes/CLASSIC/suelo_clss_2.png";
    }
    else if (skin == "HARRY_POTTER") {
        rutaFondo = "imagenes/HP/suelo_hp_2.png";
    }
    else if (skin == "STAR_WARS") {
        rutaFondo = "imagenes/SW/suelo_sw_2.png";
    }
    else {
        rutaFondo = "imagenes/CLASSIC/suelo_clss_2.png";
    }

    if (!texturaFondoArena.loadFromFile(rutaFondo)) {
        std::cerr << "Error cargando la textura de fondo de Arena: " << rutaFondo << std::endl;
    }

    spriteFondoArena = new sf::Sprite(texturaFondoArena);

    // Casting explícito a float para garantizar precisión en la división y evitar truncamientos
    float escalaX = 800.f / static_cast<float>(texturaFondoArena.getSize().x);
    float escalaY = 800.f / static_cast<float>(texturaFondoArena.getSize().y);
    spriteFondoArena->setScale(sf::Vector2f(escalaX, escalaY));
}

Arena::~Arena() {
    for (auto p : lista_proyectiles) delete p;
    lista_proyectiles.clear();
    delete spriteFondoArena;
}

void Arena::iniciarBatalla(Pieza* p1, Pieza* p2) {
    atacante = p1;
    defensor = p2;
    posAtacante = sf::Vector2f(100.f, 400.f);
    posDefensor = sf::Vector2f(700.f, 400.f);
}

void Arena::procesarEntrada(sf::RenderWindow& ventana) {
    float dt = relojArena.restart().asSeconds();

    // Cinemática básica del atacante
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W)) posAtacante.y -= 400.f * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S)) posAtacante.y += 400.f * dt;

    static bool teclaLibre = true;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
        if (teclaLibre) {
            lista_proyectiles.push_back(new Proyectiles(posAtacante.x, posAtacante.y, 10, 8.0f, { 1.f, 0.f }, atacante->getBando()));
            teclaLibre = false;
        }
    }
    else {
        teclaLibre = true;
    }

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) {
        (*it)->mover();

        // Limpieza de proyectiles fuera del rango de la pantalla
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
    // Lógica de colisiones (Pendiente de implementación)
}

void Arena::dibujarPantalla(sf::RenderWindow& ventana) {
    // Restauración del estado de la vista gráfica.
    // Esto garantiza que el sistema de coordenadas sea de 800x800 píxeles,
    // sobrescribiendo cualquier View modificada por otra clase como el Tablero.
    ventana.setView(ventana.getDefaultView());

    // Renderizado del escenario y las entidades
    ventana.draw(*spriteFondoArena);

    if (atacante) {
        atacante->dibujar(ventana, nullptr, 0, 50.f);
    }
    if (defensor) {
        defensor->dibujar(ventana, nullptr, 0, 50.f);
    }

    for (auto p : lista_proyectiles) {
        p->dibujar(ventana);
    }
}