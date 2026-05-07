#include "Jugador.h"
#include "Pieza.h"
#include <algorithm>

Jugador::Jugador(std::string _nombre, Bando _bando)
    : nombre(_nombre), bando(_bando), piezasRestantes(0) {
}

Jugador::~Jugador() {
    piezas.clear();
}

void Jugador::anadirPieza(Pieza* p) {
    if (p) {
        piezas.push_back(p);
        p->setJugador(this); 
    }
}

void Jugador::eliminarPieza(Pieza* p) {
    auto it = std::find(piezas.begin(), piezas.end(), p);
    if (it != piezas.end()) {
        piezas.erase(it);
    }
}
