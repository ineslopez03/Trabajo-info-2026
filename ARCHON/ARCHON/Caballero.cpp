#include "Caballero.h"

// Devuelve la ruta de imagen según el skin y el bando
std::string Caballero::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Dobby (LUZ) o Griphook el Goblin (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/HP/Dobby.png" : "imagenes/HP/Griphook.png";
    }
    else if (skin == "STAR_WARS") {
        // Rebelde (LUZ) o Stormtrooper (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/SW/Rebelde.png" : "imagenes/SW/Stormtrooper.png";
    }
    else {
        // Skin clásico: Caballero (LUZ) o Goblin (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/Classic/Caballero.png" : "imagenes/Classic/Goblin.png";
    }
}

// Constructor: inicializa el Caballero con sus estadísticas
Caballero::Caballero(Bando b, std::string skin)
    : PiezaTerrestre(10, 4, 4, 2, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b) {
    // vida=10, daño=4, velMov=4, velAta=2, textura según bando/skin
    this->rangoMovimiento = 3;// Se mueve hasta 3 casillas en horizontal o vertical
}