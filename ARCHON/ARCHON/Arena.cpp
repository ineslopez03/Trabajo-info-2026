#include "Arena.h"
#include <iostream>
#include <algorithm> 
#include <cmath> // Necesario para std::hypot

Arena::Arena(Pieza* p1, Pieza* p2, const std::string& skin)
    : spriteFondoArena(nullptr), textoCuentaAtras(fuenteArena),
    textoEtiquetaIzq(fuenteArena), textoEtiquetaDer(fuenteArena)
{
    this->skinArena = skin;
    iniciarBatalla(p1, p2);

    std::string rutaFondo;
    if (skinArena == "HARRY_POTTER") rutaFondo = "imagenes/HP/suelo_hp_2.png";
    else if (skinArena == "STAR_WARS") rutaFondo = "imagenes/SW/suelo_sw_2.png";
    else rutaFondo = "imagenes/CLASSIC/suelo_clss_2.png";

    if (!texturaFondoArena.loadFromFile(rutaFondo)) std::cerr << "Error fondo Arena" << std::endl;

    spriteFondoArena = new sf::Sprite(texturaFondoArena);
    spriteFondoArena->setScale(sf::Vector2f(800.f / texturaFondoArena.getSize().x, 800.f / texturaFondoArena.getSize().y));

    if (!fuenteArena.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) std::cerr << "Error fuente Arena" << std::endl;

    textoCuentaAtras.setCharacterSize(150);
    textoCuentaAtras.setFillColor(sf::Color(255, 215, 0));
    textoCuentaAtras.setOutlineThickness(6.f);
    textoCuentaAtras.setOutlineColor(sf::Color::Black);
    faseCuentaAtras = 3;

    sf::Vector2f tamanoBarra(250.f, 25.f);

    textoEtiquetaIzq.setString("JUGADOR 1");
    textoEtiquetaIzq.setCharacterSize(20);
    textoEtiquetaIzq.setFillColor(sf::Color::White);
    textoEtiquetaIzq.setPosition({ 30.f, 15.f });

    textoEtiquetaDer.setString("JUGADOR 2");
    textoEtiquetaDer.setCharacterSize(20);
    textoEtiquetaDer.setFillColor(sf::Color::White);
    textoEtiquetaDer.setPosition({ 770.f - textoEtiquetaDer.getLocalBounds().size.x, 15.f });

    marcoIzq.setSize(tamanoBarra);
    marcoIzq.setFillColor(sf::Color::Transparent);
    marcoIzq.setOutlineThickness(3.f);
    marcoIzq.setOutlineColor(sf::Color(200, 200, 200));
    marcoIzq.setPosition({ 30.f, 45.f });

    marcoDer.setSize(tamanoBarra);
    marcoDer.setFillColor(sf::Color::Transparent);
    marcoDer.setOutlineThickness(3.f);
    marcoDer.setOutlineColor(sf::Color(200, 200, 200));
    marcoDer.setPosition({ 520.f, 45.f });

    barraFondoIzq.setSize(tamanoBarra);
    barraFondoIzq.setFillColor(sf::Color(80, 20, 20, 200));
    barraFondoIzq.setPosition({ 30.f, 45.f });

    barraFondoDer.setSize(tamanoBarra);
    barraFondoDer.setFillColor(sf::Color(80, 20, 20, 200));
    barraFondoDer.setPosition({ 520.f, 45.f });

    barraVidaIzq.setSize(tamanoBarra);
    barraVidaIzq.setFillColor(sf::Color(50, 205, 50));
    barraVidaIzq.setPosition({ 30.f, 45.f });

    barraVidaDer.setSize(tamanoBarra);
    barraVidaDer.setFillColor(sf::Color(50, 205, 50));
    barraVidaDer.setPosition({ 520.f, 45.f });
}

Arena::~Arena() {
    for (auto p : lista_proyectiles) delete p;
    lista_proyectiles.clear();
    delete spriteFondoArena;
}

void Arena::iniciarBatalla(Pieza* p1, Pieza* p2) {
    if (p1->getBando() == Bando::LUZ) { piezaIzquierda = p1; piezaDerecha = p2; }
    else { piezaIzquierda = p2; piezaDerecha = p1; }
    posIzquierda = sf::Vector2f(100.f, 400.f);
    posDerecha = sf::Vector2f(700.f, 400.f);
}

