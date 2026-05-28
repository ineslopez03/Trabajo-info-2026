#pragma once
#include "PiezaVoladora.h"

class Rango : public PiezaVoladora {
protected: 
	static std::string obtenerRuta(Bando b, std::string skin);
public:
	Rango(Bando b, std::string skin);
};

