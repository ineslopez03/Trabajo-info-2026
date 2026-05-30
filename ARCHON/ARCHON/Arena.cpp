#include "Arena.h"
#include <iostream>
#include <algorithm> 
#include <cmath> 
#include <typeinfo>

Arena::Arena(Pieza* p1, Pieza* p2, const std::string& skin, Pieza* atacante)
    : spriteFondoArena(nullptr), obstaculos(sf::Vector2f(100.f, 427.f), sf::Vector2f(1000.f, 427.f)), textoVictoria(fuenteVictoria)
{
    this->atacanteOriginal = atacante;
    this->skinArena = skin;
    iniciarBatalla(p1, p2);
    faseCuentaAtras = 3;

    // Conexión del nuevo sistema de inyección visual
    graficos.vincularPiezas(piezaIzquierda, piezaDerecha);

    std::string rutaFondo;
    if (skinArena == "HARRY_POTTER") rutaFondo = "imagenes/HP/suelo_hp_2.png";
    else if (skinArena == "STAR_WARS") rutaFondo = "imagenes/SW/suelo_sw_2.png";
    else rutaFondo = "imagenes/CLASSIC/suelo_clss_2.png";

    if (!texturaFondoArena.loadFromFile(rutaFondo)) {
        std::cerr << "Error cargando la textura de fondo de la Arena" << std::endl;
    }

    spriteFondoArena = new sf::Sprite(texturaFondoArena);
    spriteFondoArena->setScale(sf::Vector2f(1100.f / texturaFondoArena.getSize().x, 855.f / texturaFondoArena.getSize().y));

    if (!fuenteVictoria.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "Error cargando fuente para victoria" << std::endl;
    }

    textoVictoria.setCharacterSize(60);
    textoVictoria.setOutlineThickness(4.f);
    textoVictoria.setOutlineColor(sf::Color::Black);
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

    if (combateFinalizado) {
        temporizadorSalida -= dt;
        return;
    }

    obstaculos.actualizar(dt, posIzquierda, posDerecha);
    MotorFisicasArena::propagarOndas(lista_ondas, dt);

    if (tiempoRestanteCooldownIzq > 0.f) tiempoRestanteCooldownIzq -= dt;
    if (tiempoRestanteCooldownDer > 0.f) tiempoRestanteCooldownDer -= dt;

    // --- PROCESAMIENTO MODULAR FLANCO IZQUIERDO ---
    sf::Vector2f dirIzq(0.f, 0.f);
    if (tiempoRestanteCooldownIzq <= 0.f) {
        IntencionJugador intIzq = ControladorPelea::obtenerIntencionIzquierda();
        dirIzq = intIzq.direccionMovimiento;

        if (intIzq.intentandoAtacar) {
            if (teclaDisparoIzquierdaLibre) {
                if (piezaIzquierda->esCuerpoACuerpo()) {
                    MotorFisicasArena::generarOndaChoque(posIzquierda, piezaIzquierda->getBando(), lista_ondas);
                    if (std::hypot(posIzquierda.x - posDerecha.x, posIzquierda.y - posDerecha.y) <= 95.f) {
                        piezaDerecha->recibirDanyo(piezaIzquierda->getDanio());
                    }
                }
                else {
                    sf::Vector2f dirProyectil = MotorFisicasArena::calcularDireccionOrtogonal(posIzquierda, posDerecha);
                    lista_proyectiles.push_back(new Proyectiles(posIzquierda.x, posIzquierda.y, piezaIzquierda->getDanio(), 600.0f, dirProyectil, piezaIzquierda->getBando(), skinArena));
                }
                teclaDisparoIzquierdaLibre = false;
                tiempoRestanteCooldownIzq = 1.4f - (piezaIzquierda->getVelAta() * 0.2f);
            }
        }
        else {
            teclaDisparoIzquierdaLibre = true;
        }
    }
    MotorFisicasArena::aplicarFisicaMovimiento(piezaIzquierda, posIzquierda, dirIzq, posDerecha, dt, obstaculos);

    // --- PROCESAMIENTO MODULAR FLANCO DERECHO ---
    sf::Vector2f dirDer(0.f, 0.f);
    if (tiempoRestanteCooldownDer <= 0.f) {
        IntencionJugador intDer = ControladorPelea::obtenerIntencionDerecha();
        dirDer = intDer.direccionMovimiento;

        if (intDer.intentandoAtacar) {
            if (teclaDisparoDerechaLibre) {
                if (piezaDerecha->esCuerpoACuerpo()) {
                    MotorFisicasArena::generarOndaChoque(posDerecha, piezaDerecha->getBando(), lista_ondas);
                    if (std::hypot(posDerecha.x - posIzquierda.x, posDerecha.y - posIzquierda.y) <= 95.f) {
                        piezaIzquierda->recibirDanyo(piezaDerecha->getDanio());
                    }
                }
                else {
                    sf::Vector2f dirProyectil = MotorFisicasArena::calcularDireccionOrtogonal(posDerecha, posIzquierda);
                    lista_proyectiles.push_back(new Proyectiles(posDerecha.x, posDerecha.y, piezaDerecha->getDanio(), 600.0f, dirProyectil, piezaDerecha->getBando(), skinArena));
                }
                teclaDisparoDerechaLibre = false;
                tiempoRestanteCooldownDer = 1.4f - (piezaDerecha->getVelAta() * 0.2f);
            }
        }
        else {
            teclaDisparoDerechaLibre = true;
        }
    }
    MotorFisicasArena::aplicarFisicaMovimiento(piezaDerecha, posDerecha, dirDer, posIzquierda, dt, obstaculos);

    gestionarColisiones(); // Mantenemos esta rutina de barrido interno unida al array destructivo

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) {
        (*it)->mover(dt);
        if ((*it)->getPosicion().x > 1100 || (*it)->getPosicion().x < 0 || (*it)->getPosicion().y > 855 || (*it)->getPosicion().y < 0) {
            delete* it;
            it = lista_proyectiles.erase(it);
        }
        else {
            ++it;
        }
    }

    if (!combateFinalizado && (piezaIzquierda->getVidaBase() <= 0 || piezaDerecha->getVidaBase() <= 0)) {
        combateFinalizado = true;
        if (piezaIzquierda->getVidaBase() <= 0) {
            textoVictoria.setString("GANADOR BANDO DE OSCURIDAD");
            textoVictoria.setFillColor(sf::Color::Red);
        }
        else {
            textoVictoria.setString("GANADOR BANDO DE LUZ");
            textoVictoria.setFillColor(sf::Color::Cyan);
        }

        sf::FloatRect limitesTexto = textoVictoria.getLocalBounds();
        textoVictoria.setOrigin({ limitesTexto.size.x / 2.f, limitesTexto.size.y / 2.f });
        textoVictoria.setPosition({ 550.f, 427.f });
    }
}

void Arena::gestionarColisiones() {
    float radioHitbox = 35.f;
    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) {
        bool proyectilDestruido = false;
        sf::Vector2f posP = (*it)->getPosicion();

        if (obstaculos.hayColisionCircular(posP, 5.f)) proyectilDestruido = true;

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
        else ++it;
    }
}

void Arena::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.setView(ventana.getDefaultView());
    ventana.draw(*spriteFondoArena);
    obstaculos.dibujar(ventana);

    for (const auto& onda : lista_ondas) ventana.draw(onda.forma);

    auto dibujarAuraDanyo = [&](Pieza* p, sf::Vector2f pos) {
        if (p && p->estaParpadeandoDanyo()) {
            sf::CircleShape auraImpacto(45.f);
            auraImpacto.setOrigin({ 45.f, 45.f });
            auraImpacto.setPosition(pos);
            auraImpacto.setFillColor(sf::Color(255, 0, 0, 130));
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

    // La interfaz ahora gestiona su telemetría matemáticamente en base a las piezas vinculadas
    graficos.actualizar(faseCuentaAtras);
    graficos.dibujar(ventana, faseCuentaAtras >= 0);

    if (combateFinalizado) ventana.draw(textoVictoria);
}