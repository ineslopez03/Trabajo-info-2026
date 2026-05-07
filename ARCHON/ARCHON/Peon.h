#pragma once
#include "PiezaTerrestre.h"

class Peon : public PiezaTerrestre {
public:
    // Constructor
    Peon(Bando b);

    // Función para cargar la textura (la definimos en el .cpp)
    static sf::Texture& obtenerTexturaHP(Bando b);
};