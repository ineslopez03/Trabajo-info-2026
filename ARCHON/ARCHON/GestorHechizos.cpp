#include "GestorHechizos.h"
#include "Tablero.h"
#include "Elemental.h"
#include <cmath>  

void GestorHechizos::ejecutarHechizo(int idHechizo, Casilla* objetivo, Tablero* tablero) {
    Bando bandoLanzador = tablero->turnoActual;
    // guarda el bando antes de cualquier cambio
    switch (idHechizo) {
    case 1:// TELEPORT: mueve una pieza aliada a cualquier casilla vacía del tablero
        if (tablero->piezaAuxiliar == nullptr) {
            // Primer clic: selecciona la pieza aliada a teletransportar
            if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == tablero->turnoActual) {
                tablero->piezaAuxiliar = objetivo;
            }
        }
        else {
            // Segundo clic: coloca la pieza en la casilla vacía destino
            if (!objetivo->estaOcupada()) {
                objetivo->setPieza(tablero->piezaAuxiliar->getPieza());
                tablero->piezaAuxiliar->setPieza(nullptr);
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
            }
        }
        break;

    case 2:// HEAL: restaura la vida máxima de una pieza aliada dañada
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == tablero->turnoActual) {
            Pieza* p = objetivo->getPieza();
            if (p->getVidaBase() < p->getVidaMaxima()) {// solo cura si tiene vida perdida
                p->resetVida();
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
                tablero->piezaAuxiliar = nullptr;
            }
        }
        break;

    case 3:// SHIFT TIME: avanza el ciclo de color del tablero 3 turnos, sin necesitar objetivo
        tablero->turnosContados += 3;
        if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
        else tablero->bandoOscuroUsoMagia = true;
        break;

    case 4:// EXCHANGE: intercambia la posición de dos piezas cualesquiera del tablero
        if (objetivo->estaOcupada()) {
            if (tablero->piezaAuxiliar == nullptr) {
                tablero->piezaAuxiliar = objetivo;// primer clic: guarda la primera pieza
            }
            else {
                // Segundo clic: intercambia las dos piezas
                Pieza* p1 = tablero->piezaAuxiliar->getPieza();
                Pieza* p2 = objetivo->getPieza();
                tablero->piezaAuxiliar->setPieza(p2);
                objetivo->setPieza(p1);
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
                tablero->piezaAuxiliar = nullptr;
            }
        }
        break;

    case 5:// SUMMON: crea un Elemental temporal para combatir contra una pieza enemiga
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() != tablero->turnoActual) {
            tablero->coordenadasCombate = sf::Vector2i(objetivo->getX(), objetivo->getY());
            tablero->atacante = new Elemental(tablero->turnoActual, tablero->skinActual);// Elemental creado en memoria
            tablero->defensor = objetivo->getPieza();
            tablero->hayCombatePendiente = true;// MotorArchon lanzará la Arena
            if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
            else tablero->bandoOscuroUsoMagia = true;
        }
        break;

    case 6:// REVIVE: resucita la última pieza muerta en una casilla adyacente al Hechicero
    {
        std::vector<Pieza*>& cementerio = (tablero->turnoActual == Bando::LUZ) ? tablero->piezasMuertasLuz : tablero->piezasMuertasOscuridad;
        if (!cementerio.empty() && !objetivo->estaOcupada()) {
            int hX = (tablero->origenSeleccionado != nullptr) ? tablero->origenSeleccionado->getX() : -1;
            int hY = (tablero->origenSeleccionado != nullptr) ? tablero->origenSeleccionado->getY() : -1;
            // Solo permite resucitar en casillas a 1 paso del Hechicero (adyacentes y diagonales)
            if (std::abs(objetivo->getX() - hX) <= 1 && std::abs(objetivo->getY() - hY) <= 1) {
                Pieza* pRevivida = cementerio.back();// coge la última pieza muerta
                cementerio.pop_back();
                pRevivida->resetVida(); // restaura su vida máxima
                objetivo->setPieza(pRevivida);// la coloca en el tablero
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
            }
        }
    }
    break;

    case 7:// IMPRISON: bloquea una pieza enemiga durante 2 turnos
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() != tablero->turnoActual) {
            objetivo->getPieza()->setEncarcelada(2);// 2 = turnos que estará bloqueada
            if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
            else tablero->bandoOscuroUsoMagia = true;
        }
        break;
    }
}