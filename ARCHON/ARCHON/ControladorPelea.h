#pragma once
#include <SFML/Window/Keyboard.hpp> 
#include <SFML/System/Vector2.hpp> 

//agrupa si el jugador quiere moverse y/o atacar
struct IntencionJugador {
    sf::Vector2f direccionMovimiento; //vector que indica hacia donde quiere caminar
    bool intentandoAtacar; //flag que se pone a true si presiona el boton de ataque
};


class ControladorPelea {
public:

    static IntencionJugador obtenerIntencionIzquierda(); //lee el teclado del jugador 1 (wasd)
    static IntencionJugador obtenerIntencionDerecha(); //lee el teclado del jugador 2 (flechitas)
};