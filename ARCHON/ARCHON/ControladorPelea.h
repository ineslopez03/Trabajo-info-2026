#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

// Estructura de transferencia de intenciones
struct IntencionJugador {
    sf::Vector2f direccionMovimiento;
    bool intentandoAtacar;
};

class ControladorPelea {
public:
    // Métodos estáticos para traducir hardware a comandos lógicos
    static IntencionJugador obtenerIntencionIzquierda();
    static IntencionJugador obtenerIntencionDerecha();
};