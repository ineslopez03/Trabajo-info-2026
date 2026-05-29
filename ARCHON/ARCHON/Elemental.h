#pragma once
#include "PiezaVoladora.h"

class Elemental : public PiezaVoladora {
protected: 
	static std::string obtenerRuta(Bando b, std::string skin);
public:
	Elemental(Bando b, std::string skin);
};

