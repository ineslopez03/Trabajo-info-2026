#include "GestorCombate.h"
#include "Tablero.h"
#include "Pieza.h"
#include <iostream>

void GestorCombate::procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal, Tablero* tablero) {
    registrarMuerte(perdedor, tablero);// guarda al perdedor en el cementerio antes de eliminarlo
    eliminarPiezaDelMapa(perdedor, tablero);// vacía la casilla donde estaba el perdedor

    if (atacanteOriginal == tablero->atacante && tablero->hechizoSeleccionado == 5) {
        // Caso especial hechizo 5 (Summon): el atacante era un Elemental temporal
        if (ganador == atacanteOriginal) {// el Elemental se destruye al ganar, no queda en el tablero
            delete ganador;
            std::cout << "El Elemental vencio y se desvanecio en el eter." << std::endl;
        }
        // Si el Elemental perdió, el ganador (enemigo) no necesita restauración especial
        tablero->hechizoSeleccionado = 0; 
    }
    else {
        // Combate normal: si ganó el atacante, lo mueve a la casilla del combate
        if (ganador == atacanteOriginal) {
            moverPiezaACasilla(ganador, tablero->coordenadasCombate, tablero);
        }
      
        }
    }

void GestorCombate::registrarMuerte(Pieza* p, Tablero* tablero) {
    if (p == nullptr) return;
    // Guarda la pieza en el cementerio de su bando para que Revive pueda resucitarla
    if (p->getBando() == Bando::LUZ) tablero->piezasMuertasLuz.push_back(p);
    else                              tablero->piezasMuertasOscuridad.push_back(p);
}

void GestorCombate::eliminarPiezaDelMapa(Pieza* p, Tablero* tablero) {
    if (p == nullptr) return;
    // Guarda la pieza en el cementerio de su bando para que Revive pueda resucitarla
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero->matriz[i][j]->getPieza() == p) {
                tablero->matriz[i][j]->setPieza(nullptr);
                return;// sale en cuanto la encuentra
            }
        }
    }
}

void GestorCombate::moverPiezaACasilla(Pieza* p, sf::Vector2i destino, Tablero* tablero) {
    // Primero vacía la casilla origen buscando la pieza en el tablero
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero->matriz[i][j]->getPieza() == p) {
                tablero->matriz[i][j]->setPieza(nullptr);
                // Luego coloca la pieza en la casilla destino del combate
            }
        }
    }
   
    tablero->matriz[destino.x][destino.y]->setPieza(p);
}