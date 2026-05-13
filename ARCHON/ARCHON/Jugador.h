#pragma once
#include <vector>
#include <string>
#include "Pieza.h"

class Jugador {
protected:
    std::string nombre;
    Bando bando;
    std::vector<Pieza*> piezas;
    int piezasRestantes;

public:
    Jugador(std::string _nombre, Bando _bando);
    virtual ~Jugador();

    virtual void decidirMovimiento() = 0;

    void anadirPieza(Pieza* p);
    void eliminarPieza(Pieza* p);
    Bando getBando() const { return bando; }
    std::string getNombre() const { return nombre; }

    // Casting explícito aplicado para resolver el Warning C4267
    int getNumPiezas() const { return static_cast<int>(piezas.size()); }

    std::vector<Pieza*>& getPiezas() { return piezas; }
};