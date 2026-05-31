#pragma once
#include "PiezaTeletransporte.h"
#include <string>

class Hechicero : public PiezaTeletransporte {// Hechicero hereda de PiezaTeletransporte (movimiento libre sin restricción de camino)
private:
    static std::string obtenerRuta(Bando b, std::string skin); // Devuelve la ruta de imagen según bando y skin
    bool hechizosDisponibles[7];// Array de 7 flags: cada posición indica si ese hechizo aún está disponible
public:
    Hechicero(Bando b, std::string skin); // Constructor: inicializa el Hechicero y marca todos sus hechizos como disponibles
    bool puedeLanzar(int indice) const { return hechizosDisponibles[indice]; }// Devuelve true si el hechizo en la posición 'indice' aún no ha sido usado
    void gastarHechizo(int indice) { hechizosDisponibles[indice] = false; } // Marca el hechizo en 'indice' como gastado (no se puede volver a usar)
};

