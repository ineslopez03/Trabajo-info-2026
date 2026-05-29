#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Constantes.h"
#include "Pieza.h"
#include "Casilla.h"

class Tablero;  

class GestorHechizos {
public:
    void ejecutarHechizo(int idHechizo, Casilla* objetivo, Tablero* tablero);
};