#include "Tablero.h"
#include <iostream>
#include <optional>
#include <cmath>

// Inclusión de la nueva topología de entidades
#include "Casilla.h"
#include "Caballero.h"
#include "Golem.h"
#include "Valquiria.h"
#include "Unicornio.h"
#include "Arquero.h"
#include "Genio.h"
#include "Fenix.h"
#include "Hechicero.h"

Tablero::Tablero() : Tablero(95.0f, " ") {}

Tablero::Tablero(float _tamano, std::string skin) {
    tamCasilla = _tamano;
    skinActual = skin;
    turnoActual = Bando::LUZ;
    primerClicRealizado = false;
    origenSeleccionado = nullptr;
    piezaAuxiliar = nullptr;
    hayCombatePendiente = false;
    atacante = nullptr;
    defensor = nullptr;
    turnosContados = 0;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j] = new Casilla(i, j);
        }
    }

    inicializarTablero();

    float anchoVisible = 12.0f * tamCasilla;
    float altoVisible = 9.0f * tamCasilla;

    vistaEstatica.setSize({ anchoVisible, altoVisible });
    vistaEstatica.setCenter({ anchoVisible / 2.0f - 50.0f, altoVisible / 2.0f });
    vistaEstatica.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));

    for (int i = 0; i < 8; i++) {
        hechizosLuzUsados[i] = false;
        hechizosOscurosUsados[i] = false;
    }
    if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Ni siquiera encontre la fuente del sistema." << std::endl;
    }
    inicializarBotones();
}

Tablero::~Tablero() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            delete matriz[i][j];
        }
    }
    for (auto b : botonesHechizos) {
        delete b; 
    }
}

void Tablero::inicializarTablero() {
    // --- BANDO DE LA LUZ (Flanco Izquierdo) ---
    // Columna 0: Retaguardia
    matriz[0][0]->setPieza(new Valquiria(Bando::LUZ, skinActual));
    matriz[0][1]->setPieza(new Golem(Bando::LUZ, skinActual));
    matriz[0][2]->setPieza(new Unicornio(Bando::LUZ, skinActual));
    matriz[0][3]->setPieza(new Genio(Bando::LUZ, skinActual));
    matriz[0][4]->setPieza(new Hechicero(Bando::LUZ, skinActual)); // Nodo central de poder
    matriz[0][5]->setPieza(new Fenix(Bando::LUZ, skinActual));
    matriz[0][6]->setPieza(new Unicornio(Bando::LUZ, skinActual));
    matriz[0][7]->setPieza(new Golem(Bando::LUZ, skinActual));
    matriz[0][8]->setPieza(new Valquiria(Bando::LUZ, skinActual));

    // Columna 1: Vanguardia e Infantería Ligera
    matriz[1][0]->setPieza(new Arquero(Bando::LUZ, skinActual));
    for (int j = 1; j <= 7; j++) {
        matriz[1][j]->setPieza(new Caballero(Bando::LUZ, skinActual));
    }
    matriz[1][8]->setPieza(new Arquero(Bando::LUZ, skinActual));


    // --- BANDO DE LA OSCURIDAD (Flanco Derecho) ---
    // Columna 8: Retaguardia
    matriz[8][0]->setPieza(new Valquiria(Bando::OSCURIDAD, skinActual));
    matriz[8][1]->setPieza(new Golem(Bando::OSCURIDAD, skinActual));
    matriz[8][2]->setPieza(new Unicornio(Bando::OSCURIDAD, skinActual));
    matriz[8][3]->setPieza(new Genio(Bando::OSCURIDAD, skinActual));
    matriz[8][4]->setPieza(new Hechicero(Bando::OSCURIDAD, skinActual)); // Nodo central de poder
    matriz[8][5]->setPieza(new Fenix(Bando::OSCURIDAD, skinActual));
    matriz[8][6]->setPieza(new Unicornio(Bando::OSCURIDAD, skinActual));
    matriz[8][7]->setPieza(new Golem(Bando::OSCURIDAD, skinActual));
    matriz[8][8]->setPieza(new Valquiria(Bando::OSCURIDAD, skinActual));

    // Columna 7: Vanguardia e Infantería Ligera
    matriz[7][0]->setPieza(new Arquero(Bando::OSCURIDAD, skinActual));
    for (int j = 1; j <= 7; j++) {
        matriz[7][j]->setPieza(new Caballero(Bando::OSCURIDAD, skinActual));
    }
    matriz[7][8]->setPieza(new Arquero(Bando::OSCURIDAD, skinActual));
}