void Arena::procesarEntrada(sf::RenderWindow& ventana) {
    if (faseCuentaAtras >= 0) {
        float tiempoTranscurrido = relojCuentaAtras.getElapsedTime().asSeconds();
        if (tiempoTranscurrido >= 1.0f) {
            faseCuentaAtras--;
            relojCuentaAtras.restart();
        }

        if (faseCuentaAtras > 0) textoCuentaAtras.setString(std::to_string(faseCuentaAtras));
        else if (faseCuentaAtras == 0) textoCuentaAtras.setString("YA"); // Modificación aplicada

        sf::FloatRect limites = textoCuentaAtras.getLocalBounds();
        textoCuentaAtras.setOrigin({ limites.size.x / 2.0f, limites.size.y / 2.0f });
        textoCuentaAtras.setPosition({ 400.f, 400.f });

        relojArena.restart();
        return;
    }

    float dt = relojArena.restart().asSeconds();
    const float velocidad = 400.f;
    const float margen = 30.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) posIzquierda.y -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) posIzquierda.y += velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) posIzquierda.x -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) posIzquierda.x += velocidad * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2)) {
        if (teclaDisparoIzquierdaLibre) {
            // Velocidad del proyectil ajustada a 600.f
            lista_proyectiles.push_back(new Proyectiles(posIzquierda.x, posIzquierda.y, 10, 600.0f, { 1.f, 0.f }, piezaIzquierda->getBando(), skinArena));
            teclaDisparoIzquierdaLibre = false;
        }
    }
    else {
        teclaDisparoIzquierdaLibre = true;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    posDerecha.y -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  posDerecha.y += velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  posDerecha.x -= velocidad * dt;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) posDerecha.x += velocidad * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad0)) {
        if (teclaDisparoDerechaLibre) {
            lista_proyectiles.push_back(new Proyectiles(posDerecha.x, posDerecha.y, 10, 600.0f, { -1.f, 0.f }, piezaDerecha->getBando(), skinArena));
            teclaDisparoDerechaLibre = false;
        }
    }
    else {
        teclaDisparoDerechaLibre = true;
    }

    posIzquierda.x = std::clamp(posIzquierda.x, margen, 800.f - margen);
    posIzquierda.y = std::clamp(posIzquierda.y, margen, 800.f - margen);
    posDerecha.x = std::clamp(posDerecha.x, margen, 800.f - margen);
    posDerecha.y = std::clamp(posDerecha.y, margen, 800.f - margen);

    gestionarColisiones();

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) {
        (*it)->mover(dt); // Movimiento integrado con delta time
        if ((*it)->getPosicion().x > 800 || (*it)->getPosicion().x < 0) {
            delete* it;
            it = lista_proyectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Arena::gestionarColisiones() {
    float radioHitbox = 35.f; // Aproximación circular eficiente

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end(); ) {
        bool proyectilDestruido = false;
        sf::Vector2f posP = (*it)->getPosicion();

        // Verificación contra la pieza Derecha
        if (piezaDerecha && (*it)->getBando() != piezaDerecha->getBando()) {
            if (std::hypot(posP.x - posDerecha.x, posP.y - posDerecha.y) < radioHitbox) {
                piezaDerecha->recibirDanyo((*it)->getDanyo());
                proyectilDestruido = true;
            }
        }

        // Verificación contra la pieza Izquierda
        if (!proyectilDestruido && piezaIzquierda && (*it)->getBando() != piezaIzquierda->getBando()) {
            if (std::hypot(posP.x - posIzquierda.x, posP.y - posIzquierda.y) < radioHitbox) {
                piezaIzquierda->recibirDanyo((*it)->getDanyo());
                proyectilDestruido = true;
            }
        }

        if (proyectilDestruido) {
            delete* it;
            it = lista_proyectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Arena::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.setView(ventana.getDefaultView());
    ventana.draw(*spriteFondoArena);

    if (piezaIzquierda) piezaIzquierda->dibujarEnArena(ventana, posIzquierda, true, skinArena);
    if (piezaDerecha) piezaDerecha->dibujarEnArena(ventana, posDerecha, false, skinArena);

    for (auto p : lista_proyectiles) p->dibujar(ventana);

    if (piezaIzquierda) {
        float ratioIzq = std::max(0.0f, (float)piezaIzquierda->getVidaBase() / piezaIzquierda->getVidaMaxima());
        barraVidaIzq.setSize({ 250.f * ratioIzq, 25.f });
        ventana.draw(textoEtiquetaIzq);
        ventana.draw(barraFondoIzq);
        ventana.draw(barraVidaIzq);
        ventana.draw(marcoIzq);
    }

    if (piezaDerecha) {
        float ratioDer = std::max(0.0f, (float)piezaDerecha->getVidaBase() / piezaDerecha->getVidaMaxima());
        barraVidaDer.setSize({ 250.f * ratioDer, 25.f });
        ventana.draw(textoEtiquetaDer);
        ventana.draw(barraFondoDer);
        ventana.draw(barraVidaDer);
        ventana.draw(marcoDer);
    }

    if (faseCuentaAtras >= 0) ventana.draw(textoCuentaAtras);
}