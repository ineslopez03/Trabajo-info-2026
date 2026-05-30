#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Constantes.h"
#include "Pieza.h"
#include "Casilla.h"

class Tablero;  // forward declaration para evitar dependencia circular
// Ejecuta los 7 hechizos del Hechicero. Accede a los privados del Tablero via friend.
class GestorHechizos {
public:
    void ejecutarHechizo(int idHechizo, Casilla* objetivo, Tablero* tablero);
};