#include "GestorVictoria.h"
#include "Tablero.h"
#include <SFML/Graphics.hpp>

int GestorVictoria::verificarVictoria(Tablero* tablero) {
    int contadorLuz = 0, libresLuz = 0, nodosLuz = 0;
    int contadorOsc = 0, libresOsc = 0, nodosOsc = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Casilla* c = tablero->matriz[i][j];
            if (!c->estaOcupada()) continue; 

            Pieza* p = c->getPieza();
            if (p->getBando() == Bando::LUZ) {
                contadorLuz++;
                if (!p->estaEncarcelada()) libresLuz++;
                if (c->getEsPuntoDePoder())  nodosLuz++;
            }
            else {
                contadorOsc++;
                if (!p->estaEncarcelada()) libresOsc++;
                if (c->getEsPuntoDePoder())  nodosOsc++;
            }
        }
    }
    if (nodosLuz == 5) { tablero->motivoVictoria = 1; return 1; }
    if (nodosOsc == 5) { tablero->motivoVictoria = 1; return 2; }
    bool pierdeLuz = (contadorLuz == 0) || (contadorLuz == 1 && libresLuz == 0);
    bool pierdeOsc = (contadorOsc == 0) || (contadorOsc == 1 && libresOsc == 0);
    if (pierdeLuz && pierdeOsc) { tablero->motivoVictoria = 2; return 3; }
    if (pierdeLuz) { tablero->motivoVictoria = (contadorLuz == 0) ? 2 : 3; return 2; }
    if (pierdeOsc) { tablero->motivoVictoria = (contadorOsc == 0) ? 2 : 3; return 1; }

    return 0;
}

void GestorVictoria::actualizarFaseVictoria(Tablero* tablero, float dt) {
    tablero->temporizadorFase -= dt;

    if (tablero->temporizadorFase <= 0.f) {
        if (tablero->faseVictoria == 1) {
            tablero->faseVictoria = 2;
            tablero->temporizadorFase = 1.5f;
        }
        else if (tablero->faseVictoria == 2) {
            tablero->faseVictoria = 3;
            tablero->temporizadorFase = 4.0f;
        }
        else if (tablero->faseVictoria == 3) {
            tablero->volverAlMenu = true;
        }
    }
}

void GestorVictoria::dibujarFaseVictoria(Tablero* tablero, sf::RenderWindow& ventana) {
    sf::RectangleShape fondoNegro(tablero->vistaEstatica.getSize());
    fondoNegro.setOrigin({ tablero->vistaEstatica.getSize().x / 2.f,
                           tablero->vistaEstatica.getSize().y / 2.f });
    fondoNegro.setPosition(tablero->vistaEstatica.getCenter());
    fondoNegro.setFillColor(sf::Color::Black);
    ventana.draw(fondoNegro);
    if (tablero->estadoVictoria == 1) { tablero->textoVictoria.setString("GANADOR BANDO DE LUZ");       tablero->textoVictoria.setFillColor(sf::Color::Cyan); }
    else if (tablero->estadoVictoria == 2) { tablero->textoVictoria.setString("GANADOR BANDO DE OSCURIDAD"); tablero->textoVictoria.setFillColor(sf::Color::Red); }
    else { tablero->textoVictoria.setString("EMPATE ESTRUCTURAL");          tablero->textoVictoria.setFillColor(sf::Color::Yellow); }
    sf::FloatRect limites = tablero->textoVictoria.getLocalBounds();
    tablero->textoVictoria.setOrigin({ limites.size.x / 2.f, limites.size.y / 2.f });
    tablero->textoVictoria.setPosition(tablero->vistaEstatica.getCenter());
    ventana.draw(tablero->textoVictoria);
}