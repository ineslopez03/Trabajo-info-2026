#pragma once
class Tablero;
class Casilla;
// Gestiona la selección de piezas, movimientos, inicio de combates y cambio de turno
class GestorTurno {
public:
    void gestionarTurno(Casilla* casillaClicada, Tablero* tablero);// procesa cada clic del jugador
    void finalizarTurno(Tablero* tablero);// descuenta bloqueos, cambia bando, resetea selección
};
