#include "Fenix.h"

// Devuelve la ruta de imagen del Fénix según el skin y bando
std::string Fenix::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Fawkes el fénix (LUZ) o Dementor (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/HP/Fawkes.png" : "imagenes/HP/Dementor.png";
    }
    else if (skin == "STAR_WARS") {
        // Luke Skywalker (LUZ) o Darth Vader (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/SW/Luke.png" : "imagenes/SW/Darthvader.png";
    }
    else {
        // Skin clásico: Fénix (LUZ) o Dragón (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/Classic/Fenix.png" : "imagenes/Classic/Dragon.png";
    }
}

Fenix::Fenix(Bando b, std::string skin)// Constructor: inicializa el Fénix con sus estadísticas
    : PiezaVoladora(16, 8, 4, 1, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
    // vida=16, daño=8, velMov=4, velAta=1; pieza robusta con buen alcance de movimiento
{
    this->rangoMovimiento = 4;
    // Puede moverse hasta 4 casillas en cualquier dirección
}