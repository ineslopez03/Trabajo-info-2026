#include "Golem.h"
// Devuelve la ruta de imagen del Golem según el skin y bando
std::string Golem::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Hagrid (LUZ) o Troll (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/HP/Hagrid.png" : "imagenes/HP/Troll.png";
    }
    else if (skin == "STAR_WARS") {
        // Chewbacca (LUZ) o Guardia Rojo Imperial (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/SW/Chewbacca.png" : "imagenes/SW/Guardiarojo.png";
    }
    else {
        // Skin clásico: Golem (LUZ) o Troll (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/Classic/Golem.png" : "imagenes/Classic/Troll.png";
    }
}
// Constructor: inicializa el Golem con sus estadísticas (muy resistente, muy lento)
Golem::Golem(Bando b, std::string skin)
    : PiezaTerrestre(18, 7, 2, 1, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
    // vida=18 (la más alta entre terrestres), daño=7, velMov=2, velAta=1 (el más lento)
{
    this->rangoMovimiento = 2;// Solo puede moverse 2 casillas, el menor rango terrestre
}

// Dibuja el Golem en la arena de combate (delega a PiezaTerrestre)
void Golem::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaTerrestre::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}