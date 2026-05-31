#include "Genio.h"
// Devuelve la ruta de imagen del Genio según el skin y bando
std::string Genio::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Nick  (LUZ) o Boggart (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/HP/Nick.png" : "imagenes/HP/Boggart.png";
    }
    // Obi-Wan Kenobi (LUZ) o IG-88 (OSCURIDAD)
    else if (skin == "STAR_WARS") {
        return (b == Bando::LUZ) ? "imagenes/SW/Obiwan.png" : "imagenes/SW/IG.png";
    }
    else {
        // Skin clásico: Genio (LUZ) o Shapeshifter (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/Classic/Genio.png" : "imagenes/Classic/Shapeshifter.png";
    }
}
// Constructor: inicializa el Genio con sus estadísticas
Genio::Genio(Bando b, std::string skin)
    : PiezaVoladora(14, 5, 5, 3, Pieza::obtenerTexturaDesdeRuta(obtenerRuta(b, skin)), b)
{// vida=14, daño=5, velMov=5, velAta=3; pieza ágil con buena velocidad de movimiento y ataque
    this->rangoMovimiento = 3;// Se mueve hasta 3 casillas en cualquier dirección
}
// Dibuja el Genio en la arena de combate (delega a PiezaVoladora)
void Genio::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    PiezaVoladora::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
}