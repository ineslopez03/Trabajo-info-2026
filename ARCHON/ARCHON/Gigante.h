#pragma once
#include "PiezaTerrestre.h"

class Gigante : public PiezaTerrestre {
private: static std::string obtenerRuta(Bando b, std::string skin);
public:
	Gigante(Bando b, std::string skin);
};

