#include <iostream>
#include <optional>
#include <cmath>
#include "Tablero.h"
#include "GestorHechizos.h"
#include "GestorVictoria.h"
#include "GestorCombate.h"
#include "GestorTurno.h"
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

Tablero::Tablero(float _tamano, std::string skin) : textoVictoria(fuente) {
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
    estadoVictoria = 0;
    motivoVictoria = 0;
    faseVictoria = 0;
    temporizadorFase = 0.f;
    volverAlMenu = false;
    bandoLuzUsoMagia = false;
    bandoOscuroUsoMagia = false;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            matriz[i][j] = new Casilla(i, j);

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

    if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf"))
        std::cout << "Error cargando fuente." << std::endl;

    textoVictoria.setCharacterSize(60);
    textoVictoria.setOutlineThickness(5.f);
    textoVictoria.setOutlineColor(sf::Color::Black);

    inicializarBotones();
}

Tablero::~Tablero() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            delete matriz[i][j];
    for (auto b : botonesHechizos)
        delete b;
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
    for (int j = 1; j <= 7; j++) matriz[1][j]->setPieza(new Caballero(Bando::LUZ, skinActual));
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
    for (int j = 1; j <= 7; j++) matriz[7][j]->setPieza(new Caballero(Bando::OSCURIDAD, skinActual));
    matriz[7][8]->setPieza(new Arquero(Bando::OSCURIDAD, skinActual));
}

void Tablero::procesarEntrada(sf::RenderWindow& ventanaJuego) {
    float dt = relojTablero.restart().asSeconds();

    if (estadoVictoria == 0) {
        estadoVictoria = gestorVictoria.verificarVictoria(this);
        if (estadoVictoria != 0) {
            if (motivoVictoria == 3) { faseVictoria = 1; temporizadorFase = 2.0f; }
            else { faseVictoria = 2; temporizadorFase = 1.5f; }
        }
    }
    else {
        gestorVictoria.actualizarFaseVictoria(this, dt);
        while (ventanaJuego.pollEvent()) {}
        return;
    }

    while (const std::optional<sf::Event> evento = ventanaJuego.pollEvent()) {
        if (evento->is<sf::Event::Closed>())
            ventanaJuego.close();

        if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {
            if (tecla->code == sf::Keyboard::Key::Escape) {
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                modoHechizoActivo = false;
                piezaAuxiliar = nullptr;
            }
        }

        if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (click->button == sf::Mouse::Button::Left) {
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);
                sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaEstatica);

                if (primerClicRealizado && origenSeleccionado != nullptr) {
                    Pieza* p = origenSeleccionado->getPieza();
                    if (p != nullptr && dynamic_cast<Hechicero*>(p) != nullptr) {
                        bool yaUsoMagia = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;
                        if (!yaUsoMagia) {
                            for (int i = 0; i < botonesHechizos.size(); i++) {
                                if (botonesHechizos[i]->botonContieneRaton(posMapeada)) {
                                    hechizoSeleccionado = i + 1;
                                    modoHechizoActivo = true;
                                    if (hechizoSeleccionado == 3) procesarMagia(nullptr);
                                    return;
                                }
                            }
                        }
                    }
                }

                int celdaX = static_cast<int>(std::floor(posMapeada.x / tamCasilla));
                int celdaY = static_cast<int>(std::floor(posMapeada.y / tamCasilla));
                if (celdaX >= 0 && celdaX < 9 && celdaY >= 0 && celdaY < 9)
                    gestionarTurno(matriz[celdaX][celdaY]);
            }
        }
    }
}

void Tablero::dibujarPantalla(sf::RenderWindow& ventanaJuego) {
    ventanaJuego.setView(vistaEstatica);

    if (estadoVictoria != 0 && faseVictoria == 3) {
        gestorVictoria.dibujarFaseVictoria(this, ventanaJuego);
        return;
    }

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
        bool yaUsoMagia = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;
        sf::Vector2f mousePos = ventanaJuego.mapPixelToCoords(sf::Mouse::getPosition(ventanaJuego), vistaEstatica);
        std::string nombres[] = { "Teleport", "Heal", "Shift Time", "Exchange", "Summon", "Revive", "Imprison" };
        for (int i = 0; i < botonesHechizos.size(); i++) {
            botonesHechizos[i]->setTexto(yaUsoMagia ? "AGOTADO" : nombres[i]);
            botonesHechizos[i]->actualizarColorBoton(mousePos);
            botonesHechizos[i]->dibujar(ventanaJuego);
        }
    }

    if (estadoVictoria != 0 && faseVictoria == 2) {
        if (static_cast<int>(temporizadorFase * 10) % 2 == 0) {
            sf::RectangleShape fondoVerde(vistaEstatica.getSize());
            fondoVerde.setOrigin({ vistaEstatica.getSize().x / 2.f, vistaEstatica.getSize().y / 2.f });
            fondoVerde.setPosition(vistaEstatica.getCenter());
            fondoVerde.setFillColor(sf::Color(0, 255, 0, 120));
            ventanaJuego.draw(fondoVerde);
        }
    }
}

void Tablero::gestionarTurno(Casilla* casillaClicada) {
    gestorTurno.gestionarTurno(casillaClicada, this);
}

void Tablero::finalizarTurno() {
    gestorTurno.finalizarTurno(this);
}

bool Tablero::esMovimientoValido(Casilla* origen, Casilla* destino) {
    if (!origen || !destino) return false;
    Pieza* p = origen->getPieza();
    if (p == nullptr) return false;
    return p->mover(origen, destino, matriz);
}

void Tablero::limpiarBanderaCombate() {
    hayCombatePendiente = false;
}

void Tablero::procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal) {
    gestorCombate.procesarResultadoCombate(ganador, perdedor, atacanteOriginal, this);
    atacante = nullptr;
    defensor = nullptr;
    turnosContados++;
    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
}

void Tablero::procesarMagia(Casilla* objetivo) {
    if (!modoHechizoActivo || !origenSeleccionado) return;
    gestorHechizos.ejecutarHechizo(hechizoSeleccionado, objetivo, this);
    bool magiaCompletada = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;
    if (magiaCompletada) finalizarTurno();
}

void Tablero::inicializarBotones() {
    std::string nombres[] = { "Teleport", "Heal", "Shift Time", "Exchange", "Summon", "Revive", "Imprison" };
    float posX = (9 * tamCasilla) + 20;
    for (int i = 0; i < 7; i++)
        botonesHechizos.push_back(new Boton(posX, 50.0f + ((float)i * 70.0f), 180, 50, nombres[i], fuente));
}

ColorCasilla Tablero::getColorCasilla(int x, int y) {
    if (x < 0 || x >= 9 || y < 0 || y >= 9) return ColorCasilla::GRIS_CLARO;
    return matriz[x][y]->getColorActual();
}