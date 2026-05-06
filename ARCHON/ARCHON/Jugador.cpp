#include "Jugador.h"
#include <algorithm>

Jugador::Jugador(std::string _nombre, Bando _bando)
    : nombre(_nombre), bando(_bando), piezasRestantes(0) {
}

Jugador::~Jugador() {
    // Nota: Las piezas se suelen borrar en el Motor o Tablero para evitar 
    // errores de doble liberación si están en varias listas.
    piezas.clear();
}

void Jugador::anadirPieza(Pieza* p) {
    if (p) {
        piezas.push_back(p);
        p->setJugador(this); // Establecemos la relación bidireccional
    }
}

void Jugador::eliminarPieza(Pieza* p) {
    auto it = std::find(piezas.begin(), piezas.end(), p);
    if (it != piezas.end()) {
        piezas.erase(it);
    }
}