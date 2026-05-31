#include "Valquiria.h"

std::string Valquiria::obtenerRuta(Bando b, std::string skin) {//ruta del sprite segun bando y skin
    if (skin == "HARRY_POTTER") {
        return (b == Bando::LUZ) ? "imagenes/HP/Hermion.png" : "imagenes/HP/Bellatrix.png";//bando luz u oscuridad hp
    }
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Leia.png" : "imagenes/SW/Morador.png";//bando luz u oscuridad sw
    }
    else {
        return (b == Bando::LUZ) ? "imagenes/Classic/Valkiria.png" : "imagenes/Classic/Banshee.png";//bando luz u oscuridad clasico
    }
}

Valquiria::Valquiria(Bando b, std::string skin)
    : PiezaVoladora(10, 4, 6, 3, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b) {//parametros de vida y daño
    this->rangoMovimiento = 3;//casillas maximas de vuelo
}