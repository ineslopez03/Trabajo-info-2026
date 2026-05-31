#include "Hechicero.h"
#include <iostream>
// Devuelve la ruta de imagen del Hechicero según el skin y bando
std::string Hechicero::obtenerRuta(Bando b, std::string skin) {
    if (skin == "HARRY_POTTER") {
        // Dumbledore (LUZ) o Voldemort (OSCURIDAD): los dos magos más poderosos
        return (b == Bando::LUZ) ? "imagenes/HP/Dumbledore.png" : "imagenes/HP/Voldemort.png";
    }
    else if (skin == "STAR_WARS") {
        // Yoda (LUZ) o Palpatine (OSCURIDAD): los líderes de cada bando
        return (b == Bando::LUZ) ? "imagenes/SW/Yoda.png" : "imagenes/SW/Palpatin.png";
    }
    else {
        // Skin clásico: Hechicero (LUZ) o Hechicera (OSCURIDAD)
        return (b == Bando::LUZ) ? "imagenes/Classic/Hechicero.png" : "imagenes/Classic/Hechicera.png";
    }
}
// Constructor: inicializa el Hechicero con sus estadísticas y prepara sus hechizos
Hechicero::Hechicero(Bando b, std::string skin)
    : PiezaTeletransporte(20, 8, 5, 2, Pieza::obtenerTexturaDesdeRuta(Hechicero::obtenerRuta(b, skin)), b)
{// vida=20, daño=8, velMov=5, velAta=2;
    this->velMov = 2;// el rango de teletransporte es de 2 casillas
    for (int i = 0; i < 7; i++) {
        hechizosDisponibles[i] = true;// Todos los hechizos comienzan disponibles al inicio
    }
}
// Dibuja el Hechicero en la arena de combate (delega a PiezaTeletransporte)
void Hechicero::dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) {
    // Delega completamente al padre; no añade lógica visual propia
        PiezaTeletransporte::dibujarEnArena(ventana, pos, mirandoDerecha, skin);
    
}