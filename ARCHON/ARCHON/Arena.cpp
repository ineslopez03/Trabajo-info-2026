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

    
    auto calcularMovimientoSeguro = [&](Pieza* pieza, sf::Vector2f posActual, sf::Vector2f dir) -> sf::Vector2f {
        sf::Vector2f posNueva = posActual + sf::Vector2f(dir.x * velocidad * dt, dir.y * velocidad * dt);

        posNueva.x = std::clamp(posNueva.x, margen, 1100.f - margen);
        posNueva.y = std::clamp(posNueva.y, margen, 855.f - margen);

        bool esVoladora = (dynamic_cast<PiezaVoladora*>(pieza) != nullptr);

        if (!esVoladora) {
           
            if (obstaculos.hayColisionCircular(posNueva, 20.f)) {
                return posActual;
            }
        }

        return posNueva;
        };

    sf::Vector2f dirIzq(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dirIzq.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dirIzq.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dirIzq.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dirIzq.x += 1.0f;

    posIzquierda = calcularMovimientoSeguro(piezaIzquierda, posIzquierda, dirIzq);

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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))    dirDer.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))  dirDer.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))  dirDer.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) dirDer.x += 1.0f;

    posDerecha = calcularMovimientoSeguro(piezaDerecha, posDerecha, dirDer);

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

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end(); ) {
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
