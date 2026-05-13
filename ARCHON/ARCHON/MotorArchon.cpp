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
    skinActual = "ARCHON"; // Valor por defecto
}

MotorArchon::~MotorArchon() {
    if (pantallaActiva != nullptr) delete pantallaActiva;
    if (jugador1 != nullptr) delete jugador1;
    if (jugador2 != nullptr) delete jugador2;
}

void MotorArchon::cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1, Pieza* p2, std::string skinSeleccionada) {
    if (pantallaActiva != nullptr) {
        delete pantallaActiva;
        pantallaActiva = nullptr;
    }

    estadoActual = nuevoEstado;

    if (!skinSeleccionada.empty()) {
        skinActual = skinSeleccionada;
    }

    switch (estadoActual) {
    case EstadoJuego::MENU:
        pantallaActiva = new MenuPrincipal();
        break;

    case EstadoJuego::TABLERO:
        pantallaActiva = new Tablero(95.0f, skinActual);
        break;

    case EstadoJuego::ARENA:
        if (p1 != nullptr && p2 != nullptr) {
            pantallaActiva = new Arena(p1, p2, skinActual);
        }
        else {
            estadoActual = EstadoJuego::TABLERO;
            pantallaActiva = new Tablero();
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
                    Pieza* pAtacante = tab->getAtacante();
                    Pieza* pDefensor = tab->getDefensor();
                    std::string currentSkin = tab->getSkin();
                    tab->resetCombate();
                    cambiarEstado(EstadoJuego::ARENA, pAtacante, pDefensor, currentSkin);
                }
            }

            ventana.clear();
            pantallaActiva->dibujarPantalla(ventana);
            ventana.display();
        }
    }
}

void MotorArchon::inicializar() {
    ventana.create(sf::VideoMode({ 800, 800 }), "ARCHON 2026");

    jugador1 = new JugadorHumano("Heroe de la Luz", Bando::LUZ);
    jugador2 = new JugadorIA("Senor de la Oscuridad", Bando::OSCURIDAD);

    cambiarEstado(EstadoJuego::MENU);
}

