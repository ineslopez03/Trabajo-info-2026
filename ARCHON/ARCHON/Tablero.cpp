#include "Tablero.h"
#include <iostream>
#include <cmath>

Tablero::Tablero(sf::RenderWindow& ventana)
    : ventanaJuego(ventana), origenSeleccionado(nullptr), primerClicRealizado(false), anguloRotacion(0.0f) {
    inicializarTablero();
}

Tablero::~Tablero() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            delete matriz[i][j];
        }
    }
}
void Tablero::inicializarTablero() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // 1. Identificar Puntos de Poder (Amarillos fijos)
            // Son las 4 esquinas, los centros de los 4 bordes y el centro total.
            bool esPuntoPoder = (i == 0 || i == 4 || i == 8) && (j == 0 || j == 4 || j == 8);

  
            bool oscila = false;

            
            if (i == 4 || j == 4) {
                oscila = true;
            }
         
            else if ((i == 1 || i == 7) && (j == 2 || j == 6)) {
                oscila = true;
            }
            else if ((i == 2 || i == 6) && (j == 1 || j == 7)) {
                oscila = true;
            }

            // Si es punto de poder, NO oscila 
            if (esPuntoPoder) oscila = false;

            matriz[i][j] = new Casilla(i, j, oscila);
        }
    }
}
void Tablero::dibujarPantalla() {
    sf::View vistaTablero = ventanaJuego.getDefaultView();
    float tamCasilla = 60.0f; // Ajustado de 80 a 60
    float tamTotal = 9 * tamCasilla;

    vistaTablero.setCenter({ tamTotal / 2.f, tamTotal / 2.f });
    vistaTablero.setRotation(sf::degrees(anguloRotacion));
    ventanaJuego.setView(vistaTablero);


    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j]->dibujar(ventanaJuego, origenSeleccionado,tiempoAcumuladoOscilacion, tamCasilla);
        }
    }
    ventanaJuego.setView(ventanaJuego.getDefaultView());
}

bool Tablero::estaEnRango(Casilla* origen, Casilla* destino) {
    if (!origen->getPieza()) return false;
    int dist = std::abs(origen->getX() - destino->getX()) + std::abs(origen->getY() - destino->getY());
    return dist <= origen->getPieza()->getVelmov();
}

bool Tablero::esMovimientoValido(Casilla* origen, Casilla* destino) {
    if (destino->estaOcupada()) return false;
    return estaEnRango(origen, destino);
}

bool Tablero::esAtaqueValido(Casilla* origen, Casilla* destino) {
    if (!destino->estaOcupada()) return false;
    if (origen->getPieza()->getBando() == destino->getPieza()->getBando()) return false;
    return estaEnRango(origen, destino);
}

void Tablero::gestionarTurno(Casilla* origen, Casilla* destino) {
    if (esMovimientoValido(origen, destino)) {
        destino->setPieza(origen->getPieza());
        origen->setPieza(nullptr);
        turnosContados++;
        if (turnosContados >= 2) {
            // Avanzamos el "reloj" virtual del tablero en 2 segundos
            tiempoAcumuladoOscilacion += Duracion_fase;
            turnosContados = 0; // Reiniciamos el ciclo de turnos
        }
    }
    else if (esAtaqueValido(origen, destino)) {
        this->atacante = origen->getPieza();
        this->defensor = destino->getPieza();
        this->hayCombatePendiente = true;
    }
}

void Tablero::procesarEntrada() {
    // 1. Rotación con tecla Espacio
    static bool espacioPresionado = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        if (!espacioPresionado) { // Para que solo rote una vez por pulsación
            anguloRotacion += 180.0f;
            espacioPresionado = true;
        }
    }
    else {
        espacioPresionado = false;
    }

    // 2. Gestión de clics del ratón
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);

        // CRITICAL: Mapear píxeles a coordenadas de la vista rotada
        sf::View vistaActual = ventanaJuego.getView(); // Obtenemos la vista con rotación
        sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaActual);

        int celdaX = (int)(posMapeada.x / 60);
        int celdaY = (int)(posMapeada.y / 60);

        if (celdaX >= 0 && celdaX < 9 && celdaY >= 0 && celdaY < 9) {
            Casilla* casillaClic = matriz[celdaX][celdaY];

            if (!primerClicRealizado) {
                if (casillaClic->estaOcupada()) {
                    origenSeleccionado = casillaClic;
                    primerClicRealizado = true;
                }
            }
            else {
                gestionarTurno(origenSeleccionado, casillaClic);
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                comprobarVictoria();
            }
        }
    }
}

bool Tablero::comprobarVictoria() {
    int piezasLuz = 0, piezasOscuras = 0;
    int puntosLuz = 0, puntosOscuros = 0;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Casilla* c = matriz[i][j];
            if (c->estaOcupada()) {
                Pieza* p = c->getPieza();
                if (p->getBando() == Bando::LUZ) piezasLuz++;
                else piezasOscuras++;

                // Puntos de Poder (Esquinas y centros de bordes/tablero)
                bool esPuntoPoder = (i == 0 || i == 4 || i == 8) && (j == 0 || j == 4 || j == 8);
                if (esPuntoPoder) {
                    if (p->getBando() == Bando::LUZ) puntosLuz++;
                    else puntosOscuros++;
                }
            }
        }
    }

    if (puntosLuz == 5 || piezasOscuras == 0) return true;
    if (puntosOscuros == 5 || piezasLuz == 0) return true;

    return false;
}