void Tablero::procesarEntrada(sf::RenderWindow& ventanaJuego) {
    while (const std::optional<sf::Event> evento = ventanaJuego.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventanaJuego.close();
        }

        // 1. Cancelar selección con Escape
        if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>()) {
            if (teclaPresionada->code == sf::Keyboard::Key::Escape) {
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                modoHechizoActivo = false;
                piezaAuxiliar = nullptr;
                std::cout << "Accion cancelada." << std::endl;
            }
        }

        // 2. Lógica de Clic Izquierdo
        if (const auto* mouseClick = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button == sf::Mouse::Button::Left) {
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);
                sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaEstatica);

                // --- CLIC EN BOTONES DE HECHIZOS ---
                if (primerClicRealizado && origenSeleccionado != nullptr) {
                    Pieza* p = origenSeleccionado->getPieza();
                    if (p != nullptr && dynamic_cast<Hechicero*>(p) != nullptr) {

                        bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;

                        // COMPROBACIÓN: ¿Ya ha usado algún hechizo este bando?
                        bool yaUsoMagia = false;
                        for (int j = 0; j < 8; j++) { if (registro[j]) yaUsoMagia = true; }

                        if (!yaUsoMagia) {
                            for (int i = 0; i < botonesHechizos.size(); i++) {
                                if (botonesHechizos[i]->botonContieneRaton(posMapeada)) {
                                    hechizoSeleccionado = i + 1; // 1 a 7
                                    modoHechizoActivo = true;

                                    std::cout << "Hechizo " << hechizoSeleccionado << " seleccionado." << std::endl;

                                    if (hechizoSeleccionado == 3) { // Shift Time es instantáneo
                                        procesarMagia(nullptr);
                                    }
                                    return; // Salimos para evitar clickear el tablero a la vez
                                }
                            }
                        }
                    }
                }

                // --- CLIC EN EL TABLERO ---
                int celdaX = static_cast<int>(std::floor(posMapeada.x / tamCasilla));
                int celdaY = static_cast<int>(std::floor(posMapeada.y / tamCasilla));

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
        if (p != nullptr && p->getBando() == turnoActual) {
            if (p->estaEncarcelada()) return;

            origenSeleccionado = casillaClicada;
            primerClicRealizado = true;

            // Feedback visual en consola sobre la magia
            if (dynamic_cast<Hechicero*>(p) != nullptr) {
                bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;
                bool yaUsoMagia = false;
                for (int j = 0; j < 8; j++) { if (registro[j]) yaUsoMagia = true; }

                if (yaUsoMagia) std::cout << "MAGIA AGOTADA PARA ESTE BANDO." << std::endl;
                else std::cout << "HECHICERO LISTO: Elige un hechizo de los botones." << std::endl;
            }
        }
    }
    else {
        if (modoHechizoActivo) {
            procesarMagia(casillaClicada);
            return;
        }
        if (origenSeleccionado == casillaClicada) {
            primerClicRealizado = false;
            origenSeleccionado = nullptr;
            return;
        }
        if (modoHechizoActivo) {
            procesarMagia(casillaClicada);
            return;
        }

        if (esMovimientoValido(origenSeleccionado, casillaClicada)) {
            if (casillaClicada->estaOcupada()) {

                // 1. Guardamos la posición usando los atributos de la casilla
                // Asegúrate de que tu clase Casilla tenga las variables fila y columna (o i, j)
                this->coordenadasCombate = sf::Vector2i(casillaClicada->getX(), casillaClicada->getY());

                // 2. Seteamos los combatientes
                this->atacante = origenSeleccionado->getPieza();
                this->defensor = casillaClicada->getPieza();

                // 3. Activamos el flag de combate
                this->hayCombatePendiente = true;

                // IMPORTANTE: Limpiamos la selección para que el motor pueda cambiar de estado
                primerClicRealizado = false;
                origenSeleccionado = nullptr;

                std::cout << "¡Combate iniciado! Destino guardado." << std::endl;
            }
            else {
                // Movimiento normal a casilla vacía
                casillaClicada->setPieza(origenSeleccionado->getPieza());
                origenSeleccionado->setPieza(nullptr);
                finalizarTurno();
            }
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
        return true;
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
    ventanaJuego.setView(vistaEstatica);

    // Dibujar Tablero y Casillas
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j]->dibujar(ventanaJuego, origenSeleccionado, turnosContados, tamCasilla);

            // Dibujar marcador amarillo si hay piezaAuxiliar (para Teleport/Exchange)
            if (matriz[i][j] == piezaAuxiliar) {
                sf::RectangleShape marcador(sf::Vector2f(tamCasilla - 10, tamCasilla - 10));
                marcador.setPosition({ i * tamCasilla + 5, j * tamCasilla + 5 });
                marcador.setOutlineThickness(4.0f);
                marcador.setOutlineColor(sf::Color::Yellow);
                marcador.setFillColor(sf::Color::Transparent);
                ventanaJuego.draw(marcador);
            }
        }
    }

    // Dibujar Botones solo si el Hechicero está seleccionado
    if (primerClicRealizado && dynamic_cast<Hechicero*>(origenSeleccionado->getPieza())) {
        bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;

        // Comprobar si ya se gastó el único uso
        bool yaSeUso = false;
        for (int j = 0; j < 8; j++) { if (registro[j]) yaSeUso = true; }

        sf::Vector2f mousePos = ventanaJuego.mapPixelToCoords(sf::Mouse::getPosition(ventanaJuego), vistaEstatica);

        for (int i = 0; i < botonesHechizos.size(); i++) {
            if (yaSeUso) {
                botonesHechizos[i]->setTexto("AGOTADO");
            }
            botonesHechizos[i]->actualizarColorBoton(mousePos);
            botonesHechizos[i]->dibujar(ventanaJuego);
        }
    }
}

