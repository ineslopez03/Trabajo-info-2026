#pragma once
#include "PiezaTeletransporte.h"
#include <string>

class Hechicero : public PiezaTeletransporte {// Hechicero hereda de PiezaTeletransporte (movimiento libre sin restricción de camino)
private:
    static std::string obtenerRuta(Bando b, std::string skin); // Devuelve la ruta de imagen según bando y skin
    bool hechizosDisponibles[7];// Array de 7 flags: cada posición indica si ese hechizo aún está disponible
    std::string nombresHechizos[7] = {
        "Teleport (Aparicion)",     // Mueve cualquier pieza
        "Heal (Episkey)",           // Cura vida total
        "Shift Time (Gira-tiempo)", // Cambia el ciclo de luz
        "Exchange (Permutacion)",   // Intercambia dos piezas
        "Summon (Resurreccion)",    // Revive a un aliado
        "Imprison (Petrificus)",    // Paraliza un enemigo
        "Banish (Desterrar)"        // Elimina una pieza (muy difícil)
    };
public:
    Hechicero(Bando b, std::string skin); // Constructor: inicializa el Hechicero y marca todos sus hechizos como disponibles
    bool puedeLanzar(int indice) const { return hechizosDisponibles[indice]; }// Devuelve true si el hechizo en la posición 'indice' aún no ha sido usado
    void gastarHechizo(int indice) { hechizosDisponibles[indice] = false; } // Marca el hechizo en 'indice' como gastado (no se puede volver a usar)
    // Sobrescritura para inyectar la hoja de sprites en combate
    void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) override;
};
