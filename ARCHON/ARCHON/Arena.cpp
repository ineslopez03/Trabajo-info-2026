#include "Arena.h"
#include <iostream>
#include <algorithm> // Para std::clamp

Arena::Arena(Pieza* p1, Pieza* p2, const std::string& skin) : spriteFondoArena(nullptr) {
    this->skinArena = skin;
    iniciarBatalla(p1, p2);

    std::string rutaFondo;
    if (skinArena == "HARRY_POTTER") rutaFondo = "imagenes/HP/suelo_hp_2.png";
    else if (skinArena == "STAR_WARS") rutaFondo = "imagenes/SW/suelo_sw_2.png";
    else rutaFondo = "imagenes/CLASSIC/suelo_clss_2.png";

    if (!texturaFondoArena.loadFromFile(rutaFondo)) {
        std::cerr << "Error cargando la textura de fondo" << std::endl;
    }

    spriteFondoArena = new sf::Sprite(texturaFondoArena);
    spriteFondoArena->setScale(sf::Vector2f(800.f / texturaFondoArena.getSize().x, 800.f / texturaFondoArena.getSize().y));
}

Arena::~Arena() {
    for (auto p : lista_proyectiles) delete p;
    lista_proyectiles.clear();
    delete spriteFondoArena;
}

void Arena::iniciarBatalla(Pieza* p1, Pieza* p2) {
    // Lógica de asignación fija: Luz a la izquierda, Oscuridad a la derecha
    if (p1->getBando() == Bando::LUZ) {
        piezaIzquierda = p1;
        piezaDerecha = p2;
    }
    else {
        piezaIzquierda = p2;
        piezaDerecha = p1;
    }

    posIzquierda = sf::Vector2f(100.f, 400.f);
    posDerecha = sf::Vector2f(700.f, 400.f);
}

void Arena::procesarEntrada(sf::RenderWindow& ventana) {
    float dt = relojArena.restart().asSeconds();
    const float velocidad = 400.f;
    const float margen = 30.f; // Margen para que el centro del personaje no toque el borde

    // --- CONTROLES JUGADOR IZQUIERDO (WASD + Espacio) ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) posIzquierda.y -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) posIzquierda.y += velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) posIzquierda.x -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) posIzquierda.x += velocidad * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (teclaDisparoIzquierdaLibre) {
            lista_proyectiles.push_back(new Proyectiles(posIzquierda.x, posIzquierda.y, 10, 8.0f, { 1.f, 0.f }, piezaIzquierda->getBando()));
            teclaDisparoIzquierdaLibre = false;
        }
    }
    else {
        teclaDisparoIzquierdaLibre = true;
    }

    // --- CONTROLES JUGADOR DERECHO (Flechas + Enter) ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    posDerecha.y -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  posDerecha.y += velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  posDerecha.x -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) posDerecha.x += velocidad * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        if (teclaDisparoDerechaLibre) {
            lista_proyectiles.push_back(new Proyectiles(posDerecha.x, posDerecha.y, 10, 8.0f, { -1.f, 0.f }, piezaDerecha->getBando()));
            teclaDisparoDerechaLibre = false;
        }
    }
    else {
        teclaDisparoDerechaLibre = true;
    }

    // --- RESTRICCIÓN DE BORDES (Clamping) ---
    // Limitamos las coordenadas X e Y entre 0 y 800 para ambos personajes
    posIzquierda.x = std::clamp(posIzquierda.x, margen, 800.f - margen);
    posIzquierda.y = std::clamp(posIzquierda.y, margen, 800.f - margen);

    posDerecha.x = std::clamp(posDerecha.x, margen, 800.f - margen);
    posDerecha.y = std::clamp(posDerecha.y, margen, 800.f - margen);

    // --- GESTIÓN DE PROYECTILES ---
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

void Arena::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.setView(ventana.getDefaultView());
    ventana.draw(*spriteFondoArena);

    if (piezaIzquierda) {
        // Mira a la derecha (true)
        piezaIzquierda->dibujarEnArena(ventana, posIzquierda, true, skinArena);
    }
    if (piezaDerecha) {
        // Mira a la izquierda (false)
        piezaDerecha->dibujarEnArena(ventana, posDerecha, false, skinArena);
    }

    for (auto p : lista_proyectiles) p->dibujar(ventana);
}

void Arena::gestionarColisiones() {
    // Implementación pendiente
}