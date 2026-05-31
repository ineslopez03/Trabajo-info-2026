#include "GestorHechizos.h"
#include "Tablero.h"
#include "Elemental.h"
#include <cmath>  

void GestorHechizos::ejecutarHechizo(int idHechizo, Casilla* objetivo, Tablero* tablero) {
    Bando bandoLanzador = tablero->turnoActual;
    // guarda el bando antes de cualquier cambio
    switch (idHechizo) {
    case 1:// TELEPORT: mueve una pieza aliada a cualquier casilla vacía del tablero
        if (tablero->piezaAuxiliar == nullptr) {// Primer click: seleccionar la pieza a teleportar
            // Solo se puede seleccionar una pieza propia
            if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == tablero->turnoActual) {
                tablero->piezaAuxiliar = objetivo;// Guarda la casilla origen del teleport
            }
        }
        else {
            // Segundo clic: coloca la pieza en la casilla vacía destino
            if (!objetivo->estaOcupada()) {// El destino debe estar vacío
                objetivo->setPieza(tablero->piezaAuxiliar->getPieza());// Mueve la pieza al destino
                tablero->piezaAuxiliar->setPieza(nullptr); // Vacía la casilla origen
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;// Marca magia usada
                else tablero->bandoOscuroUsoMagia = true;
            }
        }
        break;

    case 2:// HEAL: restaura la vida máxima de una pieza aliada dañada
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == tablero->turnoActual) {
            Pieza* p = objetivo->getPieza();
            if (p->getVidaBase() < p->getVidaMaxima()) {// solo cura si tiene vida perdida
                p->resetVida();  // Restaura la vida al máximo
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
                tablero->piezaAuxiliar = nullptr;  // Limpia la auxiliar tras curar
            }
        }
        break;

    case 3:// SHIFT TIME: avanza el ciclo de color del tablero 3 turnos, sin necesitar objetivo
        tablero->turnosContados += 3;
        if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
        else tablero->bandoOscuroUsoMagia = true;
        break;

    case 4: // EXCHANGE: intercambia la posición de dos piezas aliadas del tablero
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == tablero->turnoActual) {
            if (tablero->piezaAuxiliar == nullptr) {
                tablero->piezaAuxiliar = objetivo; // primer clic: guarda la primera pieza aliada
            }
            else {
                // Segundo clic: intercambia las dos piezas aliadas
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
            // Solo se puede invocar contra piezas enemigas
            if (tablero->matriz[objetivo->getX()][objetivo->getY()]->getEsPuntoDePoder()) return;
            tablero->coordenadasCombate = sf::Vector2i(objetivo->getX(), objetivo->getY());// Guarda la posición del combate
            tablero->atacante = new Elemental(tablero->turnoActual, tablero->skinActual);// Elemental creado en memoria
            tablero->defensor = objetivo->getPieza();// El defensor es la pieza enemiga en la casilla
            tablero->hayCombatePendiente = true;// MotorArchon lanzará la Arena
            if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
            else tablero->bandoOscuroUsoMagia = true;
        }
        break;

    case 6:// REVIVE: resucita la última pieza muerta en una casilla adyacente al Hechicero
    {
        std::vector<Pieza*>& cementerio = (tablero->turnoActual == Bando::LUZ) ? tablero->piezasMuertasLuz : tablero->piezasMuertasOscuridad;// Selecciona el cementerio del bando activo
        if (!cementerio.empty() && !objetivo->estaOcupada()) { // Debe haber piezas muertas y el destino libre
            int hX = (tablero->origenSeleccionado != nullptr) ? tablero->origenSeleccionado->getX() : -1;
            int hY = (tablero->origenSeleccionado != nullptr) ? tablero->origenSeleccionado->getY() : -1;
            // Solo permite resucitar en casillas a 1 paso del Hechicero (adyacentes y diagonales)
            if (std::abs(objetivo->getX() - hX) <= 1 && std::abs(objetivo->getY() - hY) <= 1) {
                Pieza* pRevivida = cementerio.back();// coge la última pieza muerta
                cementerio.pop_back();// La elimina del cementerio
                pRevivida->resetVida(); // restaura su vida máxima
                objetivo->setPieza(pRevivida);// la coloca en el tablero
                if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
                else tablero->bandoOscuroUsoMagia = true;
            }
        }
    }
    break;

    case 7: // IMPRISON: bloquea una pieza enemiga durante 2 turnos
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() != tablero->turnoActual) {
            // Las piezas en puntos de poder son inmunes a hechizos enemigos
            if (tablero->matriz[objetivo->getX()][objetivo->getY()]->getEsPuntoDePoder()) return;
            objetivo->getPieza()->setEncarcelada(2); // 2 = turnos que estará bloqueada
            if (bandoLanzador == Bando::LUZ) tablero->bandoLuzUsoMagia = true;
            else tablero->bandoOscuroUsoMagia = true;
        }
        break;
    }
}