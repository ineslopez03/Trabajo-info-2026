#pragma once
#include "PiezaVoladora.h"

class Rango : public PiezaVoladora {
private:
    static sf::Texture& obtenerTexturaHP(Bando b);
public:
    Rango(Bando b);
};