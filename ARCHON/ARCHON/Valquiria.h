#pragma once
#include "PiezaVoladora.h"
#include <string>

class Valquiria : public PiezaVoladora {//hereda de pieza voladora
private:
    static std::string obtenerRuta(Bando b, std::string skin);//metodo para la textura
public:
    Valquiria(Bando b, std::string skin);//constructor de la valquiria
};