#include"Pieza.h"
#include"Casilla.h"

float Pieza::getVidaEfectiva(ColorCasilla colorActual)
{
	float modificador = 1.0f;//Neutral(Gris)
    if (bando == Bando::LUZ) {
        switch (colorActual) {
        case ColorCasilla::BLANCO:      modificador = 1.3f; break; // Máximo bonus
        case ColorCasilla::GRIS_CLARO:  modificador = 1.15f; break;
        case ColorCasilla::GRIS_OSCURO: modificador = 0.85f; break;
        case ColorCasilla::NEGRO:       modificador = 0.7f; break; // Máxima penalización
        }
    }
    else if (bando == Bando::OSCURIDAD) {
        switch (colorActual) {
        case ColorCasilla::NEGRO:       modificador = 1.3f; break;
        case ColorCasilla::GRIS_OSCURO: modificador = 1.15f; break;
        case ColorCasilla::GRIS_CLARO:  modificador = 0.85f; break;
        case ColorCasilla::BLANCO:      modificador = 0.7f; break;
        }
    }

    return (float)this->vida * modificador;
}
