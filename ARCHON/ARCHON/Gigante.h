#pragma once
#include "PiezaTerrestre.h"

class Gigante : public PiezaTerrestre {
public:
    Gigante(Bando b);
    static sf::Texture& obtenerTexturaHP(Bando b);

};