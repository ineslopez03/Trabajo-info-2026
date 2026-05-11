#include "Gigante.h"
#include <iostream>
std::string obtenerRutaGigante(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Carpeta HP + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/HP/Hagrid.png" : "imagenes/HP/Troll.png";
    }
    else if (skin == "STAR_WARS") {
        // Carpeta SW + nombres específicos
        return (b == Bando::LUZ) ? "imagenes/SW/Chewbacca.png" : "imagenes/SW/Rancor.png";
    }
    else {
        // Carpeta CLASSIC + nombres estándar
        return (b == Bando::LUZ) ? "imagenes/Classic/Golem.png" : "imagenes/Classic/Troll.png";
    }
}



Gigante::Gigante(Bando b, std::string skin)
    : PiezaTerrestre(15, 5, 2, 0, PiezaTerrestre::obtenerTexturaDesdeRuta(obtenerRutaGigante(b, skin)), b)
{
    this->rangoMovimiento = 2;
}
