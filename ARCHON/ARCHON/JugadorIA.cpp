#include "JugadorIA.h"
#include <iostream>

JugadorIA::JugadorIA(std::string _nombre, Bando _bando, int _nivel)
    : Jugador(_nombre, _bando), nivelDificultad(_nivel) {
    // Inicialización de la IA
}

JugadorIA::~JugadorIA() {
    // La limpieza de piezas se delega a la clase base
}

void JugadorIA::decidirMovimiento() {
    // Lógica de ingeniería para la IA:
    // 1. Analizar todas las piezas vivas en el vector 'piezas'.
    // 2. Evaluar las casillas adyacentes según el tipo de Pieza (Terrestre, Voladora, etc.).
    // 3. Llamar a evaluarTablero() para puntuar cada movimiento posible.
    // 4. Ejecutar el movimiento con mayor puntuación.

    std::cout << "La IA " << nombre << " esta calculando el mejor movimiento..." << std::endl;

    int puntuacionActual = evaluarTablero();
}

int JugadorIA::evaluarTablero() {
    // Retornar un valor basado en el número de piezas propias vs piezas enemigas
    // y la posición estratégica en el tablero.
    return piezas.size() * 10;
}
