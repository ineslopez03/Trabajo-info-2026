#include "GestorTurno.h"
#include "Tablero.h"
#include "Hechicero.h"
#include <iostream>

void GestorTurno::gestionarTurno(Casilla* casillaClicada, Tablero* tablero) {
    if (!tablero->primerClicRealizado) {
        Pieza* p = casillaClicada->getPieza();
        if (p != nullptr && p->getBando() == tablero->turnoActual) {
            if (p->estaEncarcelada()) return; 

            tablero->origenSeleccionado = casillaClicada;
            tablero->primerClicRealizado = true;
            if (dynamic_cast<Hechicero*>(p) != nullptr) {
                bool* registro = (tablero->turnoActual == Bando::LUZ) ? tablero->hechizosLuzUsados : tablero->hechizosOscurosUsados;
                bool yaUsoMagia = false;
                for (int j = 0; j < 8; j++) { if (registro[j]) { yaUsoMagia = true; break; } }
                if (yaUsoMagia) std::cout << "MAGIA AGOTADA PARA ESTE BANDO." << std::endl;
                else            std::cout << "HECHICERO LISTO: Elige un hechizo de los botones." << std::endl;
            }
        }
    }
    else {

        if (tablero->modoHechizoActivo) {
            tablero->procesarMagia(casillaClicada);
            return;
        }

        if (tablero->origenSeleccionado == casillaClicada) {
            tablero->primerClicRealizado = false;
            tablero->origenSeleccionado = nullptr;
            return;
        }

        if (tablero->esMovimientoValido(tablero->origenSeleccionado, casillaClicada)) {
            if (casillaClicada->estaOcupada()) {
                tablero->coordenadasCombate = sf::Vector2i(casillaClicada->getX(), casillaClicada->getY());
                tablero->atacante = tablero->origenSeleccionado->getPieza();
                tablero->defensor = casillaClicada->getPieza();
                ColorCasilla colorSuelo = tablero->getColorCasilla(casillaClicada->getX(), casillaClicada->getY());
                int porcentajeBono = 0;
                Bando bandoFavorecido = Bando::LUZ;

                if (colorSuelo == ColorCasilla::BLANCO) { porcentajeBono = 40; bandoFavorecido = Bando::LUZ; }
                else if (colorSuelo == ColorCasilla::NEGRO) { porcentajeBono = 40; bandoFavorecido = Bando::OSCURIDAD; }

                if (porcentajeBono > 0) {
                    if (tablero->atacante->getBando() == bandoFavorecido) tablero->atacante->aplicarBonoColor(porcentajeBono);
                    if (tablero->defensor->getBando() == bandoFavorecido) tablero->defensor->aplicarBonoColor(porcentajeBono);
                }

                tablero->hayCombatePendiente = true;
                tablero->primerClicRealizado = false;
                tablero->origenSeleccionado = nullptr;
                std::cout << "¡Combate iniciado! Destino guardado." << std::endl;
            }
            else {
                casillaClicada->setPieza(tablero->origenSeleccionado->getPieza());
                tablero->origenSeleccionado->setPieza(nullptr);
                finalizarTurno(tablero);
            }
        }
    }
}

void GestorTurno::finalizarTurno(Tablero* tablero) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero->matriz[i][j]->estaOcupada()) {
                tablero->matriz[i][j]->getPieza()->pasarTurnoBloqueo();
            }
        }
    }
    tablero->turnoActual = (tablero->turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
    tablero->turnosContados++;
    tablero->primerClicRealizado = false;
    tablero->origenSeleccionado = nullptr;
    tablero->modoHechizoActivo = false;
    tablero->piezaAuxiliar = nullptr;
    std::cout << "Turno finalizado. Turno actual: " << tablero->turnosContados << std::endl;
}