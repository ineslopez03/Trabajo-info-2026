#pragma once
#include "PiezaTeletransporte.h"
#include<string.h>

class Hechicero : public PiezaTeletransporte {
private:
    // Función estática para cargar las texturas una sola vez
    static sf::Texture& obtenerTexturaHP(Bando b);
    
    bool hechizosDisponibles[7];

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
    Hechicero(Bando b);
    bool puedeLanzar(int indice) const { return hechizosDisponibles[indice]; }
    void gastarHechizo(int indice) { hechizosDisponibles[indice] = false; }
};
