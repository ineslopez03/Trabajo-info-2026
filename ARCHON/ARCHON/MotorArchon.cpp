#include "MotorArchon.h"
#include "Tablero.h"
#include "Arena.h"
#include "MenuPrincipal.h"
#include <optional> // Necesario para std::optional en SFML 3

MotorArchon::MotorArchon() {
    // Creación de la ventana principal según la normativa (Visual Studio 2026 / SFML 3)
    ventana.create(sf::VideoMode({ 800, 800 }), "ARCHON - Informatica Industrial 2026");
    estadoActual = EstadoJuego::MENU;

    // Inicialización de punteros de control
    pantallaActiva = nullptr;
    jugador1 = nullptr;
    jugador2 = nullptr;
    ejecutando = true;
}

MotorArchon::~MotorArchon() {
    // Liberación de memoria dinámica para evitar fugas (Memory Leaks) 
    // Es vital liberar los jugadores ya que el Motor es el dueño de su ciclo de vida (Composición) 
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

    // 2. Instanciación de la nueva pantalla según la jerarquía del esquema 
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

        // --- GESTIÓN DE EVENTOS (CORRECCIÓN SFML 3) ---
        // pollEvent ahora devuelve un std::optional<sf::Event> y no acepta argumentos.
        while (const std::optional<sf::Event> evento = ventana.pollEvent()) {
            // Comprobamos el tipo de evento usando el método .is<T>()
            if (evento->is<sf::Event::Closed>()) {
                ventana.close();
            }
        }

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
                // Cast seguro para acceder a los métodos específicos de la clase Tablero
                Tablero* tab = dynamic_cast<Tablero*>(pantallaActiva);

                if (tab != nullptr && tab->getHaycombate()) {
                    Pieza* pAtacante = tab->getAtacante();
                    Pieza* pDefensor = tab->getDefensor();

                    // Reseteamos el flag de combate antes de la transición
                    tab->resetCombate();

                    // Transición a la Arena enviando los datos de combate
                    cambiarEstado(EstadoJuego::ARENA, pAtacante, pDefensor);
                }
            }

            // --- 3. RENDERIZADO ---
            ventana.clear(); // Limpieza del buffer 

            // Gracias al polimorfismo, dibujamos la pantalla activa actual
            pantallaActiva->dibujarPantalla(ventana);

            ventana.display(); // Intercambio de buffers 
        }
    }
}

void MotorArchon::inicializar() {
    // Configuracion de la ventana para SFML 3
    ventana.create(sf::VideoMode({ 800, 800 }), "ARCHON 2026");

    // Instanciacion de las especializaciones segun el esquema
    jugador1 = new JugadorHumano("Heroe de la Luz", Bando::LUZ);
    jugador2 = new JugadorIA("Senor de la Oscuridad", Bando::OSCURIDAD);

    // Estado inicial
    cambiarEstado(EstadoJuego::MENU);
}
