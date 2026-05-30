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

//Constructor por defecto: delega en el constructor principal con valores base
Tablero::Tablero() : Tablero(95.0f, " ") {}

Tablero::Tablero(float _tamano, std::string skin) : textoVictoria(fuente) {
    tamCasilla = _tamano;//tamaño en píxeles en cada casilla
    skinActual = skin;//pack de imágenes seleccionado en el menú
    turnoActual = Bando::LUZ;// siempre empieza el bando LUZ
    primerClicRealizado = false;// ninguna pieza seleccionada al inicio
    origenSeleccionado = nullptr;
    piezaAuxiliar = nullptr;// sin casilla auxiliar de hechizo activa
    hayCombatePendiente = false;
    atacante = nullptr;
    defensor = nullptr;
    turnosContados = 0; // el ciclo de colores empieza en fase 0 (negro)
    estadoVictoria = 0; // 0=jugando, 1=gana Luz, 2=gana Oscuridad, 3=empate
    motivoVictoria = 0;// se asigna al detectar victoria
    faseVictoria = 0;
    temporizadorFase = 0.f;
    volverAlMenu = false;
    bandoLuzUsoMagia = false;// ningún bando ha usado magia al inicio del turno
    bandoOscuroUsoMagia = false;
    // Crea las 81 casillas del tablero (9 columnas x 9 filas)
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            matriz[i][j] = new Casilla(i, j);

    inicializarTablero();// coloca las piezas en posición inicial
    // Define el área visible: 12 casillas de ancho (9 tablero + espacio para botones de hechizos)
    float anchoVisible = 12.0f * tamCasilla;
    float altoVisible = 9.0f * tamCasilla;
    vistaEstatica.setSize({ anchoVisible, altoVisible });
    vistaEstatica.setCenter({ anchoVisible / 2.0f - 50.0f, altoVisible / 2.0f });
    vistaEstatica.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
    // Ningún hechizo ha sido usado al inicio de la partida
    for (int i = 0; i < 8; i++) {
        hechizosLuzUsados[i] = false;
        hechizosOscurosUsados[i] = false;
    }

    if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf"))
        std::cout << "Error cargando fuente." << std::endl;

    textoVictoria.setCharacterSize(60);
    textoVictoria.setOutlineThickness(5.f);
    textoVictoria.setOutlineColor(sf::Color::Black);

    inicializarBotones();// crea los 7 botones de hechizos a la derecha del tablero
}

Tablero::~Tablero() {
    // Libera las 81 casillas (cada casilla libera su pieza si tiene)
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            delete matriz[i][j];
    // Libera los botones de hechizos
    for (auto b : botonesHechizos)
        delete b;
}

