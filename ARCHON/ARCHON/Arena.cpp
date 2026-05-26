#include "Arena.h"
#include "PiezaVoladora.h" 
#include <iostream>
#include <algorithm> 
#include <cmath> 

Arena::Arena(Pieza* p1, Pieza* p2, const std::string& skin, Pieza* atacante)
    : spriteFondoArena(nullptr), obstaculos(sf::Vector2f(100.f, 427.f), sf::Vector2f(1000.f, 427.f))
{
    this->atacanteOriginal = atacante;
    this->skinArena = skin;
    iniciarBatalla(p1, p2);
    faseCuentaAtras = 3;

    std::string rutaFondo;
    if (skinArena == "HARRY_POTTER") rutaFondo = "imagenes/HP/suelo_hp_2.png";
    else if (skinArena == "STAR_WARS") rutaFondo = "imagenes/SW/suelo_sw_2.png";
    else rutaFondo = "imagenes/CLASSIC/suelo_clss_2.png";

    if (!texturaFondoArena.loadFromFile(rutaFondo)) {
        std::cerr << "Error cargando la textura de fondo de la Arena" << std::endl;
    }

    spriteFondoArena = new sf::Sprite(texturaFondoArena);
    spriteFondoArena->setScale(sf::Vector2f(1100.f / texturaFondoArena.getSize().x, 855.f / texturaFondoArena.getSize().y));
}

Arena::~Arena() {
    for (auto p : lista_proyectiles) delete p;
    lista_proyectiles.clear();
    delete spriteFondoArena;
}

void Arena::iniciarBatalla(Pieza* p1, Pieza* p2) {
    if (p1->getBando() == Bando::LUZ) {
        piezaIzquierda = p1;
        piezaDerecha = p2;
    }
    else {
        piezaIzquierda = p2;
        piezaDerecha = p1;
    }

    posIzquierda = sf::Vector2f(100.f, 427.f);
    posDerecha = sf::Vector2f(1000.f, 400.f);
    obstaculos.reiniciar(posIzquierda, posDerecha);
}

void Arena::procesarEntrada(sf::RenderWindow& ventana) {
    if (faseCuentaAtras >= 0) {
        float tiempoTranscurrido = relojCuentaAtras.getElapsedTime().asSeconds();
        if (tiempoTranscurrido >= 1.0f) {
            faseCuentaAtras--;
            relojCuentaAtras.restart();
        }
        relojArena.restart();
        return;
    }

    float dt = relojArena.restart().asSeconds();
    obstaculos.actualizar(dt, posIzquierda, posDerecha);

    const float velocidad = 400.f;
    const float margen = 30.f;

    // Refactorización: Motor de físicas reactivo para colisiones curvas
    auto aplicarFisica = [&](Pieza* pieza, sf::Vector2f& pos, sf::Vector2f dir, sf::Vector2f posEnemigo) {
        // 1. Movimiento libre inicial (Integración de Euler)
        pos.x += dir.x * velocidad * dt;
        pos.y += dir.y * velocidad * dt;

        // 2. Colisión con los límites de la ventana (cajas planas)
        pos.x = std::clamp(pos.x, margen, 1100.f - margen);
        pos.y = std::clamp(pos.y, margen, 855.f - margen);

        // 3. Resolución reactiva contra los obstáculos del mapa
        bool esVoladora = (dynamic_cast<PiezaVoladora*>(pieza) != nullptr);
        if (!esVoladora) {
            obstaculos.expulsarDeColision(pos, 20.f);
        }

        // 4. Resolución reactiva de contacto cuerpo a cuerpo con el rival
        float radioHitboxCuerpo = 30.f;
        float dx = pos.x - posEnemigo.x;
        float dy = pos.y - posEnemigo.y;
        float distancia = std::hypot(dx, dy);
        float radioCombinado = radioHitboxCuerpo * 2.f;

        if (distancia < radioCombinado && distancia > 0.0001f) {
            float solapamiento = radioCombinado - distancia;
            // Deslizamiento vectorial del cuerpo al chocar entre sí
            pos.x += (dx / distancia) * solapamiento;
            pos.y += (dy / distancia) * solapamiento;
        }
        };

    sf::Vector2f dirIzq(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dirIzq.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dirIzq.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dirIzq.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dirIzq.x += 1.0f;

    // Se modifica la variable directamente por referencia
    aplicarFisica(piezaIzquierda, posIzquierda, dirIzq, posDerecha);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2)) {
        if (teclaDisparoIzquierdaLibre) {
            lista_proyectiles.push_back(new Proyectiles(posIzquierda.x, posIzquierda.y, 10, 600.0f, { 1.f, 0.f }, piezaIzquierda->getBando(), skinArena));
            teclaDisparoIzquierdaLibre = false;
        }
    }
    else {
        teclaDisparoIzquierdaLibre = true;
    }

    sf::Vector2f dirDer(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) dirDer.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) dirDer.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) dirDer.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) dirDer.x += 1.0f;

    // Se modifica la variable directamente por referencia
    aplicarFisica(piezaDerecha, posDerecha, dirDer, posIzquierda);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad0)) {
        if (teclaDisparoDerechaLibre) {
            lista_proyectiles.push_back(new Proyectiles(posDerecha.x, posDerecha.y, 10, 600.0f, { -1.f, 0.f }, piezaDerecha->getBando(), skinArena));
            teclaDisparoDerechaLibre = false;
        }
    }
    else {
        teclaDisparoDerechaLibre = true;
    }

    gestionarColisiones();

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) {
        (*it)->mover(dt);
        if ((*it)->getPosicion().x > 1100 || (*it)->getPosicion().x < 0) {
            delete* it;
            it = lista_proyectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Arena::gestionarColisiones() {
    float radioHitbox = 35.f;

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) {
        bool proyectilDestruido = false;
        sf::Vector2f posP = (*it)->getPosicion();

        if (obstaculos.hayColisionCircular(posP, 5.f)) {
            proyectilDestruido = true;
        }

        if (!proyectilDestruido && piezaDerecha && (*it)->getBando() != piezaDerecha->getBando()) {
            if (std::hypot(posP.x - posDerecha.x, posP.y - posDerecha.y) < radioHitbox) {
                piezaDerecha->recibirDanyo((*it)->getDanyo());
                proyectilDestruido = true;
            }
        }

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
    obstaculos.dibujar(ventana);

    if (piezaIzquierda) piezaIzquierda->dibujarEnArena(ventana, posIzquierda, true, skinArena);
    if (piezaDerecha) piezaDerecha->dibujarEnArena(ventana, posDerecha, false, skinArena);

    for (auto p : lista_proyectiles) p->dibujar(ventana);

    float rIzq = 0.f;
    if (piezaIzquierda) {
        rIzq = (float)piezaIzquierda->getVidaBase() / (float)piezaIzquierda->getVidaMaximaOriginal();
        if (rIzq < 0.0f) rIzq = 0.0f;
    }

    float rDer = 0.f;
    if (piezaDerecha) {
        rDer = (float)piezaDerecha->getVidaBase() / (float)piezaDerecha->getVidaMaximaOriginal();
        if (rDer < 0.0f) rDer = 0.0f;
    }

    graficos.actualizar(rIzq, rDer, faseCuentaAtras);
    graficos.dibujar(ventana, faseCuentaAtras >= 0);
}