#pragma once
#include "PiezaVoladora.h"

class Fenix : public PiezaVoladora {
private:
    // Función estática para cargar las texturas una sola vez
    static sf::Texture& obtenerTexturaHP(Bando b);

public:
    Fenix(Bando b);
};