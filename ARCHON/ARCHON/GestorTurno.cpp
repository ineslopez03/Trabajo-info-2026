#include "GestorTurno.h"
#include "Tablero.h"
#include "Hechicero.h"
#include <iostream>

void GestorTurno::gestionarTurno(Casilla* casillaClicada, Tablero* tablero) {
    if (!tablero->primerClicRealizado) {
        // PRIMER CLIC: intenta seleccionar una pieza propia
        Pieza* p = casillaClicada->getPieza();
        if (p != nullptr && p->getBando() == tablero->turnoActual) {
            if (p->estaEncarcelada()) return; // pieza bloqueada por Imprison, no puede moverse

            tablero->origenSeleccionado = casillaClicada;
            tablero->primerClicRealizado = true;
            // Calcula y guarda las casillas a las que puede moverse esta pieza
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    Casilla* destino = tablero->matriz[i][j];
                    if (destino == casillaClicada) continue;// excluye la casilla propia
                    if (tablero->esMovimientoValido(casillaClicada, destino)) {
                        tablero->casillasValidas.push_back(destino);
                    }
                }
            }
            // Si es un Hechicero, avisa por consola si ya gastó la magia este turno
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
        // SEGUNDO CLIC: actúa sobre la casilla destino
        if (tablero->modoHechizoActivo) {
            // Hay un hechizo activo: el clic es el objetivo del hechizo
            tablero->procesarMagia(casillaClicada);
            return;
        }

        if (tablero->origenSeleccionado == casillaClicada) {
            // Clic en la misma casilla → cancela la selección y limpia el resaltado
            tablero->primerClicRealizado = false;
            tablero->origenSeleccionado = nullptr;
            tablero->casillasValidas.clear();
            return;
        }

        if (tablero->esMovimientoValido(tablero->origenSeleccionado, casillaClicada)) {
            if (casillaClicada->estaOcupada()) {
                // Casilla con enemigo → prepara el combate
                tablero->coordenadasCombate = sf::Vector2i(casillaClicada->getX(), casillaClicada->getY());
                tablero->atacante = tablero->origenSeleccionado->getPieza();
                tablero->defensor = casillaClicada->getPieza();
                // Aplica el bono de color antes de lanzar la Arena
                // Blanco: +40% Luz | Negro: +40% Oscuridad | Grises: sin bono
                ColorCasilla colorSuelo = tablero->getColorCasilla(casillaClicada->getX(), casillaClicada->getY());
                int porcentajeBono = 0;
                Bando bandoFavorecido = Bando::LUZ;

                if (colorSuelo == ColorCasilla::BLANCO) { porcentajeBono = 40; bandoFavorecido = Bando::LUZ; }
                else if (colorSuelo == ColorCasilla::NEGRO) { porcentajeBono = 40; bandoFavorecido = Bando::OSCURIDAD; }

                if (porcentajeBono > 0) {
                    if (tablero->atacante->getBando() == bandoFavorecido) tablero->atacante->aplicarBonoColor(porcentajeBono);
                    if (tablero->defensor->getBando() == bandoFavorecido) tablero->defensor->aplicarBonoColor(porcentajeBono);
                }
                // MotorArchon detectará hayCombatePendiente y lanzará la Arena
                tablero->hayCombatePendiente = true;
                tablero->primerClicRealizado = false;
                tablero->origenSeleccionado = nullptr;
                tablero->casillasValidas.clear();
                std::cout << "¡Combate iniciado! Destino guardado." << std::endl;
            }
            else {
                // Casilla vacía → mueve la pieza directamente y pasa el turno
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
            Pieza* p = tablero->matriz[i][j]->getPieza();
            if (p != nullptr) {
                // Curación pasiva: las piezas en casillas de su color se curan 5 puntos por turno
                if (tablero->getColorCasilla(i, j) == (p->getBando() == Bando::LUZ ? ColorCasilla::BLANCO : ColorCasilla::NEGRO)) {
                    p->curar(5);
                }
                p->pasarTurnoBloqueo();// descuenta un turno de bloqueo si está encarcelada
            }
        }
    }
    tablero->turnoActual = (tablero->turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
    tablero->turnosContados++; // avanza el ciclo de colores del tablero
    tablero->primerClicRealizado = false;
    tablero->origenSeleccionado = nullptr;
    tablero->modoHechizoActivo = false;
    tablero->piezaAuxiliar = nullptr;
    tablero->casillasValidas.clear();// limpia el resaltado de movimientos
    std::cout << "Turno finalizado. Turno actual: " << tablero->turnosContados << std::endl;
}