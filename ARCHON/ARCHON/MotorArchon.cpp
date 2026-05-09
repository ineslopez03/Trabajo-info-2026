#include "MotorArchon.h"
#include "Tablero.h"
#include "Arena.h"
#include "MenuPrincipal.h"
#include <optional> 

MotorArchon::MotorArchon() {
    
    ventana.create(sf::VideoMode({ 800, 800 }), "ARCHON - Informatica Industrial 2026");
    estadoActual = EstadoJuego::MENU;

   
    pantallaActiva = nullptr;
    jugador1 = nullptr;
    jugador2 = nullptr;
    ejecutando = true;
}

MotorArchon::~MotorArchon() {
    
    if (pantallaActiva != nullptr) delete pantallaActiva;
    if (jugador1 != nullptr) delete jugador1;
    if (jugador2 != nullptr) delete jugador2;
}

void MotorArchon::cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1, Pieza* p2) {
  
    if (pantallaActiva != nullptr) {
        delete pantallaActiva;
        pantallaActiva = nullptr;
    }

    estadoActual = nuevoEstado;

    
    switch (estadoActual) {
    case EstadoJuego::MENU:
        pantallaActiva = new MenuPrincipal();
        break;

    case EstadoJuego::TABLERO:
        pantallaActiva = new Tablero();
        break;

    case EstadoJuego::ARENA:
       
        if (p1 != nullptr && p2 != nullptr) {
            pantallaActiva = new Arena(p1, p2);
        }
        else {
            
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
            // 1. EL TABLERO PROCESA LA ENTRADA
            pantallaActiva->procesarEntrada(ventana);

            // 2. LÓGICA DE TRANSICIÓN
            if (estadoActual == EstadoJuego::MENU && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                cambiarEstado(EstadoJuego::TABLERO);
            }

            if (estadoActual == EstadoJuego::TABLERO) {
                Tablero* tab = dynamic_cast<Tablero*>(pantallaActiva);
                if (tab != nullptr && tab->getHaycombate()) {
                    Pieza* pAtacante = tab->getAtacante();
                    Pieza* pDefensor = tab->getDefensor();
                    tab->resetCombate();
                    cambiarEstado(EstadoJuego::ARENA, pAtacante, pDefensor);
                }
            }

            // 3. RENDERIZADO
            ventana.clear();
            pantallaActiva->dibujarPantalla(ventana);
            ventana.display();
        }
    }
}
void MotorArchon::inicializar() {
    
    ventana.create(sf::VideoMode({ 900, 900 }), "ARCHON 2026");

   
    jugador1 = new JugadorHumano("Heroe de la Luz", Bando::LUZ);
    jugador2 = new JugadorIA("Senor de la Oscuridad", Bando::OSCURIDAD);


    cambiarEstado(EstadoJuego::MENU);
}
