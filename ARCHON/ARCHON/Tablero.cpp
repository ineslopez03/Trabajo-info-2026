#include <iostream>
#include <optional>
#include <cmath>
#include "Tablero.h"
#include"GestorHechizos.h"
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
//Constructor por defecto:usa tamaño casilla 95px y sin skin especial
Tablero::Tablero(float _tamano, std::string skin) : textoVictoria(fuente) {
    tamCasilla = _tamano;//tamaño en pixeles de cada casilla
    skinActual = skin;//nombre del pack de imágenes
    turnoActual = Bando::LUZ;//siempre empieza LUZ
    primerClicRealizado = false;//aún no se ha seleccionado ninguna pieza
    origenSeleccionado = nullptr;
    piezaAuxiliar = nullptr;//casilla auxiliar usada para algunos hechizos(Teleport y Exchange)
    hayCombatePendiente = false;
    atacante = nullptr;
    defensor = nullptr;
    turnosContados = 0;//contador global de turnos,controla el ciclo de colores

    estadoVictoria = 0;// 0=jugando,1=gana LUZ,2=gana OSCURIDAD, 3=empate;
    motivoVictoria = 0;//1=nodos,2=aniquilación
    faseVictoria = 0;
    temporizadorFase = 0.f;
    volverAlMenu = false;

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
        //Rastrean qué hechizos se han gastado
    }
    if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cout << "Ni siquiera encontre la fuente del sistema." << std::endl;
    }

    textoVictoria.setCharacterSize(60);
    textoVictoria.setOutlineThickness(5.f);
    textoVictoria.setOutlineColor(sf::Color::Black);

    inicializarBotones();
}// Inicializa todos los atributos de estado del tablero
//La vista estática define el área visible(9 casillas * tamCasilla)

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
}// Coloca las piezas en su posición inicial de Archon:
//Fila 0(LUZ): Valquiria-Golem-Unicornio-Genio-Hechicero-Fenix-Unicornio-Golem-Valquiria
//Fila 1(LUZ): Arquero en extremos,Caballeros en el centro
//Fila 8 y 7(Oscuridad): espejo exacto

int Tablero::verificarVictoria() {
    int contadorLuz = 0, libresLuz = 0, nodosLuz = 0;
    int contadorOsc = 0, libresOsc = 0, nodosOsc = 0;
    //Recorre el tablero contando piezas de cada bando
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            Casilla* c = matriz[i][j];
            if (!c->estaOcupada()) continue; // casilla vacía, se salta
            Pieza* p = c->getPieza();

            if (p->getBando() == Bando::LUZ) {
                contadorLuz++;
                if (!p->estaEncarcelada()) libresLuz++; // solo cuenta si puede moverse
                if (c->getEsPuntoDePoder())  nodosLuz++; // controla un nodo
            }
            else {
                contadorOsc++;
                if (!p->estaEncarcelada()) libresOsc++;
                if (c->getEsPuntoDePoder())  nodosOsc++;
            }
        }
    }
    // Condición 1: un bando ocupa los 5 nodos → gana
    if (nodosLuz == 5) { motivoVictoria = 1; return 1; }
    if (nodosOsc == 5) { motivoVictoria = 1; return 2; }
    // Pierde un bando si no tiene piezas, o si su única pieza está encarcelada
    bool pierdeLuz = (contadorLuz == 0) || (contadorLuz == 1 && libresLuz == 0);
    bool pierdeOsc = (contadorOsc == 0) || (contadorOsc == 1 && libresOsc == 0);
    //Condición 2/3 empate si los dos pierden a la vez
    if (pierdeLuz && pierdeOsc) { motivoVictoria = 2; return 3; }
    if (pierdeLuz) { motivoVictoria = (contadorLuz == 0) ? 2 : 3; return 2; }
    if (pierdeOsc) { motivoVictoria = (contadorOsc == 0) ? 2 : 3; return 1; }
    return 0;//nadie ha ganado todavía
}

