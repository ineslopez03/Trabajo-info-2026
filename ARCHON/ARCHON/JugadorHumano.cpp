#include "JugadorHumano.h"
#include <iostream>

JugadorHumano::JugadorHumano(std::string _nombre, Bando _bando)
    : Jugador(_nombre, _bando) {
    // Inicialización específica si fuera necesaria
}

JugadorHumano::~JugadorHumano() {
    // La liberación de piezas se gestiona en la clase base Jugador
}

void JugadorHumano::decidirMovimiento() {
    // Lógica:
    // 1. El motor detecta que es el turno de este jugador.
    // 2. Se activa la escucha de eventos de ratón/teclado en la InterfazUsuario.
    // 3. El jugador humano selecciona una pieza de su vector 'piezas'.
    // 4. Se valida que la casilla de destino sea legal según el tipo de Pieza.

    // Por ahora, dejamos la traza de control para depuración
    std::cout << "Esperando entrada del usuario: " << nombre << std::endl;
}
