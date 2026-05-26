#include "Tablero.h"
#include <iostream>
#include <optional>
#include <cmath>
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
    matriz[0][0]->setPieza(new Valquiria(Bando::LUZ, skinActual));
    matriz[0][1]->setPieza(new Golem(Bando::LUZ, skinActual));
    matriz[0][2]->setPieza(new Unicornio(Bando::LUZ, skinActual));
    matriz[0][3]->setPieza(new Genio(Bando::LUZ, skinActual));
    matriz[0][4]->setPieza(new Hechicero(Bando::LUZ, skinActual));
    matriz[0][5]->setPieza(new Fenix(Bando::LUZ, skinActual));
    matriz[0][6]->setPieza(new Unicornio(Bando::LUZ, skinActual));
    matriz[0][7]->setPieza(new Golem(Bando::LUZ, skinActual));
    matriz[0][8]->setPieza(new Valquiria(Bando::LUZ, skinActual));

    matriz[1][0]->setPieza(new Arquero(Bando::LUZ, skinActual));
    for (int j = 1; j <= 7; j++) {
        matriz[1][j]->setPieza(new Caballero(Bando::LUZ, skinActual));
    }
    matriz[1][8]->setPieza(new Arquero(Bando::LUZ, skinActual));

    matriz[8][0]->setPieza(new Valquiria(Bando::OSCURIDAD, skinActual));
    matriz[8][1]->setPieza(new Golem(Bando::OSCURIDAD, skinActual));
    matriz[8][2]->setPieza(new Unicornio(Bando::OSCURIDAD, skinActual));
    matriz[8][3]->setPieza(new Genio(Bando::OSCURIDAD, skinActual));
    matriz[8][4]->setPieza(new Hechicero(Bando::OSCURIDAD, skinActual));
    matriz[8][5]->setPieza(new Fenix(Bando::OSCURIDAD, skinActual));
    matriz[8][6]->setPieza(new Unicornio(Bando::OSCURIDAD, skinActual));
    matriz[8][7]->setPieza(new Golem(Bando::OSCURIDAD, skinActual));
    matriz[8][8]->setPieza(new Valquiria(Bando::OSCURIDAD, skinActual));

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

        if (const auto* teclaPresionada = evento->getIf<sf::Event::KeyPressed>()) {
            if (teclaPresionada->code == sf::Keyboard::Key::Escape) {
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                modoHechizoActivo = false;
                piezaAuxiliar = nullptr;
                std::cout << "Accion cancelada." << std::endl;
            }
        }

        if (const auto* mouseClick = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseClick->button == sf::Mouse::Button::Left) {
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);
                sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaEstatica);

                if (primerClicRealizado && origenSeleccionado != nullptr) {
                    Pieza* p = origenSeleccionado->getPieza();
                    if (p != nullptr && dynamic_cast<Hechicero*>(p) != nullptr) {
                        bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;

                        bool yaUsoMagia = false;
                        for (int j = 0; j < 8; j++) { if (registro[j]) yaUsoMagia = true; }
                        if (!yaUsoMagia) {
                            for (int i = 0; i < botonesHechizos.size(); i++) {
                                if (botonesHechizos[i]->botonContieneRaton(posMapeada)) {
                                    hechizoSeleccionado = i + 1;
                                    modoHechizoActivo = true;
                                    std::cout << "Hechizo " << hechizoSeleccionado << " seleccionado." << std::endl;
                                    if (hechizoSeleccionado == 3) {
                                        procesarMagia(nullptr);
                                    }
                                    return;
                                }
                            }
                        }
                    }
                }

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
        if (esMovimientoValido(origenSeleccionado, casillaClicada)) {
            if (casillaClicada->estaOcupada()) {
                this->coordenadasCombate = sf::Vector2i(casillaClicada->getX(), casillaClicada->getY());
                this->atacante = origenSeleccionado->getPieza();
                this->defensor = casillaClicada->getPieza();
                this->hayCombatePendiente = true;

                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                std::cout << "¡Combate iniciado! Destino guardado." << std::endl;
            }
            else {
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

// Nueva rutina de seguridad: Permite transicionar de estado sin alterar datos prematuramente
void Tablero::limpiarBanderaCombate() {
    hayCombatePendiente = false;
}

// Consolidación estructural de eventos post-arena
void Tablero::procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal) {
    // 1. Gestión del cementerio
    registrarMuerte(perdedor);
    eliminarPiezaDelMapa(perdedor);

    // 2. Ocupación de la coordenada disputada
    if (ganador == atacanteOriginal) {
        moverPiezaACasilla(ganador, coordenadasCombate);
    }

    // 3. Resolución inversa de la bonificación ambiental
    ColorCasilla colorCasilla = getColorCasilla(coordenadasCombate.x, coordenadasCombate.y);
    int porcentajeBono = 0;
    Bando bandoFavorecido = Bando::LUZ;

    if (colorCasilla == ColorCasilla::BLANCO) { porcentajeBono = 40; bandoFavorecido = Bando::LUZ; }
    else if (colorCasilla == ColorCasilla::GRIS_CLARO) { porcentajeBono = 20; bandoFavorecido = Bando::LUZ; }
    else if (colorCasilla == ColorCasilla::GRIS_OSCURO) { porcentajeBono = 20; bandoFavorecido = Bando::OSCURIDAD; }
    else if (colorCasilla == ColorCasilla::NEGRO) { porcentajeBono = 40; bandoFavorecido = Bando::OSCURIDAD; }

    if (ganador->getBando() == bandoFavorecido && porcentajeBono > 0) {
        int vidaConBono = ganador->getVidaBase();
        int vidaOriginal = static_cast<int>(vidaConBono / (1.0f + (porcentajeBono / 100.0f)));
        if (vidaOriginal <= 0 && vidaConBono > 0) vidaOriginal = 1;
        ganador->restaurarValoresOriginales(vidaOriginal);
    }
    else {
        ganador->restaurarValoresOriginales(ganador->getVidaBase());
    }

    // 4. Limpieza de punteros de asalto y ciclo de reloj
    atacante = nullptr;
    defensor = nullptr;
    turnosContados++;
    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
}

void Tablero::dibujarPantalla(sf::RenderWindow& ventanaJuego) {
    ventanaJuego.setView(vistaEstatica);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j]->dibujar(ventanaJuego, origenSeleccionado, turnosContados, tamCasilla);

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

    if (primerClicRealizado && dynamic_cast<Hechicero*>(origenSeleccionado->getPieza())) {
        bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;

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
    case 2:
        if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == turnoActual) {
            Pieza* p = objetivo->getPieza();
            if (p->getVidaBase() < p->getVidaMaxima()) {
                p->resetVida();
                registro[2] = true;
                piezaAuxiliar = nullptr;
                std::cout << "Hechizo Heal aplicado con exito." << std::endl;
            }
            else {
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
    case 5:
        if (piezaAuxiliar == nullptr) {
            if (objetivo->estaOcupada() && objetivo->getPieza()->getBando() == turnoActual) {
                piezaAuxiliar = objetivo;
                std::cout << "Pieza seleccionada para invocar. Elige casilla vacia junto al Hechicero." << std::endl;
                return;
            }
        }
        else {
            if (!objetivo->estaOcupada()) {
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
        break;
    case 6:
    {
        std::vector<Pieza*>& cementerio = (turnoActual == Bando::LUZ) ? piezasMuertasLuz : piezasMuertasOscuridad;
        if (cementerio.empty()) {
            std::cout << "No hay piezas para revivir." << std::endl;
            modoHechizoActivo = false;
            return;
        }
        if (!objetivo->estaOcupada()) {
            Pieza* pRevivida = cementerio.back();
            cementerio.pop_back();
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
    if (p == nullptr) return;
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
                matriz[i][j]->setPieza(nullptr);
                return;
            }
        }
    }
}

void Tablero::moverPiezaACasilla(Pieza* p, sf::Vector2i destino) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (matriz[i][j]->getPieza() == p) {
                matriz[i][j]->setPieza(nullptr);
            }
        }
    }
    matriz[destino.x][destino.y]->setPieza(p);
}

ColorCasilla Tablero::getColorCasilla(int x, int y) {
    if (x < 0 || x >= 9 || y < 0 || y >= 9) return ColorCasilla::GRIS_CLARO;
    Casilla* c = matriz[x][y];
    if (c->getEsOscilante()) {
        int fase = turnosContados % 6;
        switch (fase) {
        case 0: return ColorCasilla::NEGRO;
        case 1: return ColorCasilla::GRIS_OSCURO;
        case 2: return ColorCasilla::GRIS_CLARO;
        case 3: return ColorCasilla::BLANCO;
        case 4: return ColorCasilla::GRIS_CLARO;
        case 5: return ColorCasilla::GRIS_OSCURO;
        }
    }
    return c->getColorActual();
}