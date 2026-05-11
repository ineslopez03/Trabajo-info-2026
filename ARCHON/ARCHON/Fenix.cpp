#include "Fenix.h"
#include <iostream>

std::string obtenerRutaFenix(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Carpeta HP + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/HP/Fawkes.png" : "imagenes/HP/Dementor.png";
    }
    else if (skin == "STAR_WARS") {
        // Carpeta SW + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/SW/X-Wing.png" : "imagenes/SW/Tie Fighter.png";
    }
    else {
        // Carpeta CLASSIC + nombres estándar
        return (b == Bando::LUZ) ? "imagenes/Classic/Phoenix.png" : "imagenes/Classic/Shapeshifter.png";
    }
}

Fenix::Fenix(Bando b, std::string skin)
    : PiezaVoladora(12, 4, 5, 2, PiezaVoladora::obtenerTexturaDesdeRuta(obtenerRutaFenix(b, skin)), b)
{
    this->rangoMovimiento = 5;
}
