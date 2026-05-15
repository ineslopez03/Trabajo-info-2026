#include "MotorArchon.h"
#include "Tablero.h"
#include "Arena.h"
#include "MenuPrincipal.h"
#include <optional>

MotorArchon::MotorArchon() {
    estadoActual = EstadoJuego::MENU;

    pantallaActiva = nullptr;
    jugador1 = nullptr;
    jugador2 = nullptr;
    ejecutando = true;
    miTablero = nullptr;
    skinActual = "ARCHON"; // Valor por defecto
}

MotorArchon::~MotorArchon() {
    if (pantallaActiva != nullptr) delete pantallaActiva;
    if (jugador1 != nullptr) delete jugador1;
    if (jugador2 != nullptr) delete jugador2;
}
void MotorArchon::cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1, Pieza* p2, std::string skinSeleccionada) {
    // SOLO borramos la pantalla si NO es el tablero persistente
    if (pantallaActiva != nullptr && pantallaActiva != miTablero) {
        delete pantallaActiva;
    }
    pantallaActiva = nullptr;

    estadoActual = nuevoEstado;

    if (!skinSeleccionada.empty()) {
        skinActual = skinSeleccionada;
    }

    switch (estadoActual) {
    case EstadoJuego::MENU:
        ventana.setView(ventana.getDefaultView());
        pantallaActiva = new MenuPrincipal();
        break;

    case EstadoJuego::TABLERO:
        if (miTablero == nullptr) {
            miTablero = new Tablero(95.0f, skinActual);
        }
        // Reseteamos la vista por si venimos de la arena (que mide 800)
        ventana.setView(sf::View(sf::FloatRect({ 0.f, 0.f }, { 1100.f, 855.f })));
        pantallaActiva = miTablero;
        break;

    case EstadoJuego::ARENA:
        if (p1 != nullptr && p2 != nullptr) {
            pantallaActiva = new Arena(p1, p2, skinActual,p1);
        }
        break;

    case EstadoJuego::FIN:
        ejecutando = false;
        break;
    }
}

void MotorArchon::bucle() {
    while (ejecutando && ventana.isOpen()) {
        if (pantallaActiva != nullptr) {
            pantallaActiva->procesarEntrada(ventana);

            if (estadoActual == EstadoJuego::MENU) {
                MenuPrincipal* menu = dynamic_cast<MenuPrincipal*>(pantallaActiva);
                if (menu != nullptr && menu->getIniciarJuego()) {
                    cambiarEstado(EstadoJuego::TABLERO, nullptr, nullptr, menu->getSkinSeleccionada());
                }
            }

            if (estadoActual == EstadoJuego::TABLERO) {
                Tablero* tab = dynamic_cast<Tablero*>(pantallaActiva);
                if (tab != nullptr && tab->getHaycombate()) {
                    casillaDestinoCombate = tab->getCoordenadasCombate();
                    Pieza* pAtacante = tab->getAtacante();
                    Pieza* pDefensor = tab->getDefensor();
                    std::string currentSkin = tab->getSkin();
                    int colorSuelo = tab->getColorCasilla(casillaDestinoCombate.x, casillaDestinoCombate.y);
                    int bonoVida = 20.0f;
                    if (colorSuelo == 1) { // Suelo BLANCO (Ventaja Luz)
                        if (pAtacante->getBando() == Bando::LUZ)
                            pAtacante->setVida(pAtacante->getVidaBase() + bonoVida);
                        if (pDefensor->getBando() == Bando::LUZ)
                            pDefensor->setVida(pDefensor->getVidaBase() + bonoVida);
                    }
                    else if (colorSuelo == -1) { // Suelo NEGRO (Ventaja Oscuridad)
                        if (pAtacante->getBando() == Bando::OSCURIDAD)
                            pAtacante->setVida(pAtacante->getVidaBase() + bonoVida);
                        if (pDefensor->getBando() == Bando::OSCURIDAD)
                            pDefensor->setVida(pDefensor->getVidaBase() + bonoVida);
                    }
                    tab->resetCombate();
                    cambiarEstado(EstadoJuego::ARENA, pAtacante, pDefensor, currentSkin);
                }
            }
            
            if (estadoActual == EstadoJuego::ARENA) {
                Arena* arena = dynamic_cast<Arena*>(pantallaActiva);
                if (arena != nullptr) {
                    Pieza* pIzq = arena->getPiezaIzquierda();
                    Pieza* pDer = arena->getPiezaDerecha();

                    // 1. SOLO entramos aquí si alguien murió
                    if (pIzq->getVidaBase() <= 0 || pDer->getVidaBase() <= 0) {
                        Pieza* muerto = (pIzq->getVidaBase() <= 0) ? pIzq : pDer;
                        Pieza* ganador = (pIzq->getVidaBase() <= 0) ? pDer : pIzq;

                        if (miTablero != nullptr) {
                            miTablero->registrarMuerte(muerto);
                            miTablero->eliminarPiezaDelMapa(muerto);

                            // Solo movemos al ganador si era el atacante
                            if (ganador == arena->getPiezaAtacanteReal()) {
                                miTablero->moverPiezaACasilla(ganador, casillaDestinoCombate);
                            }
                            if (ganador->getVidaBase() > ganador->getVidaMaxima()) {
                                ganador->resetVida();
                            }
                        }
                        // 2. EL CAMBIO DE ESTADO DEBE ESTAR AQUÍ DENTRO
                        cambiarEstado(EstadoJuego::TABLERO);
                    }
                }
            }

            ventana.clear();
            pantallaActiva->dibujarPantalla(ventana);
            ventana.display();
        }
    }
}

void MotorArchon::inicializar() {
    ventana.create(sf::VideoMode({ 1100, 855 }), "ARCHON 2026");
    sf::View vistaMenu(sf::FloatRect({ 0.f, 0.f }, { 1100.f, 855.f }));
    ventana.setView(vistaMenu);
    jugador1 = new JugadorHumano("Heroe de la Luz", Bando::LUZ);
    jugador2 = new JugadorIA("Senor de la Oscuridad", Bando::OSCURIDAD);

    cambiarEstado(EstadoJuego::MENU);
}

