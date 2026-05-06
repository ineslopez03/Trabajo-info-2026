#pragma once
#include "Jugador.h"

class JugadorIA : public Jugador {
private:
    int nivelDificultad; // Atributo específico del esquema para la IA

public:
    // Constructor que inicializa la base y el atributo de dificultad
    JugadorIA(std::string _nombre, Bando _bando, int _nivel = 1);
    virtual ~JugadorIA();

    // Implementación del método virtual puro para la toma de decisiones algorítmica
    void decidirMovimiento() override;

    // Método específico para evaluar el estado del tablero (según esquema)
    int evaluarTablero();
};