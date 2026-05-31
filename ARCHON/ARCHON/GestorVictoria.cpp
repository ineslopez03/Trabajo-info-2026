#include "GestorVictoria.h"
#include "Tablero.h"
#include <SFML/Graphics.hpp>

int GestorVictoria::verificarVictoria(Tablero* tablero) {
    int contadorLuz = 0, libresLuz = 0, nodosLuz = 0;
    int contadorOsc = 0, libresOsc = 0, nodosOsc = 0;
    // Recorre el tablero contando piezas, piezas libres y nodos controlados de cada bando
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Casilla* c = tablero->matriz[i][j];// Casilla actual
            if (!c->estaOcupada()) continue;// casilla vacía, se salta
            Pieza* p = c->getPieza(); // Obtiene la pieza
            if (p->getBando() == Bando::LUZ) { // Si es del bando LUZ
                contadorLuz++; // Incrementa el total de LUZ
                if (!p->estaEncarcelada()) libresLuz++;// Si no está encarcelada, cuenta como libre
                if (c->getEsPuntoDePoder())  nodosLuz++;// controla un nodo de poder
            }
            else {
                contadorOsc++;
                if (!p->estaEncarcelada()) libresOsc++;
                if (c->getEsPuntoDePoder())  nodosOsc++;
            }
        }
    }
    // Condición 1 (nodos): un bando ocupa los 5 puntos de poder → gana
    if (nodosLuz == 5) { tablero->motivoVictoria = 1; return 1; }
    if (nodosOsc == 5) { tablero->motivoVictoria = 1; return 2; }
    // Un bando pierde si no tiene piezas o su única pieza está encarcelada
    bool pierdeLuz = (contadorLuz == 0) || (contadorLuz == 1 && libresLuz == 0);
    bool pierdeOsc = (contadorOsc == 0) || (contadorOsc == 1 && libresOsc == 0);
    // Condición 2/3 (empate): los dos bandos pierden a la vez
    if (pierdeLuz && pierdeOsc) { tablero->motivoVictoria = 2; return 3; }
    // motivoVictoria 2=aniquilación (0 piezas), 3=prisión (última pieza encarcelada)
    if (pierdeLuz) { tablero->motivoVictoria = (contadorLuz == 0) ? 2 : 3; return 2; }
    if (pierdeOsc) { tablero->motivoVictoria = (contadorOsc == 0) ? 2 : 3; return 1; }

    return 0;// nadie ha ganado todavía
}

void GestorVictoria::actualizarFaseVictoria(Tablero* tablero, float dt) {
    tablero->temporizadorFase -= dt;// descuenta el tiempo transcurrido este frame

    if (tablero->temporizadorFase <= 0.f) {
        if (tablero->faseVictoria == 1) {
            // Fase 1 terminó (delay del bloqueo azul) → pasa al parpadeo verde
            tablero->faseVictoria = 2;
            tablero->temporizadorFase = 1.5f;
        }
        else if (tablero->faseVictoria == 2) {
            // Parpadeo verde terminó → pasa a pantalla negra con el texto del ganador
            tablero->faseVictoria = 3;
            tablero->temporizadorFase = 4.0f;
        }
        else if (tablero->faseVictoria == 3) {
            // Texto terminó → señala a MotorArchon que debe volver al menú
            tablero->volverAlMenu = true;
        }
    }
}

void GestorVictoria::dibujarFaseVictoria(Tablero* tablero, sf::RenderWindow& ventana) {
    // Fondo negro que cubre todo el área visible del tablero
    sf::RectangleShape fondoNegro(tablero->vistaEstatica.getSize());
    fondoNegro.setOrigin({ tablero->vistaEstatica.getSize().x / 2.f,
                           tablero->vistaEstatica.getSize().y / 2.f });
    fondoNegro.setPosition(tablero->vistaEstatica.getCenter());
    fondoNegro.setFillColor(sf::Color::Black);
    ventana.draw(fondoNegro);
    // Elige el mensaje y color según el resultado
    if (tablero->estadoVictoria == 1) { tablero->textoVictoria.setString("GANADOR BANDO DE LUZ");       tablero->textoVictoria.setFillColor(sf::Color::Cyan); }
    else if (tablero->estadoVictoria == 2) { tablero->textoVictoria.setString("GANADOR BANDO DE OSCURIDAD"); tablero->textoVictoria.setFillColor(sf::Color::Red); }
    else { tablero->textoVictoria.setString("EMPATE ESTRUCTURAL");          tablero->textoVictoria.setFillColor(sf::Color::Yellow); }
    // Centra el texto en la pantalla
    sf::FloatRect limites = tablero->textoVictoria.getLocalBounds();
    tablero->textoVictoria.setOrigin({ limites.size.x / 2.f, limites.size.y / 2.f });
    tablero->textoVictoria.setPosition(tablero->vistaEstatica.getCenter());
    ventana.draw(tablero->textoVictoria);
}