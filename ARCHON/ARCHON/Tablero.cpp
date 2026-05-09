#include "Tablero.h"
#include <iostream>
#include <optional> 
#include <cmath>
#include "Peon.h"
#include "Casilla.h"
#include"Gigante.h"

// Constructor por defecto
Tablero::Tablero() : Tablero(95.0f) {}

Tablero::Tablero(float _tamano) {
    tamCasilla = _tamano;
    turnoActual = Bando::LUZ;
    primerClicRealizado = false;
    origenSeleccionado = nullptr;
    hayCombatePendiente = false;
    atacante = nullptr;
    defensor = nullptr;
    turnosContados = 0;

    // 1. Creamos la matriz de casillas
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j] = new Casilla(i, j);
        }
    }

    // 2. Colocamos las piezas iniciales
    inicializarTablero();

    // 3. CONFIGURACIÓN DE LA VISTA 
    float tamLogico = 9.0f * tamCasilla;
    vistaEstatica.setSize({ tamLogico, tamLogico });

    vistaEstatica.setCenter({ tamLogico / 2.f, tamLogico / 2.f });

   
    vistaEstatica.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
}

Tablero::~Tablero() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            delete matriz[i][j];
        }
    }
}

void Tablero::inicializarTablero() {
    for (int j = 0; j < 9; j++) {
        matriz[1][j]->setPieza(new Peon(Bando::LUZ));
        matriz[7][j]->setPieza(new Peon(Bando::OSCURIDAD));
    }
    matriz[0][2]->setPieza(new Gigante(Bando::LUZ));
    matriz[0][6]->setPieza(new Gigante(Bando::LUZ));
    matriz[8][2]->setPieza(new Gigante(Bando::OSCURIDAD));
    matriz[8][6]->setPieza(new Gigante(Bando::OSCURIDAD));
}

void Tablero::procesarEntrada(sf::RenderWindow& ventanaJuego) {
   
    while (const std::optional<sf::Event> evento = ventanaJuego.pollEvent()) {

        if (evento->is<sf::Event::Closed>()) {
            ventanaJuego.close();
        }

        if (const auto* mouseClick = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button == sf::Mouse::Button::Left) {

               
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);

               
                sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaEstatica);

         
                int celdaX = static_cast<int>(std::floor(posMapeada.x / tamCasilla));
                int celdaY = static_cast<int>(std::floor(posMapeada.y / tamCasilla));

                // Debug para consola
                std::cout << "Click en Celda: [" << celdaX << "," << celdaY << "]" << std::endl;

                if (celdaX >= 0 && celdaX < 9 && celdaY >= 0 && celdaY < 9) {
                    gestionarTurno(matriz[celdaX][celdaY]);
                }
            }
        }
    }
}

void Tablero::gestionarTurno(Casilla* casillaClicada) {
    if (!primerClicRealizado) {
        Pieza* p = casillaClicada->getPieza();
        // Verificamos que haya pieza y sea su turno
        if (p != nullptr && p->getBando() == turnoActual) {
            origenSeleccionado = casillaClicada;
            primerClicRealizado = true;
            std::cout << "Pieza seleccionada!" << std::endl;
        }
    }
    else {
        // Si clicamos la misma casilla, cancelamos
        if (origenSeleccionado == casillaClicada) {
            primerClicRealizado = false;
            origenSeleccionado = nullptr;
            return;
        }

        // Lógica de Movimiento y Ataque
        if (esMovimientoValido(origenSeleccionado, casillaClicada)) {

            // ¿Es un ataque o un movimiento a una casilla vacía?
            if (casillaClicada->estaOcupada()) {
                // ¡ES UN ATAQUE! Preparamos las variables para que MotorArchon lo detecte
                atacante = origenSeleccionado->getPieza();
                defensor = casillaClicada->getPieza();
                hayCombatePendiente = true;

                std::cout << "¡Combate iniciado! Pasando a la Arena..." << std::endl;

                // Limpiamos la selección del ratón, pero NO movemos la pieza en el tablero aún.
                // Eso se hará cuando alguien gane la batalla.
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                // Nota: El cambio de turno se hará en tu función resetCombate()

            }
            else {
                // MOVIMIENTO NORMAL A CASILLA VACÍA
                casillaClicada->setPieza(origenSeleccionado->getPieza());
                origenSeleccionado->setPieza(nullptr);

                // Reset de estado y cambio de turno
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
                turnosContados++;

                std::cout << "Movimiento exitoso. Turno del oponente." << std::endl;
            }

        }
        else if (casillaClicada->getPieza() && casillaClicada->getPieza()->getBando() == turnoActual) {
            // Si clicamos otra pieza nuestra válida, cambiamos la selección
            origenSeleccionado = casillaClicada;
        }
        else {
            // Clic en lugar inválido: deseleccionamos
            primerClicRealizado = false;
            origenSeleccionado = nullptr;
        }
    }
}

bool Tablero::esMovimientoValido(Casilla* origen, Casilla* destino) {
   
    if (!origen || !destino) return false;
    Pieza* p = origen->getPieza();
    if (p == nullptr) return false;
    return p->mover(origen, destino, matriz);
}
bool Tablero::esAtaqueValido(Casilla* origen, Casilla* destino) {
    if (!origen || !destino || !destino->estaOcupada()) return false;
    if (origen->getPieza()->getBando() != destino->getPieza()->getBando()) {
        return true; // Por ahora simplificado para habilitar la Arena
    }
    return false;
}

void Tablero::resetCombate() {
    hayCombatePendiente = false;
    atacante = nullptr;
    defensor = nullptr;
    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
}

void Tablero::dibujarPantalla(sf::RenderWindow& ventanaJuego) {
    // 1. Aplicamos la vista antes de dibujar nada
    ventanaJuego.setView(vistaEstatica);

    // 2. Dibujamos las casillas (ellas dibujarán sus piezas)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j]->dibujar(ventanaJuego, origenSeleccionado, turnosContados, tamCasilla);
        }
    }
}