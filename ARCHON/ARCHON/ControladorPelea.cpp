#include "ControladorPelea.h"

IntencionJugador ControladorPelea::obtenerIntencionIzquierda() {
    IntencionJugador intencion = { {0.f, 0.f}, false };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) intencion.direccionMovimiento.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) intencion.direccionMovimiento.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) intencion.direccionMovimiento.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) intencion.direccionMovimiento.x += 1.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2)) {
        intencion.intentandoAtacar = true;
    }

    return intencion;
}

IntencionJugador ControladorPelea::obtenerIntencionDerecha() {
    IntencionJugador intencion = { {0.f, 0.f}, false };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) intencion.direccionMovimiento.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) intencion.direccionMovimiento.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) intencion.direccionMovimiento.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) intencion.direccionMovimiento.x += 1.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad0)) {
        intencion.intentandoAtacar = true;
    }

    return intencion;
}