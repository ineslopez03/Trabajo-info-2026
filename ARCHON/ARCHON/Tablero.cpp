#include "Tablero.h"
#include <iostream>
#include <optional> 
#include <cmath>
#include "Peon.h"
#include "Casilla.h"
#include"Gigante.h"
#include"Fenix.h"
#include"Rango.h"
#include"Hechicero.h"
// Constructor por defecto
Tablero::Tablero() : Tablero(95.0f) {}

Tablero::Tablero(float _tamano) {
    tamCasilla = _tamano;
    turnoActual = Bando::LUZ;
    primerClicRealizado = false;
    origenSeleccionado = nullptr;
    piezaAuxiliar = nullptr;
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
    for (int i = 0; i < 8; i++) {
        hechizosLuzUsados[i] = false;
        hechizosOscurosUsados[i] = false;
    }
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

    matriz[0][1]->setPieza(new Fenix(Bando::LUZ));
    matriz[8][1]->setPieza(new Fenix(Bando::OSCURIDAD));
    matriz[0][7]->setPieza(new Fenix(Bando::LUZ));
    matriz[8][7]->setPieza(new Fenix(Bando::OSCURIDAD));

    matriz[0][0]->setPieza(new Rango(Bando::LUZ));
    matriz[0][3]->setPieza(new Rango(Bando::LUZ));
    matriz[0][5]->setPieza(new Rango(Bando::LUZ));
    matriz[0][8]->setPieza(new Rango(Bando::LUZ));
  
    matriz[8][0]->setPieza(new Rango(Bando::OSCURIDAD));
    matriz[8][3]->setPieza(new Rango(Bando::OSCURIDAD));
    matriz[8][5]->setPieza(new Rango(Bando::OSCURIDAD));
    matriz[8][8]->setPieza(new Rango(Bando::OSCURIDAD));

    matriz[0][4]->setPieza(new Hechicero(Bando::LUZ));
    matriz[8][4]->setPieza(new Hechicero(Bando::OSCURIDAD));


}

