#pragma once
class Tablero;
class Casilla;

class GestorTurno {
public:
    void gestionarTurno(Casilla* casillaClicada, Tablero* tablero);
    void finalizarTurno(Tablero* tablero);
};
