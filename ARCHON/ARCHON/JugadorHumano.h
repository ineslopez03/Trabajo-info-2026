#pragma once
#include "Jugador.h"

class JugadorHumano : public Jugador {
public:
    // Utilizamos el constructor de la clase base Jugador
    JugadorHumano(std::string _nombre, Bando _bando);
    virtual ~JugadorHumano();

    // Implementación obligatoria del método virtual puro de la clase abstracta
    void decidirMovimiento() override;
};
