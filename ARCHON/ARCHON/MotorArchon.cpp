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
    skinActual = "ARCHON";
}

MotorArchon::~MotorArchon() {
    if (pantallaActiva != nullptr) delete pantallaActiva;
    if (jugador1 != nullptr) delete jugador1;
    if (jugador2 != nullptr) delete jugador2;
}

void MotorArchon::cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1, Pieza* p2, std::string skinSeleccionada) {
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
        ventana.setView(sf::View(sf::FloatRect({ 0.f, 0.f }, { 1100.f, 855.f })));
        pantallaActiva = miTablero;
        break;
    case EstadoJuego::ARENA:
        if (p1 != nullptr && p2 != nullptr) {
            pantallaActiva = new Arena(p1, p2, skinActual, p1);
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

                    ColorCasilla colorSuelo = tab->getColorCasilla(casillaDestinoCombate.x, casillaDestinoCombate.y);
                    int porcentajeBono = 0;
                    Bando bandoFavorecido = Bando::LUZ;

                    if (colorSuelo == ColorCasilla::BLANCO) { porcentajeBono = 40; bandoFavorecido = Bando::LUZ; }
                    else if (colorSuelo == ColorCasilla::GRIS_CLARO) { porcentajeBono = 20; bandoFavorecido = Bando::LUZ; }
                    else if (colorSuelo == ColorCasilla::GRIS_OSCURO) { porcentajeBono = 20; bandoFavorecido = Bando::OSCURIDAD; }
                    else if (colorSuelo == ColorCasilla::NEGRO) { porcentajeBono = 40; bandoFavorecido = Bando::OSCURIDAD; }

                    if (porcentajeBono > 0) {
                        if (pAtacante->getBando() == bandoFavorecido) {
                            pAtacante->aplicarBonoColor(porcentajeBono);
                        }
                        if (pDefensor->getBando() == bandoFavorecido) {
                            pDefensor->aplicarBonoColor(porcentajeBono);
                        }
                    }

                    tab->limpiarBanderaCombate();
                    cambiarEstado(EstadoJuego::ARENA, pAtacante, pDefensor, currentSkin);
                }
            }

            if (estadoActual == EstadoJuego::ARENA) {
                Arena* arena = dynamic_cast<Arena*>(pantallaActiva);
                if (arena != nullptr) {
                    // Refactorización: La comprobación delega el salto al temporizador interno de la Arena
                    if (arena->isTransicionLista()) {
                        Pieza* pIzq = arena->getPiezaIzquierda();
                        Pieza* pDer = arena->getPiezaDerecha();

                        Pieza* muerto = (pIzq->getVidaBase() <= 0) ? pIzq : pDer;
                        Pieza* ganador = (pIzq->getVidaBase() <= 0) ? pDer : pIzq;

                        if (miTablero != nullptr) {
                            miTablero->procesarResultadoCombate(ganador, muerto, arena->getPiezaAtacanteReal());
                        }

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