void Tablero::inicializarTablero() {
    // Fila 0 (Luz): Valquiria-Golem-Unicornio-Genio-Hechicero-Fenix-Unicornio-Golem-Valquiria
    matriz[0][0]->setPieza(new Valquiria(Bando::LUZ, skinActual));
    matriz[0][1]->setPieza(new Golem(Bando::LUZ, skinActual));
    matriz[0][2]->setPieza(new Unicornio(Bando::LUZ, skinActual));
    matriz[0][3]->setPieza(new Genio(Bando::LUZ, skinActual));
    matriz[0][4]->setPieza(new Hechicero(Bando::LUZ, skinActual));
    matriz[0][5]->setPieza(new Fenix(Bando::LUZ, skinActual));
    matriz[0][6]->setPieza(new Unicornio(Bando::LUZ, skinActual));
    matriz[0][7]->setPieza(new Golem(Bando::LUZ, skinActual));
    matriz[0][8]->setPieza(new Valquiria(Bando::LUZ, skinActual));
    // Fila 1 (Luz): Arquero en los extremos, Caballeros en las 7 posiciones del centro
    matriz[1][0]->setPieza(new Arquero(Bando::LUZ, skinActual));
    for (int j = 1; j <= 7; j++) matriz[1][j]->setPieza(new Caballero(Bando::LUZ, skinActual));
    matriz[1][8]->setPieza(new Arquero(Bando::LUZ, skinActual));
    // Filas 8 y 7 (Oscuridad): espejo exacto de las filas 0 y 1
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
    float dt = relojTablero.restart().asSeconds();// tiempo del frame para los temporizadores
    // Bloque 1: comprueba victoria cada frame mientras el juego sigue activo
    if (estadoVictoria == 0) {
        estadoVictoria = gestorVictoria.verificarVictoria(this);
        if (estadoVictoria != 0) {
            // Arranca la cinemática de fin de partida según el motivo
            if (motivoVictoria == 3) { faseVictoria = 1; temporizadorFase = 2.0f; }// prisión: delay antes del parpadeo
            else { faseVictoria = 2; temporizadorFase = 1.5f; } // nodos/aniquilación: directo al parpadeo
        }
 
    }
    else {
        // Bloque 2: alguien ganó, avanza la animación y bloquea la entrada del jugador
        gestorVictoria.actualizarFaseVictoria(this, dt);
        while (ventanaJuego.pollEvent()) {}// vacía el buffer de eventos para bloquear interacción
        return;
    }
    // Bloque 3: entrada normal del jugador
    while (const std::optional<sf::Event> evento = ventanaJuego.pollEvent()) {
        if (evento->is<sf::Event::Closed>())
            ventanaJuego.close();

        if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {
            if (tecla->code == sf::Keyboard::Key::Escape) {
                // Escape cancela cualquier acción en curso y limpia el resaltado
                primerClicRealizado = false;
                origenSeleccionado = nullptr;
                modoHechizoActivo = false;
                piezaAuxiliar = nullptr;
                casillasValidas.clear();
            }
        }

        if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (click->button == sf::Mouse::Button::Left) {
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);
                // Convierte píxeles de pantalla a coordenadas del mundo del tablero
                sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaEstatica);
                // Si hay un Hechicero seleccionado, comprueba si el clic fue en un botón de hechizo
                if (primerClicRealizado && origenSeleccionado != nullptr) {
                    Pieza* p = origenSeleccionado->getPieza();
                    if (p != nullptr && dynamic_cast<Hechicero*>(p) != nullptr) {
                        bool yaUsoMagia = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;
                        if (!yaUsoMagia) {
                            for (int i = 0; i < botonesHechizos.size(); i++) {
                                if (botonesHechizos[i]->botonContieneRaton(posMapeada)) {
                                    hechizoSeleccionado = i + 1; // hechizos numerados del 1 al 7
                                    modoHechizoActivo = true;
                                    if (hechizoSeleccionado == 3) procesarMagia(nullptr);// Shift Time no necesita objetivo
                                    return;
                                }
                            }
                        }
                    }
                }
                // Convierte la posición del ratón a índices de celda (0-8)
                int celdaX = static_cast<int>(std::floor(posMapeada.x / tamCasilla));
                int celdaY = static_cast<int>(std::floor(posMapeada.y / tamCasilla));
                if (celdaX >= 0 && celdaX < 9 && celdaY >= 0 && celdaY < 9)
                    gestionarTurno(matriz[celdaX][celdaY]);// clic válido dentro del tablero
            }
        }
    }
}

