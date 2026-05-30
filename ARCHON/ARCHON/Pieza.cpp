#include "Pieza.h"
#include "Casilla.h"
#include <algorithm>

void Pieza::aplicarBonoColor(int porcentaje)
{
    // el incremento se calcula de forma estricta sobre la vida base de diseño
    int incremento = (vidaMaximaBase * porcentaje) / 100;
    this->vidaMaxima = vidaMaximaBase + incremento;
    this->vida += incremento;
}

void Pieza::restaurarValoresOriginales(int vidaAntesDelCombate)
{
    // se restituye el tope físico y se asimila el daño sufrido en la arena
    this->vidaMaxima = vidaMaximaBase;
    this->vida = vidaAntesDelCombate;
    if (this->vida > this->vidaMaxima) {
        this->vida = this->vidaMaxima; 
    }
}
void Pieza::curar(int cantidad) {
    this->vida += cantidad;
    if (this->vida > this->vidaMaximaBase) {
        this->vida = this->vidaMaximaBase;
    }
}