#include "MotorArchon.h"
#include "Tablero.h"
#include "Arena.h"
#include "MenuPrincipal.h"

MotorArchon::MotorArchon() {
    // Creación de la ventana principal según la normativa (Visual Studio 2026 / SFML)
    ventana.create(sf::VideoMode({ 800, 800 }), "ARCHON - Informática Industrial 2026");
    estadoActual = EstadoJuego::MENU;

    // Inicialización de punteros de control
    pantallaActiva = nullptr;
    jugador1 = nullptr;
    jugador2 = nullptr;
    ejecutando = true;
}

MotorArchon::~MotorArchon() {
    // Liberación de memoria dinámica para evitar fugas (Memory Leaks)
    if (pantallaActiva != nullptr) delete pantallaActiva;
    if (jugador1 != nullptr) delete jugador1;
    if (jugador2 != nullptr) delete jugador2;
}

void MotorArchon::cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1, Pieza* p2) {
    // 1. Limpieza de la pantalla anterior
    if (pantallaActiva != nullptr) {
        delete pantallaActiva;
        pantallaActiva = nullptr;
    }

    estadoActual = nuevoEstado;

    // 2. Instanciación de la nueva pantalla según la jerarquía del esquema[cite: 2]
    switch (estadoActual) {
    case EstadoJuego::MENU:
        pantallaActiva = new MenuPrincipal();
        break;

    case EstadoJuego::TABLERO:
        pantallaActiva = new Tablero();
        break;

    case EstadoJuego::ARENA:
        // Se pasan los punteros de las piezas que entran en conflicto
        if (p1 != nullptr && p2 != nullptr) {
            pantallaActiva = new Arena(p1, p2);
        }
        else {
            // Seguridad: Si no hay piezas, volvemos al tablero
            estadoActual = EstadoJuego::TABLERO;
            pantallaActiva = new Tablero();
        }
        break;

    case EstadoJuego::FIN:
        ejecutando = false;
        break;
    }
}

void MotorArchon::bucle() {
    while (ejecutando && ventana.isOpen()) {
        if (pantallaActiva != nullptr) {

            // --- 1. PROCESAMIENTO DE ENTRADA ---
            pantallaActiva->procesarEntrada(ventana);

            // --- 2. LÓGICA DE TRANSICIÓN ---
            // Transición de MENÚ a TABLERO mediante tecla Enter
            if (estadoActual == EstadoJuego::MENU && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                cambiarEstado(EstadoJuego::TABLERO);
            }

            // Gestión de combate detectado por la lógica del Tablero
            if (estadoActual == EstadoJuego::TABLERO) {
                // Cast seguro para acceder a los métodos específicos de la clase Tablero[cite: 2]
                Tablero* tab = dynamic_cast<Tablero*>(pantallaActiva);

                if (tab != nullptr && tab->getHaycombate()) {
                    Pieza* atacante = tab->getAtacante();
                    Pieza* defensor = tab->getDefensor();

                    // Reseteamos el flag de combate antes de la transición
                    tab->resetCombate();

                    // Transición a la Arena enviando los datos de combate
                    cambiarEstado(EstadoJuego::ARENA, atacante, defensor);
                }
            }

            // --- 3. RENDERIZADO ---
            ventana.clear(); // Limpieza del buffer

            // Gracias al polimorfismo, dibujamos la pantalla activa actual[cite: 2]
            pantallaActiva->dibujarPantalla(ventana);

            ventana.display(); // Intercambio de buffers
        }
    }
}

void MotorArchon::inicializar() {
    // Configuración inicial del sistema de jugadores
    // Según el esquema, Jugador es abstracta, aquí se instanciarían JugadorHumano o JugadorIA[cite: 2]
    cambiarEstado(EstadoJuego::MENU);

    // Por ahora, inicialización genérica (esto debe evolucionar según el bando elegido)
    // jugador1 = new JugadorHumano(...); 
    // jugador2 = new JugadorIA(...);
}