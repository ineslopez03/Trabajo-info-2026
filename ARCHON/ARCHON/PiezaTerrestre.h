#pragma once
#include"Pieza.h"

class PiezaTerrestre : public Pieza {
public:
	
	PiezaTerrestre(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b);
	void dibujar(sf::RenderWindow& ventana, float x, float y, float tamano) override;
	bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]);
};
