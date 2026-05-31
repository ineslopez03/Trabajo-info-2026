#pragma once
#include "PiezaTerrestre.h"
#include <string>

class Golem : public PiezaTerrestre {// Golem hereda de PiezaTerrestre
private:
    static std::string obtenerRuta(Bando b, std::string skin);// Devuelve la ruta de imagen según bando y skin
public:
    Golem(Bando b, std::string skin);// Constructor del Golem
};