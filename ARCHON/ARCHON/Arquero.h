#pragma once
#include "PiezaTerrestre.h"
#include <string>

class Arquero : public PiezaTerrestre {//Hereda de pieza terrestre(movimiento horizontal/vertical)
private:
    // Método estático privado que devuelve la ruta de la imagen según el bando y el skin
    static std::string obtenerRuta(Bando b, std::string skin);
public:
    // Constructor: crea un Arquero del bando y skin indicados
    Arquero(Bando b, std::string skin);
};
