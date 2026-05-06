#pragma once
#include <vector>
#include <string>
#include "Pieza.h"

class Jugador {
protected:
    std::string nombre;
    Bando bando;
    std::vector<Pieza*> piezas; // Agregación: El jugador tiene piezas
    int piezasRestantes;

public:
    Jugador(std::string _nombre, Bando _bando);
    virtual ~Jugador();

    // Método virtual puro: Define cómo elige el jugador su siguiente movimiento
    virtual void decidirMovimiento() = 0;

    // Métodos de gestión de piezas
    void anadirPieza(Pieza* p);
    void eliminarPieza(Pieza* p);

    Bando getBando() const { return bando; }
    std::string getNombre() const { return nombre; }
    int getNumPiezas() const { return piezas.size(); }
    std::vector<Pieza*>& getPiezas() { return piezas; }
};