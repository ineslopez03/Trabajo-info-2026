#include "Arquero.h"

// Devuelve la ruta de la imagen del Arquero según el skin y el bando
std::string Arquero::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Skin Harry Potter: Harry (LUZ) o Dolores Umbridge (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/HP/Harry.png" : "imagenes/HP/Dolores.png";
    }
    else if (skin == "STAR_WARS") {
        // Skin Star Wars: Han Solo (LUZ) o Boba Fett (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/SW/Hansolo.png" : "imagenes/SW/Bobbafet.png";
    }
    else {
        // Skin clásico: Archon (LUZ) o Mantícora (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/Classic/Archon.png" : "imagenes/Classic/Manticora.png";
    }
}

// Constructor: inicializa el Arquero con sus estadísticas base
Arquero::Arquero(Bando b, std::string skin)
    : PiezaTerrestre(8, 3, 3, 3, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
    // vida=8, daño=3, velMov=3, velAta=3, textura cargada según bando y skin
{
    this->rangoMovimiento = 3;// Puede moverse hasta 3 casillas en horizontal o vertical
}