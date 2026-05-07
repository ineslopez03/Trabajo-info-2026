#pragma once
#include "PiezaTerrestre.h"

class Peon : public PiezaTerrestre {
public:
    Peon(Bando b);
    static sf::Texture& obtenerTexturaHP(Bando b);
   
};