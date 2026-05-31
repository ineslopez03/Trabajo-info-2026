#include "GestorCombate.h" 
#include "Tablero.h" 
#include "Pieza.h"
#include <iostream> 

//aplica las consecuencias de la pelea en el tablero
void GestorCombate::procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal, Tablero* tablero) {
    registrarMuerte(perdedor, tablero); //guarda al perdedor en el cementerio antes de eliminarlo para no perderlo
    eliminarPiezaDelMapa(perdedor, tablero); //vacía la casilla donde estaba el perdedor en la matriz 9x9

    if (atacanteOriginal == tablero->atacante && tablero->hechizoSeleccionado == 5) { //si el atacante era un elemental invocado por hechizo (el 5)
        //caso especial hechizo 5 (summon) porque el atacante era un elemental temporal
        if (ganador == atacanteOriginal) { //si el elemental temporal gano la pelea
            delete ganador; //el elemental se destruye al ganar, no queda ocupando hueco en el tablero
            std::cout << "El Elemental vencio y se desvanecio en el eter." << std::endl; 
        }
        //si el elemental perdió, el ganador no necesita restauración especial, se queda donde estaba
        tablero->hechizoSeleccionado = 0; //resetea la magia para el siguiente turno
    }
    else {
        //combate normal. si ganó el atacante, avanza y conquista la casilla. si gano el defensor, se queda quieto
        if (ganador == atacanteOriginal) {
            moverPiezaACasilla(ganador, tablero->coordenadasCombate, tablero); //lo mueve a la casilla de pelea
        }
    }
}

//guarda los muertos en las listas para el hechizo revive
void GestorCombate::registrarMuerte(Pieza* p, Tablero* tablero) {
    if (p == nullptr) return; //si por algun motivo llega un nulo, corta para evitar crasheos

    //guarda la pieza en el cementerio de su bando para que revive pueda resucitarla despues
    if (p->getBando() == Bando::LUZ) tablero->piezasMuertasLuz.push_back(p); //lo mete en la lista de luz
    else                              tablero->piezasMuertasOscuridad.push_back(p); //lo mete en la lista de oscuridad
}

//borra el cadaver de la matriz del tablero real
void GestorCombate::eliminarPiezaDelMapa(Pieza* p, Tablero* tablero) {
    if (p == nullptr) return; //seguridad anti nulos

    //recorre todo el tablero casilla a casilla buscando estaba esta pieza
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero->matriz[i][j]->getPieza() == p) { //si la encuentra
                tablero->matriz[i][j]->setPieza(nullptr); //quita el puntero dejando la casilla libre
                return; //sale en cuanto la encuentra para no seguir buscando 
            }
        }
    }
}

//recoloca a la pieza ganadora en la casilla de destino
void GestorCombate::moverPiezaACasilla(Pieza* p, sf::Vector2i destino, Tablero* tablero) {
    //primero vacía la casilla origen buscando la pieza en el tablero para que no se duplique
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (tablero->matriz[i][j]->getPieza() == p) { //si la encuentra en su posicion vieja
                tablero->matriz[i][j]->setPieza(nullptr); //la borra de ahi
                //luego coloca la pieza en la casilla destino del combate
            }
        }
    }

    tablero->matriz[destino.x][destino.y]->setPieza(p); //asigna oficialmente el puntero a su nueva casilla en la cuadricula
}