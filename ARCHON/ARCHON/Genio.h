#pragma once
#include "PiezaVoladora.h"
#include <string>

class Genio : public PiezaVoladora {// Genio hereda de PiezaVoladora
private:
    static std::string obtenerRuta(Bando b, std::string skin);// Devuelve la ruta de imagen según bando y skin
public:
    Genio(Bando b, std::string skin);// Constructor del Genio
};