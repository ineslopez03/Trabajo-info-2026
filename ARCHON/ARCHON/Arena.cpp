#include "Arena.h"
#include "PiezaVoladora.h" 
#include <iostream>
#include <algorithm> 
#include <cmath> 
#include <typeinfo> // Inyección de librería para evaluación de clases en tiempo real

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
    lista_ondas.clear();
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

    // RTTI: Analizador léxico para forzar el estado Cuerpo a Cuerpo por herencia sin tocar archivos
    auto forzarMelee = [](Pieza* p) {
        std::string nombreClase = typeid(*p).name();
        if (nombreClase.find("Caballero") != std::string::npos ||
            nombreClase.find("Golem") != std::string::npos ||
            nombreClase.find("Goblin") != std::string::npos ||
            nombreClase.find("Troll") != std::string::npos) {
            p->setAtaqueCuerpoACuerpo(true);
        }
        };

    forzarMelee(piezaIzquierda);
    forzarMelee(piezaDerecha);

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

    if (tiempoRestanteCooldownIzq > 0.f) tiempoRestanteCooldownIzq -= dt;
    if (tiempoRestanteCooldownDer > 0.f) tiempoRestanteCooldownDer -= dt;

    // Procesamiento de las ondas visuales de impacto
    for (auto it = lista_ondas.begin(); it != lista_ondas.end();) {
        it->radio += 300.f * dt; // Tasa de expansión radial
        it->opacidad -= 600.f * dt; // Tasa de difuminación

        if (it->opacidad <= 0.f || it->radio >= 85.f) {
            it = lista_ondas.erase(it);
        }
        else {
            it->forma.setRadius(it->radio);
            // Corrección C2660: Se encapsulan los argumentos en un sf::Vector2f implícito
            it->forma.setOrigin({ it->radio, it->radio });
            it->forma.setOutlineColor(sf::Color(it->colorBando.r, it->colorBando.g, it->colorBando.b, std::max(0, (int)it->opacidad)));
            ++it;
        }
    }

    const float margen = 30.f;

    auto aplicarFisica = [&](Pieza* pieza, sf::Vector2f& pos, sf::Vector2f dir, sf::Vector2f posEnemigo) {
        float velocidadPx = pieza->getVelMov() * 80.f;

        pos.x += dir.x * velocidadPx * dt;
        pos.y += dir.y * velocidadPx * dt;

        pos.x = std::clamp(pos.x, margen, 1100.f - margen);
        pos.y = std::clamp(pos.y, margen, 855.f - margen);

        bool esVoladora = (dynamic_cast<PiezaVoladora*>(pieza) != nullptr);
        if (!esVoladora) {
            obstaculos.expulsarDeColision(pos, 20.f);
        }

        float radioHitboxCuerpo = 30.f;
        float dx = pos.x - posEnemigo.x;
        float dy = pos.y - posEnemigo.y;
        float distancia = std::hypot(dx, dy);
        float radioCombinado = radioHitboxCuerpo * 2.f;

        if (distancia < radioCombinado && distancia > 0.0001f) {
            float solapamiento = radioCombinado - distancia;
            pos.x += (dx / distancia) * solapamiento;
            pos.y += (dy / distancia) * solapamiento;
        }
        };

    auto generarOndaChoque = [&](sf::Vector2f pos, Bando b) {
        EfectoOnda onda;
        onda.radio = 20.f;
        onda.opacidad = 255.f;
        onda.forma.setFillColor(sf::Color::Transparent);
        onda.forma.setOutlineThickness(4.f);
        onda.colorBando = (b == Bando::LUZ) ? sf::Color::Cyan : sf::Color::Red;
        onda.forma.setPosition(pos);
        lista_ondas.push_back(onda);
        };

    sf::Vector2f dirIzq(0.f, 0.f);
    if (tiempoRestanteCooldownIzq <= 0.f) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dirIzq.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dirIzq.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dirIzq.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dirIzq.x += 1.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2)) {
            if (teclaDisparoIzquierdaLibre) {
                if (piezaIzquierda->esCuerpoACuerpo()) {
                    generarOndaChoque(posIzquierda, piezaIzquierda->getBando());
                    float distanciaAlObjetivo = std::hypot(posIzquierda.x - posDerecha.x, posIzquierda.y - posDerecha.y);
                    if (distanciaAlObjetivo <= 85.f) {
                        piezaDerecha->recibirDanyo(piezaIzquierda->getDanio());
                    }
                }
                else {
                    lista_proyectiles.push_back(new Proyectiles(posIzquierda.x, posIzquierda.y, piezaIzquierda->getDanio(), 600.0f, { 1.f, 0.f }, piezaIzquierda->getBando(), skinArena));
                }

                teclaDisparoIzquierdaLibre = false;
                tiempoRestanteCooldownIzq = 1.4f - (piezaIzquierda->getVelAta() * 0.2f);
            }
        }
        else {
            teclaDisparoIzquierdaLibre = true;
        }
    }
    aplicarFisica(piezaIzquierda, posIzquierda, dirIzq, posDerecha);

    sf::Vector2f dirDer(0.f, 0.f);
    if (tiempoRestanteCooldownDer <= 0.f) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) dirDer.y -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) dirDer.y += 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) dirDer.x -= 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) dirDer.x += 1.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad0)) {
            if (teclaDisparoDerechaLibre) {
                if (piezaDerecha->esCuerpoACuerpo()) {
                    generarOndaChoque(posDerecha, piezaDerecha->getBando());
                    float distanciaAlObjetivo = std::hypot(posDerecha.x - posIzquierda.x, posDerecha.y - posIzquierda.y);
                    if (distanciaAlObjetivo <= 85.f) {
                        piezaIzquierda->recibirDanyo(piezaDerecha->getDanio());
                    }
                }
                else {
                    lista_proyectiles.push_back(new Proyectiles(posDerecha.x, posDerecha.y, piezaDerecha->getDanio(), 600.0f, { -1.f, 0.f }, piezaDerecha->getBando(), skinArena));
                }

                teclaDisparoDerechaLibre = false;
                tiempoRestanteCooldownDer = 1.4f - (piezaDerecha->getVelAta() * 0.2f);
            }
        }
        else {
            teclaDisparoDerechaLibre = true;
        }
    }
    aplicarFisica(piezaDerecha, posDerecha, dirDer, posIzquierda);

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

    // Renderizado de ondas expansivas en el suelo
    for (const auto& onda : lista_ondas) {
        ventana.draw(onda.forma);
    }

    // Método auxiliar para garantizar la visibilidad del daño crítico (Fondo sangriento)
    auto dibujarAuraDanyo = [&](Pieza* p, sf::Vector2f pos) {
        if (p && p->estaParpadeandoDanyo()) {
            sf::CircleShape auraImpacto(45.f);
            // Corrección C2660: Se encapsulan los argumentos en un sf::Vector2f implícito
            auraImpacto.setOrigin({ 45.f, 45.f });
            auraImpacto.setPosition(pos);
            auraImpacto.setFillColor(sf::Color(255, 0, 0, 130)); // Resplandor rojo base
            ventana.draw(auraImpacto);
        }
        };

    if (piezaIzquierda) {
        dibujarAuraDanyo(piezaIzquierda, posIzquierda);
        piezaIzquierda->procesarEfectoVisual();
        piezaIzquierda->dibujarEnArena(ventana, posIzquierda, true, skinArena);
    }
    if (piezaDerecha) {
        dibujarAuraDanyo(piezaDerecha, posDerecha);
        piezaDerecha->procesarEfectoVisual();
        piezaDerecha->dibujarEnArena(ventana, posDerecha, false, skinArena);
    }

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