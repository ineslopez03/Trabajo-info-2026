#include"Pieza.h"
#include"Casilla.h"
void Pieza::aplicarBonoColor(int porcentaje)
{
	int incremento = (vidaMaxima * porcentaje) / 100;
	this->vidaMaxima += incremento;
	this->vida += incremento;
}
void Pieza::restaurarValoresOriginales(int vidaAntesDelCombate)
{
	this->vida = vidaAntesDelCombate;
}