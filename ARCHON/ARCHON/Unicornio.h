#pragma once
#include "PiezaTerrestre.h"
#include <string>

class Unicornio : public PiezaTerrestre {//hereda de pieza terrestre
private:
    static std::string obtenerRuta(Bando b, std::string skin);//devuelve la ruta de la imagen
public:
    Unicornio(Bando b, std::string skin);//constructor del unicornio
};