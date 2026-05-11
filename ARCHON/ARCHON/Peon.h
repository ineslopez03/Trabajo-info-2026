#pragma once
#include "PiezaTerrestre.h"

class Peon : public PiezaTerrestre {
private: static std::string obtenerRuta(Bando b, std::string skin);
public:
	Peon(Bando b, std::string skin);
};