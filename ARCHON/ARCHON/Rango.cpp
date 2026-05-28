#include "Rango.h"
#include <iostream>
static std::string obtenerRutaRango(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Carpeta HP + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/HP/Ginny.png" : "imagenes/HP/Bellatrix.png";
    }
    else if (skin == "STAR_WARS") {
        // Carpeta SW + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/SW/Han Solo.png" : "imagenes/SW/Boba Fett.png";
    }
    else {
        // Carpeta CLASSIC + nombres estándar
        return (b == Bando::LUZ) ? "imagenes/Classic/Archer.png" : "imagenes/Classic/Basilisk.png";
    }
}
Rango::Rango(Bando b, std::string skin)
    : PiezaVoladora(30, 10, 3, 1, PiezaVoladora::obtenerTexturaDesdeRuta(obtenerRutaRango(b, skin)), b)
{
    this->rangoMovimiento = 3;
}
