#pragma once
#include "PiezaVoladora.h"

class Fenix : public PiezaVoladora {
private: static std::string obtenerRuta(Bando b, std::string skin);
public:
	Fenix(Bando b, std::string skin);
};

