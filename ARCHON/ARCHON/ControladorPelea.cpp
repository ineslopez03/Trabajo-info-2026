#include "ControladorPelea.h" 

//lee el teclado para el jugador de la izquierda (bando de luz)
IntencionJugador ControladorPelea::obtenerIntencionIzquierda() {
    IntencionJugador intencion = { {0.f, 0.f}, false }; //parte de una intencion neutra igual que antes

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) intencion.direccionMovimiento.y -= 1.0f; //si pulsa w va para arriba 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) intencion.direccionMovimiento.y += 1.0f; //si pulsa s va para abajo 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) intencion.direccionMovimiento.x -= 1.0f; //si pulsa a va para la izquierda 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) intencion.direccionMovimiento.x += 1.0f; //si pulsa d va para la derecha 

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad2)) { //si pulsa el numero 2 
        intencion.intentandoAtacar = true; //activa el flag de que quiere atacar
    }

    return intencion; //se lo pasa a la arena para que lo procese
}

//lee el teclado para el jugador de la derecha (bando de oscuridad)
IntencionJugador ControladorPelea::obtenerIntencionDerecha() {
    IntencionJugador intencion = { {0.f, 0.f}, false }; //parte de una intencion neutra igual que antes

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) intencion.direccionMovimiento.y -= 1.0f; //flecha arriba
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) intencion.direccionMovimiento.y += 1.0f; //flecha abajo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) intencion.direccionMovimiento.x -= 1.0f; //flecha izquierda
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) intencion.direccionMovimiento.x += 1.0f; //flecha derecha

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Numpad0)) { //si pulsa el 0
        intencion.intentandoAtacar = true; //marca que el jugador 2 quiere atacar
    }

    return intencion; //se lo pasa a la arena para que lo procese
}