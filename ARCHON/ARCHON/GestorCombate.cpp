#include "GestorCombate.h"
#include "Tablero.h"
#include "Pieza.h"
#include <iostream>

void GestorCombate::procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal, Tablero* tablero) {
    registrarMuerte(perdedor, tablero);
    eliminarPiezaDelMapa(perdedor, tablero);

    if (atacanteOriginal == tablero->atacante && tablero->hechizoSeleccionado == 5) {
        if (ganador == atacanteOriginal) {
            delete ganador; 
            std::cout << "El Elemental vencio y se desvanecio en el eter." << std::endl;
        }
        else {
            
            ganador->restaurarValoresOriginales(ganador->getVidaBase());
        }
        tablero->hechizoSeleccionado = 0; 
    }
    else {
       
        if (ganador == atacanteOriginal) {
            moverPiezaACasilla(ganador, tablero->coordenadasCombate, tablero);
        }
        ColorCasilla colorCasilla = tablero->getColorCasilla(tablero->coordenadasCombate.x, tablero->coordenadasCombate.y);
        int porcentajeBono = 0;
        Bando bandoFavorecido = Bando::LUZ;

        if (colorCasilla == ColorCasilla::BLANCO) { porcentajeBono = 40; bandoFavorecido = Bando::LUZ; }
        else if (colorCasilla == ColorCasilla::NEGRO) { porcentajeBono = 40; bandoFavorecido = Bando::OSCURIDAD; }

        if (ganador->getBando() == bandoFavorecido && porcentajeBono > 0) {
            int vidaConBono = ganador->getVidaBase();
            int vidaOriginal = static_cast<int>(vidaConBono / (1.0f + (porcentajeBono / 100.0f)));
            if (vidaOriginal <= 0 && vidaConBono > 0) vidaOriginal = 1; 
            ganador->restaurarValoresOriginales(vidaOriginal);
        }
        else {
            ganador->restaurarValoresOriginales(ganador->getVidaBase());
        }
    }
}

void GestorCombate::registrarMuerte(Pieza* p, Tablero* tablero) {
    if (p == nullptr) return;
    
    if (p->getBando() == Bando::LUZ) tablero->piezasMuertasLuz.push_back(p);
    else                              tablero->piezasMuertasOscuridad.push_back(p);
}

void GestorCombate::eliminarPiezaDelMapa(Pieza* p, Tablero* tablero) {
    if (p == nullptr) return;
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero->matriz[i][j]->getPieza() == p) {
                tablero->matriz[i][j]->setPieza(nullptr);
                return;
            }
        }
    }
}

void GestorCombate::moverPiezaACasilla(Pieza* p, sf::Vector2i destino, Tablero* tablero) {
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero->matriz[i][j]->getPieza() == p) {
                tablero->matriz[i][j]->setPieza(nullptr);
            }
        }
    }
   
    tablero->matriz[destino.x][destino.y]->setPieza(p);
}