void Tablero::dibujarPantalla(sf::RenderWindow& ventanaJuego) {
    ventanaJuego.setView(vistaEstatica);
    // Fase 3 de victoria: pantalla negra con el texto del ganador.
    if (estadoVictoria != 0 && faseVictoria == 3) {
        gestorVictoria.dibujarFaseVictoria(this, ventanaJuego);
        return;
    }
    // Dibuja las 81 casillas (cada casilla dibuja también su pieza si tiene)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j]->dibujar(ventanaJuego, origenSeleccionado, turnosContados, tamCasilla);
            // Si esta casilla es la auxiliar de Teleport o Exchange, muestra borde amarillo
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
    // Dibuja el resaltado de movimientos válidos tras seleccionar una pieza
    for (Casilla* c : casillasValidas) {
        if (c == origenSeleccionado) continue;// no resalta la casilla propia
        int i = c->getX();
        int j = c->getY();
        sf::RectangleShape resalte(sf::Vector2f(tamCasilla - 4, tamCasilla - 4));
        resalte.setPosition({ i * tamCasilla + 2, j * tamCasilla + 2 });
        resalte.setFillColor(sf::Color::Transparent);
        resalte.setOutlineThickness(3.0f);
        if (c->estaOcupada())
            resalte.setOutlineColor(sf::Color(255, 80, 80, 200)); // rojo: casilla con enemigo (ataque)
        else
            resalte.setOutlineColor(sf::Color(80, 255, 80, 200));// verde: casilla vacía (movimiento)
        ventanaJuego.draw(resalte);
    }
    // Si hay un Hechicero seleccionado y puede usar magia, muestra los 7 botones de hechizos
    if (primerClicRealizado && dynamic_cast<Hechicero*>(origenSeleccionado->getPieza())) {
        bool yaUsoMagia = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;
        sf::Vector2f mousePos = ventanaJuego.mapPixelToCoords(sf::Mouse::getPosition(ventanaJuego), vistaEstatica);
        std::string nombres[] = { "Teleport", "Heal", "Shift Time", "Exchange", "Summon", "Revive", "Imprison" };
        for (int i = 0; i < botonesHechizos.size(); i++) {
            botonesHechizos[i]->setTexto(yaUsoMagia ? "AGOTADO" : nombres[i]);// muestra AGOTADO si ya usó magia
            botonesHechizos[i]->actualizarColorBoton(mousePos);
            botonesHechizos[i]->dibujar(ventanaJuego);
        }
    }
    // Fase 2 de victoria: parpadeo verde semitransparente sobre todo el tablero (alterna cada 100ms)
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
// Delega en GestorTurno: selección de pieza, movimiento e inicio de combate
void Tablero::gestionarTurno(Casilla* casillaClicada) {
    gestorTurno.gestionarTurno(casillaClicada, this);
}
// Delega en GestorTurno: descuenta bloqueos, cambia bando, resetea selección
void Tablero::finalizarTurno() {
    gestorTurno.finalizarTurno(this);
}

bool Tablero::esMovimientoValido(Casilla* origen, Casilla* destino) {
    if (!origen || !destino) return false;
    Pieza* p = origen->getPieza();
    if (p == nullptr) return false;
    return p->mover(origen, destino, matriz);// delega la lógica de movimiento en cada tipo de pieza
}
// Resetea la bandera de combate tras lanzar la Arena desde MotorArchon
void Tablero::limpiarBanderaCombate() {
    hayCombatePendiente = false;
}
// Delega en GestorCombate y luego cambia el turno al bando contrario
void Tablero::procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal) {
    gestorCombate.procesarResultadoCombate(ganador, perdedor, atacanteOriginal, this);
    atacante = nullptr;
    defensor = nullptr;
    turnosContados++;
    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;
}
// Delega en GestorHechizos y finaliza el turno si el hechizo se completó
void Tablero::procesarMagia(Casilla* objetivo) {
    if (!modoHechizoActivo || !origenSeleccionado) return;
    gestorHechizos.ejecutarHechizo(hechizoSeleccionado, objetivo, this);
    bool magiaCompletada = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;
    if (magiaCompletada) finalizarTurno();
}

void Tablero::inicializarBotones() {
    // Los nombres coinciden con los 7 hechizos del Hechicero (case 1-7 en GestorHechizos)
    std::string nombres[] = { "Teleport", "Heal", "Shift Time", "Exchange", "Summon", "Revive", "Imprison" };
    float posX = (9 * tamCasilla) + 20;// a la derecha del tablero con 20px de margen
    for (int i = 0; i < 7; i++)
        botonesHechizos.push_back(new Boton(posX, 50.0f + ((float)i * 70.0f), 180, 50, nombres[i], fuente));
    // cada botón separado 70px del anterior, empezando a 50px del borde superior
}

ColorCasilla Tablero::getColorCasilla(int x, int y) {
    if (x < 0 || x >= 9 || y < 0 || y >= 9) return ColorCasilla::GRIS_CLARO;
    return matriz[x][y]->getColorActual();// devuelve el color ya calculado por Casilla::dibujar()
}