void Tablero::procesarEntrada(sf::RenderWindow& ventanaJuego) {
    while (const std::optional<sf::Event> evento = ventanaJuego.pollEvent()) {

        if (evento->is<sf::Event::Closed>()) {
            ventanaJuego.close();
        }

        // --- ENTRADA DE TECLADO (Hechizos) ---
        if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>()) {

            // Solo intentamos activar hechizos si hay una pieza seleccionada
            if (primerClicRealizado && origenSeleccionado != nullptr) {
                Pieza* p = origenSeleccionado->getPieza();

                // Comprobamos si la pieza seleccionada es un Hechicero/Hechicera
                // (Asumiendo que tus clases se llaman Hechicero)
                if (p != nullptr && dynamic_cast<Hechicero*>(p) != nullptr) {

                    bool teclaHechizo = false;

                    if (teclaPresionada->code == sf::Keyboard::Key::Num1) { hechizoSeleccionado = 1; teclaHechizo = true; }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Num2) { hechizoSeleccionado = 2; teclaHechizo = true; }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Num3) { hechizoSeleccionado = 3; teclaHechizo = true; }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Num4) { hechizoSeleccionado = 4; teclaHechizo = true; }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Num5) { hechizoSeleccionado = 5; teclaHechizo = true; }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Num6) { hechizoSeleccionado = 6; teclaHechizo = true; }
                    else if (teclaPresionada->code == sf::Keyboard::Key::Num7) { hechizoSeleccionado = 7; teclaHechizo = true; }

                    if (teclaHechizo) {
                        modoHechizoActivo = true;
                        std::cout << "MODO HECHIZO ACTIVADO: " << hechizoSeleccionado << std::endl;

                        if (hechizoSeleccionado == 3) {
                            // No pedimos clic, lo lanzamos directamente
                            std::cout << "Lanzando Shift Time inmediatamente..." << std::endl;
                            procesarMagia(nullptr);
                        }
                        else {
                            // Solo pedimos clic para los demás (Teleport, Heal, etc.)
                            std::cout << "Haz clic en el objetivo para lanzar la magia." << std::endl;
                        }
                    
                    }
                }
            }

            // Tecla opcional para cancelar selección o hechizo
            if (teclaPresionada->code == sf::Keyboard::Key::Escape) {
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                modoHechizoActivo = false;
                std::cout << "Seleccion cancelada." << std::endl;
            }
        }

        // --- ENTRADA DE RATÓN (Selección y Movimiento) ---
        if (const auto* mouseClick = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button == sf::Mouse::Button::Left) {

                sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);
                sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaEstatica);

                int celdaX = static_cast<int>(std::floor(posMapeada.x / tamCasilla));
                int celdaY = static_cast<int>(std::floor(posMapeada.y / tamCasilla));

                if (celdaX >= 0 && celdaX < 9 && celdaY >= 0 && celdaY < 9) {
                    // Llamamos a gestionarTurno, que ahora sabe manejar el modoHechizoActivo
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
            if (p->estaEncarcelada()) {
                std::cout << "Esta pieza esta presa. No puedes seleccionarla." << std::endl;
                return; // No ponemos primerClicRealizado = true, el turno sigue
            }
            origenSeleccionado = casillaClicada;
            primerClicRealizado = true;
            if (dynamic_cast<Hechicero*>(p) != nullptr) {
                bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;
                std::cout << "\n--- HECHIZOS DISPONIBLES ---" << std::endl;

                if (!registro[1]) std::cout << "[1] Teleport ";
                if (!registro[2]) std::cout << "[2] Heal ";
                if (!registro[3]) std::cout << "[3] Shift Time ";
                if (!registro[4]) std::cout << "[4] Exchange ";
                if (!registro[5]) std::cout << "[5] Summon ";
                if (!registro[6]) std::cout << "[6] Revive ";
                if (!registro[7]) std::cout << "[7] Imprison ";

                std::cout << "\n----------------------------" << std::endl;
            }
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
        if (modoHechizoActivo)
        {
            procesarMagia(casillaClicada);
            return;
        }
        // Lógica de Movimiento y Ataque
        if (esMovimientoValido(origenSeleccionado, casillaClicada)) {

            // ¿Es un ataque o un movimiento a una casilla vacía?
            if (casillaClicada->estaOcupada()) {
                // ¡ES UN ATAQUE! Preparamos las variables para que MotorArchon lo detecte
                atacante = origenSeleccionado->getPieza();
                defensor = casillaClicada->getPieza();
                ColorCasilla colorDeCombate = casillaClicada->getColorActual();
                float vEfectivaDefensor = defensor->getVidaEfectiva(colorDeCombate);
                float vEfectivaAtacante = atacante->getVidaEfectiva(colorDeCombate);
                hayCombatePendiente = true;
                turnosContados++;

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
                finalizarTurno();
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
    turnosContados++;
    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
}

void Tablero::dibujarPantalla(sf::RenderWindow& ventanaJuego) {
    // 1. Aplicamos la vista antes de dibujar nada
    ventanaJuego.setView(vistaEstatica);

    // 2. Dibujamos las casillas (ellas dibujarán sus piezas)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j]->dibujar(ventanaJuego, origenSeleccionado, turnosContados, tamCasilla);
            if (matriz[i][j] == piezaAuxiliar) {
                sf::RectangleShape marcador(sf::Vector2f(tamCasilla - 10, tamCasilla - 10));
                marcador.setPosition( { i * tamCasilla + 5, j * tamCasilla + 5 });
                marcador.setOutlineThickness(4.0f);
                marcador.setOutlineColor(sf::Color::Yellow); // Amarillo para diferenciar del verde
                marcador.setFillColor(sf::Color::Transparent);
                ventanaJuego.draw(marcador);
            }
        }
    }
}
void Tablero::procesarMagia(Casilla* objetivo) {
    if (!modoHechizoActivo || !origenSeleccionado) return;

    // Verificar si ya se usó (1-7)
    bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;
    if (registro[hechizoSeleccionado]) {
        std::cout << "Hechizo ya agotado para este bando." << std::endl;
        modoHechizoActivo = false;
        return;
    }

    switch (hechizoSeleccionado) {
    case 1: // TELEPORT
        if (piezaAuxiliar == nullptr) {
            // PASO 1: Elegir la pieza a mover
            if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == turnoActual) {
                piezaAuxiliar = objetivo;
                std::cout << "Pieza seleccionada para Teleport. Ahora elige el destino vacio." << std::endl;
                return; // Esperamos al siguiente clic
            }
            else {
                std::cout << "Debes elegir una pieza ALIADA para teletransportar." << std::endl;
                return;
            }
        }
        else {
            // PASO 2: Elegir el destino
            if (!objetivo->estaOcupada()) {
                // Movemos la pieza guardada al nuevo objetivo
                objetivo->setPieza(piezaAuxiliar->getPieza());
                piezaAuxiliar->setPieza(nullptr);

                registro[1] = true; // Hechizo gastado
                std::cout << "¡Teletransporte completado!" << std::endl;
                // No hacemos return aquí para que llegue al finalizarTurno()
            }
            else {
                std::cout << "El destino debe estar vacio. Elige otra casilla." << std::endl;
                return;
            }
        }
        break;

    case 2: // HEAL: Cura completamente (usa el resetVida que añadimos)
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == turnoActual) {
            objetivo->getPieza()->resetVida();
            registro[2] = true;
        }
        break;

    case 3: // SHIFT TIME: Altera el ciclo (Invierte colores)
        this->turnosContados += 3;
        registro[3] = true;
        break;

    case 4: // EXCHANGE
        if (objetivo->estaOcupada()) {
            if (piezaAuxiliar == nullptr) {
                piezaAuxiliar= objetivo;
                std::cout << "Pieza A marcada. Elige la Pieza B para el intercambio." << std::endl;
                return; // Esperamos al siguiente clic del ratón
            }
            else {
                // Realizar el cambio
                Pieza* p1 = piezaAuxiliar->getPieza();
                Pieza* p2 = objetivo->getPieza();
                piezaAuxiliar->setPieza(p2);
                objetivo->setPieza(p1);

                registro[4] = true; // Solo aquí marcamos como usado
                std::cout << "¡Intercambio completado!" << std::endl;
            }
        }
        break;

        //case 5: Todavía no se puede implementar

  // case 6: Todavía no se puede implementar

    case 7: // IMPRISON: Bloquea pieza enemiga
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() != turnoActual) {
            objetivo->getPieza()->setEncarcelada(3);
            registro[7] = true;
        }
        break;
        
        }
    // Finalizar turno tras la magia
    if (registro[hechizoSeleccionado]) {

        finalizarTurno();
    }
}
    void Tablero::finalizarTurno() {
        // 1. Reducir contadores de bloqueo (Imprison) de TODAS las piezas
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (matriz[i][j]->estaOcupada()) {
                    matriz[i][j]->getPieza()->pasarTurnoBloqueo();
                }
            }
        }

        // 2. Cambio de bando y avance del tiempo global
        turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
        turnosContados++;

        // 3. Resetear estados de selección para el siguiente jugador
        primerClicRealizado = false;
        origenSeleccionado = nullptr;
        modoHechizoActivo = false;
        piezaAuxiliar = nullptr;

        std::cout << "Turno finalizado. Tiempo actual: " << turnosContados << std::endl;
    }