void Tablero::procesarMagia(Casilla* objetivo) {
    if (!modoHechizoActivo || !origenSeleccionado) return;

    bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;
    if (registro[hechizoSeleccionado]) {
        std::cout << "Hechizo ya agotado para este bando." << std::endl;
        modoHechizoActivo = false;
        return;
    }

    switch (hechizoSeleccionado) {
    case 1:
        if (piezaAuxiliar == nullptr) {
            if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == turnoActual) {
                piezaAuxiliar = objetivo;
                std::cout << "Pieza seleccionada para Teleport. Ahora elige el destino vacio." << std::endl;
                return;
            }
            else {
                std::cout << "Debes elegir una pieza ALIADA para teletransportar." << std::endl;
                return;
            }
        }
        else {
            if (!objetivo->estaOcupada()) {
                objetivo->setPieza(piezaAuxiliar->getPieza());
                piezaAuxiliar->setPieza(nullptr);
                registro[1] = true;
                std::cout << "¡Teletransporte completado!" << std::endl;
            }
            else {
                std::cout << "El destino debe estar vacio. Elige otra casilla." << std::endl;
                return;
            }
        }
        break;

    case 2: // HEAL
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == turnoActual) {
            Pieza* p = objetivo->getPieza();

            // Comprobamos si está herido (incluso si es el propio Hechicero)
            if (p->getVidaBase() < p->getVidaMaxima()) {
                p->resetVida();
                registro[2] = true;

                // Limpiamos piezaAuxiliar por si acaso estaba activa
                piezaAuxiliar = nullptr;
                std::cout << "Hechizo Heal aplicado con exito." << std::endl;
            }
            else {
                // Si tiene la vida llena, no hacemos nada (no gasta turno)
                std::cout << "La pieza ya tiene salud maxima." << std::endl;
            }
        }
        break;
    case 3:
        turnosContados += 3;
        registro[3] = true;
        break;

    case 4:
        if (objetivo->estaOcupada()) {
            if (piezaAuxiliar == nullptr) {
                piezaAuxiliar = objetivo;
                std::cout << "Pieza A marcada. Elige la Pieza B para el intercambio." << std::endl;
                return;
            }
            else {
                Pieza* p1 = piezaAuxiliar->getPieza();
                Pieza* p2 = objetivo->getPieza();
                piezaAuxiliar->setPieza(p2);
                objetivo->setPieza(p1);

                registro[4] = true;
                std::cout << "¡Intercambio completado!" << std::endl;
            }
        }
        break;
    case 5: // SUMMON
    {
        if (piezaAuxiliar == nullptr) {
            // Primer clic: Seleccionar la pieza aliada que queremos "invocar" (traer hacia nosotros)
            if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == turnoActual) {
                piezaAuxiliar = objetivo;
                std::cout << "Pieza seleccionada para invocar. Elige casilla vacia junto al Hechicero." << std::endl;
                return;
            }
        }
        else {
            // Segundo clic: Casilla de destino
            if (!objetivo->estaOcupada()) {
                // Movemos la pieza de su sitio original al nuevo
                objetivo->setPieza(piezaAuxiliar->getPieza());
                piezaAuxiliar->setPieza(nullptr);

                registro[5] = true;
                std::cout << "¡Invocacion completada!" << std::endl;
                piezaAuxiliar = nullptr;
            }
            else {
                std::cout << "El destino de la invocacion debe estar vacio." << std::endl;
                return;
            }
        }
    }
    break;
    case 6: // REVIVE
    {
        // 1. Obtener la lista de muertos del bando actual
        std::vector<Pieza*>& cementerio = (turnoActual == Bando::LUZ) ? piezasMuertasLuz : piezasMuertasOscuridad;

        if (cementerio.empty()) {
            std::cout << "No hay piezas para revivir." << std::endl;
            modoHechizoActivo = false;
            return;
        }

        if (!objetivo->estaOcupada()) {
            // 2. Sacamos la última pieza que murió
            Pieza* pRevivida = cementerio.back();
            cementerio.pop_back();

            // 3. Restauramos su vida y la ponemos en el tablero
            pRevivida->resetVida();
            objetivo->setPieza(pRevivida);

            registro[6] = true;
            std::cout << "¡Pieza revivida con exito!" << std::endl;
        }
        else {
            std::cout << "La casilla destino debe estar vacia." << std::endl;
            return;
        }
    }
    break;
    case 7: 
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() != turnoActual) {
            objetivo->getPieza()->setEncarcelada(3);
            registro[7] = true;
        }
        break;
    }

    if (registro[hechizoSeleccionado]) {
        finalizarTurno();
    }
}
void Tablero::finalizarTurno() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (matriz[i][j]->estaOcupada()) {
                matriz[i][j]->getPieza()->pasarTurnoBloqueo();
            }
        }
    }

    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
    turnosContados++;

    primerClicRealizado = false;
    origenSeleccionado = nullptr;
    modoHechizoActivo = false;
    piezaAuxiliar = nullptr;

    std::cout << "Turno finalizado. Tiempo actual: " << turnosContados << std::endl;
}

