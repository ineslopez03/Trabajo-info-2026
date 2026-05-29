#include "GestorHechizos.h"
#include "Tablero.h"
#include "Elemental.h"
#include <cmath>  

void GestorHechizos::ejecutarHechizo(int idHechizo, Casilla* objetivo, Tablero* tablero) {
    Bando bandoLanzador = tablero->turnoActual;

    switch (idHechizo) {
    case 1:
        if (tablero->piezaAuxiliar == nullptr) {
            if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == tablero->turnoActual) {
                tablero->piezaAuxiliar = objetivo;
            }
        }
        else {
            if (!objetivo->estaOcupada()) {
                objetivo->setPieza(tablero->piezaAuxiliar->getPieza());
                tablero->piezaAuxiliar->setPieza(nullptr);
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
            }
        }
        break;

    case 2:
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == tablero->turnoActual) {
            Pieza* p = objetivo->getPieza();
            if (p->getVidaBase() < p->getVidaMaxima()) {
                p->resetVida();
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
                tablero->piezaAuxiliar = nullptr;
            }
        }
        break;

    case 3:
        tablero->turnosContados += 3;
        if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
        else tablero->bandoOscuroUsoMagia = true;
        break;

    case 4:
        if (objetivo->estaOcupada()) {
            if (tablero->piezaAuxiliar == nullptr) {
                tablero->piezaAuxiliar = objetivo;
            }
            else {
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

    case 5:
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() != tablero->turnoActual) {
            tablero->coordenadasCombate = sf::Vector2i(objetivo->getX(), objetivo->getY());
            tablero->atacante = new Elemental(tablero->turnoActual, tablero->skinActual);
            tablero->defensor = objetivo->getPieza();
            tablero->hayCombatePendiente = true;
            if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
            else tablero->bandoOscuroUsoMagia = true;
        }
        break;

    case 6:
    {
        std::vector<Pieza*>& cementerio = (tablero->turnoActual == Bando::LUZ) ? tablero->piezasMuertasLuz : tablero->piezasMuertasOscuridad;
        if (!cementerio.empty() && !objetivo->estaOcupada()) {
            int hX = (tablero->origenSeleccionado != nullptr) ? tablero->origenSeleccionado->getX() : -1;
            int hY = (tablero->origenSeleccionado != nullptr) ? tablero->origenSeleccionado->getY() : -1;

            if (std::abs(objetivo->getX() - hX) <= 1 && std::abs(objetivo->getY() - hY) <= 1) {
                Pieza* pRevivida = cementerio.back();
                cementerio.pop_back();
                pRevivida->resetVida();
                objetivo->setPieza(pRevivida);
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
            }
        }
    }
    break;

    case 7:
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() != tablero->turnoActual) {
            objetivo->getPieza()->setEncarcelada(2);
            if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
            else tablero->bandoOscuroUsoMagia = true;
        }
        break;
    }
}