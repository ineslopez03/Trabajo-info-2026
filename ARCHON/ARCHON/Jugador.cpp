#include "Jugador.h"
#include "Pieza.h"
#include <algorithm>
// Constructor: inicializa nombre, bando y pone el contador de piezas a cero
Jugador::Jugador(std::string _nombre, Bando _bando)
    : nombre(_nombre), bando(_bando), piezasRestantes(0) {
}
// Destructor: limpia el vector (no libera las piezas, eso lo hace el Tablero)
Jugador::~Jugador() {
    piezas.clear();
}
// Añade una pieza al jugador y le asigna este jugador como propietario
void Jugador::anadirPieza(Pieza* p) {
    if (p) { // Comprobación de seguridad: no añadir punteros nulos
        piezas.push_back(p); // Añade la pieza al vector
        p->setJugador(this);  // Le notifica a la pieza quién es su jugador
    }
}
// Elimina una pieza del vector del jugador (no la destruye)
void Jugador::eliminarPieza(Pieza* p) {
    auto it = std::find(piezas.begin(), piezas.end(), p);// Busca la pieza en el vector
    if (it != piezas.end()) {// Si la encontró
        piezas.erase(it);// La elimina del vector
    }
}
