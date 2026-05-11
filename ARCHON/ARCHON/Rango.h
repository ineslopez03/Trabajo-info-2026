#pragma once
#include "PiezaVoladora.h"

class Rango : public PiezaVoladora {
private: static std::string obtenerRuta(Bando b, std::string skin);
public:
	Rango(Bando b, std::string skin);
};