void Tablero::procesarEntrada(sf::RenderWindow& ventanaJuego) {
    float dt = relojTablero.restart().asSeconds();
    //tiempo desde el último frame para temporizadores
    
    if (estadoVictoria == 0) {
        //solo comprueba victoria mientras 
        estadoVictoria = verificarVictoria();
        if (estadoVictoria != 0) {
            if (motivoVictoria == 3) {
                faseVictoria = 1; 
                temporizadorFase = 2.0f;
            }
            else {
                faseVictoria = 2; 
                temporizadorFase = 1.5f;
            }
        }
    }
    else {
        temporizadorFase -= dt;
        if (temporizadorFase <= 0.f) {
            if (faseVictoria == 1) {
                faseVictoria = 2;
                temporizadorFase = 1.5f; 
            }
            else if (faseVictoria == 2) {
                faseVictoria = 3;
                temporizadorFase = 4.0f; 
            }
            else if (faseVictoria == 3) {
                volverAlMenu = true;
            }
        }
        while (ventanaJuego.pollEvent()) {} 
        return;
    }

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
                        bool yaUsoMagia = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;

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
//Primer clic: selecciona una pieza propia no encarcelada
//Segundo clic:
//Si es un botón de hechizo-> activa modoHechizoActivo
//Si el movimiento es válido y la casilla está vacía->mueve
//Si el movimiento es válido y hay enemigo->inicia combate(hayCombatePendiente=true)

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
    registrarMuerte(perdedor);
    eliminarPiezaDelMapa(perdedor);
    bool* registro = (turnoActual == Bando::LUZ) ? hechizosLuzUsados : hechizosOscurosUsados;
    if (atacanteOriginal == atacante && hechizoSeleccionado == 5) {
        if (ganador == atacanteOriginal) {
            delete ganador;
            std::cout << "El Elemental vencio y se desvanecio en el eter." << std::endl;
        }
        else {
            ganador->restaurarValoresOriginales(ganador->getVidaBase());
        }
        hechizoSeleccionado = 0;
    }
    else {

        if (ganador == atacanteOriginal) {
            moverPiezaACasilla(ganador, coordenadasCombate);
        }

        ColorCasilla colorCasilla = getColorCasilla(coordenadasCombate.x, coordenadasCombate.y);
        int porcentajeBono = 0;
        Bando bandoFavorecido = Bando::LUZ;

        if (colorCasilla == ColorCasilla::BLANCO) { porcentajeBono = 40; bandoFavorecido = Bando::LUZ; }
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
    }
    atacante = nullptr;
    defensor = nullptr;
    turnosContados++;
    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;

}

void Tablero::dibujarPantalla(sf::RenderWindow& ventanaJuego) {
    ventanaJuego.setView(vistaEstatica);

    // Renderizado del subestado final: Vacía los buffers visuales y muestra el veredicto en fondo negro
    if (estadoVictoria != 0 && faseVictoria == 3) {
        sf::RectangleShape fondoNegro(vistaEstatica.getSize());
        fondoNegro.setOrigin({ vistaEstatica.getSize().x / 2.f, vistaEstatica.getSize().y / 2.f });
        fondoNegro.setPosition(vistaEstatica.getCenter());
        fondoNegro.setFillColor(sf::Color::Black);
        ventanaJuego.draw(fondoNegro);

        if (estadoVictoria == 1) {
            textoVictoria.setString("GANADOR BANDO DE LUZ");
            textoVictoria.setFillColor(sf::Color::Cyan);
        }
        else if (estadoVictoria == 2) {
            textoVictoria.setString("GANADOR BANDO DE OSCURIDAD");
            textoVictoria.setFillColor(sf::Color::Red);
        }
        else {
            textoVictoria.setString("EMPATE ESTRUCTURAL");
            textoVictoria.setFillColor(sf::Color::Yellow);
        }

        sf::FloatRect limites = textoVictoria.getLocalBounds();
        textoVictoria.setOrigin({ limites.size.x / 2.f, limites.size.y / 2.f });
        textoVictoria.setPosition(vistaEstatica.getCenter());
        ventanaJuego.draw(textoVictoria);

        return; // Interrupción crítica del pipeline gráfico
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
            if (yaUsoMagia) {
                botonesHechizos[i]->setTexto("AGOTADO");
            }
            else {
                botonesHechizos[i]->setTexto(nombres[i]);
            }
            botonesHechizos[i]->actualizarColorBoton(mousePos);
            botonesHechizos[i]->dibujar(ventanaJuego);
        }
    }

    // Proyección de la saturación esmeralda superpuesta al tablero base
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

void Tablero::procesarMagia(Casilla* objetivo) {
    if (!modoHechizoActivo || !origenSeleccionado) return;

   
    static GestorHechizos gestor;
    gestor.ejecutarHechizo(hechizoSeleccionado, objetivo, this);

    bool magiaCompletada = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;
    if (magiaCompletada) {
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
    if (x < 0 || x >= 9 || y < 0 || y >= 9) return ColorCasilla::GRIS_CLARO;
    
    return matriz[x][y]->getColorActual();
    if (c->getEsPuntoDePoder()) {
        return ColorCasilla::GRIS_CLARO;
    }
    if (x <= 1) return ColorCasilla::BLANCO;
    if (x >= 7) return ColorCasilla::NEGRO;
    return ColorCasilla::GRIS_CLARO;
}