void Tablero::inicializarBotones() {
    std::string nombres[] = { "Teleport", "Heal", "Shift Time", "Exchange", "Summon", "Revive", "Imprison" };

    float posX = (9 * tamCasilla) + 20; 
    for (int i = 0; i < 7; i++) {
        float posY = 50 + (i * 70); 
        botonesHechizos.push_back(new Boton(posX, posY, 180, 50, nombres[i], fuente));
    }
}

void Tablero::registrarMuerte(Pieza* p) {
    if (p == nullptr) return; // Si no hay pieza, no hacemos nada. Evita el crash.

    if (p->getBando() == Bando::LUZ) {
        piezasMuertasLuz.push_back(p);
    }
    else {
        piezasMuertasOscuridad.push_back(p);
    }
}

void Tablero::eliminarPiezaDelMapa(Pieza* p) {
    if (p == nullptr) return;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (matriz[i][j]->getPieza() == p) {
                matriz[i][j]->setPieza(nullptr); // Quitamos la pieza del tablero
                return;
            }
        }
    }
}

void Tablero::moverPiezaACasilla(Pieza* p, sf::Vector2i destino) {
    // 1. Primero buscamos dónde estaba el ganador antes y limpiamos esa casilla
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (matriz[i][j]->getPieza() == p) {
                matriz[i][j]->setPieza(nullptr);
            }
        }
    }

    // 2. Lo ponemos en la casilla de destino (donde estaba el que murió)
    matriz[destino.x][destino.y]->setPieza(p);
}

int Tablero::getColorCasilla(int x, int y) {
    // 1. Validación de seguridad
    if (x < 0 || x >= 9 || y < 0 || y >= 9) return 0;

    Casilla* c = matriz[x][y];

    // 2. Si la casilla es OSCILANTE, sigue el ciclo de turnos
    if (c->getEsOscilante()) {
        // El ciclo de Archon suele avanzar cada pocos turnos
        // fase 0: Blanco, fase 1: Gris, fase 2: Negro, fase 3: Gris
        int fase = (turnosContados / 4) % 4;

        if (fase == 0) return 1;  // LUZ (Blanco)
        if (fase == 2) return -1; // OSCURIDAD (Negro)
        return 0;                 // NEUTRAL (Gris)
    }

    // 3. Si NO es oscilante, determinamos su color por posición o tipo
    if (c->getEsPuntoDePoder()) {
        // Los puntos de poder suelen ser neutrales o favorables al que los ocupa,
        // pero en el Archon clásico suelen ser fijos o seguir un ciclo aparte.
        // Aquí los dejaremos como neutrales (0) a menos que quieras darles color.
        return 0;
    }

    // Lógica por columnas para el resto (Fijas)
    if (x <= 1) return 1;  // Blanco fijo
    if (x >= 7) return -1; // Negro fijo

    return 0; // Gris por defecto
}