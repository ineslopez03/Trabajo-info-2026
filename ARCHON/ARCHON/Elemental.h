#pragma once
#include "PiezaVoladora.h"
// Elemental es una criatura temporal invocada por el hechizo Summon; desaparece tras el combate
class Elemental : public PiezaVoladora {// Elemental hereda de PiezaVoladora (movimiento libre en cualquier dirección)
protected: 
	static std::string obtenerRuta(Bando b, std::string skin);
	// Método para obtener la ruta de imagen
public:
	// Constructor: crea el Elemental con estadísticas elevadas y escala visual extra
	Elemental(Bando b, std::string skin);
};

