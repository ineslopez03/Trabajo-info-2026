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
// Constructor parametrizado: inicializa todos los atributos del tablero
Tablero::Tablero(float _tamano, std::string skin) : textoVictoria(fuente) {
    // textoVictoria se construye con la fuente como parámetro (inicialización en lista)
    tamCasilla = _tamano;//tamaño en píxeles en cada casilla
    skinActual = skin;//pack de imágenes seleccionado en el menú
    turnoActual = Bando::LUZ;// siempre empieza el bando LUZ
    primerClicRealizado = false;// ninguna pieza seleccionada al inicio
    origenSeleccionado = nullptr;// No hay casilla de origen seleccionada
    piezaAuxiliar = nullptr;// sin casilla auxiliar de hechizo activa
    hayCombatePendiente = false;// No hay combate pendiente al inicio
    atacante = nullptr;  // No hay atacante asignado
    defensor = nullptr; // No hay defensor asignado
    turnosContados = 0; // el ciclo de colores empieza en fase 0 (negro)
    estadoVictoria = 0; // 0=jugando, 1=gana Luz, 2=gana Oscuridad, 3=empate
    motivoVictoria = 0;// se asigna al detectar victoria
    faseVictoria = 0; // Sin fase de victoria activa
    temporizadorFase = 0.f; // Sin temporizador activo
    volverAlMenu = false;// No hay que volver al menú todavía
    bandoLuzUsoMagia = false;// ningún bando ha usado magia al inicio del turno
    bandoOscuroUsoMagia = false;
    // Crea las 81 casillas del tablero (9 columnas x 9 filas)
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            matriz[i][j] = new Casilla(i, j);

    inicializarTablero();// coloca las piezas en posición inicial
    // Define el área visible: 12 casillas de ancho (9 tablero + espacio para botones de hechizos)
    float anchoVisible = 12.0f * tamCasilla;// El ancho visible incluye zona de botones (12 columnas)
    float altoVisible = 9.0f * tamCasilla;// El alto visible es exactamente el tablero (9 filas)
    vistaEstatica.setSize({ anchoVisible, altoVisible }); // Establece el tamaño de la vista
    vistaEstatica.setCenter({ anchoVisible / 2.0f - 50.0f, altoVisible / 2.0f });// Centra la vista con un offset horizontal
    vistaEstatica.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));// La vista ocupa toda la venta
    
    for (int i = 0; i < 8; i++) { // Inicializa los 8 slots de hechizos
        hechizosLuzUsados[i] = false;// Ningún hechizo de LUZ usado
        hechizosOscurosUsados[i] = false;// Ningún hechizo de OSCURIDAD usado
    }

    if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf"))// Intenta cargar la fuente Arial del sistema
        std::cout << "Error cargando fuente." << std::endl; // Avisa si no puede cargarla

    textoVictoria.setCharacterSize(60);// Tamaño de letra grande para el texto de victoria
    textoVictoria.setOutlineThickness(5.f);// Grosor del contorno del texto
    textoVictoria.setOutlineColor(sf::Color::Black);// Color del contorno: negro

    inicializarBotones();// crea los 7 botones de hechizos a la derecha del tablero
}

Tablero::~Tablero() {
    // Libera las 81 casillas (cada casilla libera su pieza si tiene)
    for (int i = 0; i < 9; i++)// Itera las filas
        for (int j = 0; j < 9; j++) // Itera las columnas
            delete matriz[i][j]; // Libera cada casilla (y su pieza si la tiene)
    for (auto b : botonesHechizos) // Itera el vector de botones
        delete b;// Libera cada botón
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
// Procesa todos los eventos de entrada del jugador cada frame
void Tablero::procesarEntrada(sf::RenderWindow& ventanaJuego) {
    float dt = relojTablero.restart().asSeconds();// Calcula el tiempo transcurrido desde el último frame
    if (estadoVictoria == 0) { // Si el juego sigue activo
        estadoVictoria = gestorVictoria.verificarVictoria(this);// Comprueba si hay ganador
        if (estadoVictoria != 0) {// Si acaba de detectarse una victoria
            if (motivoVictoria == 3) { faseVictoria = 1; temporizadorFase = 2.0f; }// Victoria por encarcelamiento: empieza en fase 1
            else { faseVictoria = 2; temporizadorFase = 1.5f; }   // Otras victorias: empieza en fase 2
        }
 
    }
    else {
        // Si ya hay un ganador, gestiona la animación de victoria
        gestorVictoria.actualizarFaseVictoria(this, dt);// Avanza la fase de victoria según el tiempo
        while (ventanaJuego.pollEvent()) {}// Descarta todos los eventos de input durante la victoria
        return;// Sale sin procesar más lógica
    }
    // Procesa cada evento pendiente en la cola
    while (const std::optional<sf::Event> evento = ventanaJuego.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) // Si el usuario cierra la ventana
            ventanaJuego.close(); // Cierra la ventana del juego

        if (const auto* tecla = evento->getIf<sf::Event::KeyPressed>()) {// Si se presionó una tecla
            if (tecla->code == sf::Keyboard::Key::Escape) {// Si la tecla es Escape
                primerClicRealizado = false;// Cancela la selección de pieza
                origenSeleccionado = nullptr;// Limpia el origen
                modoHechizoActivo = false;// Desactiva el modo hechizo
                piezaAuxiliar = nullptr;// Limpia la pieza auxiliar
                casillasValidas.clear();// Borra los movimientos resaltados
            }
        }

        if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {// Si se hizo click
            if (click->button == sf::Mouse::Button::Left) {// Solo click izquierdo
                sf::Vector2i posRaton = sf::Mouse::getPosition(ventanaJuego);// Posición del ratón en píxeles
                // Convierte píxeles de pantalla a coordenadas del mundo del tablero
                sf::Vector2f posMapeada = ventanaJuego.mapPixelToCoords(posRaton, vistaEstatica);
                if (primerClicRealizado && origenSeleccionado != nullptr) {// Si ya hay una pieza seleccionada
                    Pieza* p = origenSeleccionado->getPieza();// Obtiene la pieza seleccionada
                    if (p != nullptr && dynamic_cast<Hechicero*>(p) != nullptr) {// Si la pieza es un Hechicero
                        bool yaUsoMagia = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;// Comprueba si ya usó magia
                        if (!yaUsoMagia) {// Si aún puede usar magia
                            for (int i = 0; i < botonesHechizos.size(); i++) {// Si aún puede usar magia
                                if (botonesHechizos[i]->botonContieneRaton(posMapeada)) {// Si el click fue sobre un botón
                                    hechizoSeleccionado = i + 1; // Selecciona el hechizo (1-indexed)
                                    modoHechizoActivo = true;// Activa el modo hechizo
                                    if (hechizoSeleccionado == 3) procesarMagia(nullptr);// Shift Time no necesita objetivo
                                    return;// Sale, esperando el click en el objetivo
                                }
                            }
                        }
                    }
                }
                // Calcula en qué celda del tablero cayó el click
                int celdaX = static_cast<int>(std::floor(posMapeada.x / tamCasilla));// Columna clicada
                int celdaY = static_cast<int>(std::floor(posMapeada.y / tamCasilla));// Fila clicada
                if (celdaX >= 0 && celdaX < 9 && celdaY >= 0 && celdaY < 9)// Si está dentro del tablero
                    gestionarTurno(matriz[celdaX][celdaY]);// Procesa el click en esa casilla
            }
        }
    }
}
// Renderiza todos los elementos visuales del tablero en pantalla
void Tablero::dibujarPantalla(sf::RenderWindow& ventanaJuego) {
    ventanaJuego.setView(vistaEstatica);// Aplica la vista estática antes de dibujar
   
    if (estadoVictoria != 0 && faseVictoria == 3) {// Si estamos en la fase final de victoria
        gestorVictoria.dibujarFaseVictoria(this, ventanaJuego);// Muestra la pantalla de victoria
        return;// No dibuja nada más
    }
    // Dibuja las 81 casillas (cada casilla dibuja también su pieza si tiene)
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            matriz[i][j]->dibujar(ventanaJuego, origenSeleccionado, turnosContados, tamCasilla);// Dibuja casilla y pieza
            // Si esta casilla es la auxiliar de Teleport o Exchange
            if (matriz[i][j] == piezaAuxiliar) {
                sf::RectangleShape marcador(sf::Vector2f(tamCasilla - 10, tamCasilla - 10));// Rectángulo ligeramente más pequeño
                marcador.setPosition({ i * tamCasilla + 5, j * tamCasilla + 5 });// Centrado en la casilla
                marcador.setOutlineThickness(4.0f); // Borde grueso
                marcador.setOutlineColor(sf::Color::Yellow); // Color amarillo para distinguirla
                marcador.setFillColor(sf::Color::Transparent); // Interior transparente
                ventanaJuego.draw(marcador);// Dibuja el marcador
            }
        }
    }
    // Dibuja el resaltado de movimientos válidos tras seleccionar una pieza
    for (Casilla* c : casillasValidas) {
        if (c == origenSeleccionado) continue;// no resalta la casilla propia
        int i = c->getX(); // Coordenada X de la casilla válida
        int j = c->getY(); // Coordenada Y de la casilla válida
        sf::RectangleShape resalte(sf::Vector2f(tamCasilla - 4, tamCasilla - 4));// Rectángulo de resaltado
        resalte.setPosition({ i * tamCasilla + 2, j * tamCasilla + 2 });// Posición ajustada al centro
        resalte.setFillColor(sf::Color::Transparent);// Sin relleno
        resalte.setOutlineThickness(3.0f);// Borde visible
        if (c->estaOcupada())
            resalte.setOutlineColor(sf::Color(255, 80, 80, 200)); // rojo: casilla con enemigo (ataque)
        else
            resalte.setOutlineColor(sf::Color(80, 255, 80, 200));// verde: casilla vacía (movimiento)
        ventanaJuego.draw(resalte); // Dibuja el resaltado
    }
    // Si hay un Hechicero seleccionado y puede usar magia, muestra los 7 botones de hechizos
    if (primerClicRealizado && dynamic_cast<Hechicero*>(origenSeleccionado->getPieza())) {
        bool yaUsoMagia = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;// Estado de magia del turno
        sf::Vector2f mousePos = ventanaJuego.mapPixelToCoords(sf::Mouse::getPosition(ventanaJuego), vistaEstatica);// Posición del ratón
        std::string nombres[] = { "Teleport", "Heal", "Shift Time", "Exchange", "Summon", "Revive", "Imprison" };// Nombres de hechizos
        for (int i = 0; i < botonesHechizos.size(); i++) {
            botonesHechizos[i]->setTexto(yaUsoMagia ? "AGOTADO" : nombres[i]);// muestra AGOTADO si ya usó magia
            botonesHechizos[i]->actualizarColorBoton(mousePos);// Actualiza el color del botón según el hover
            botonesHechizos[i]->dibujar(ventanaJuego);// Dibuja el botón
        }
    }
    // Fase 2 de victoria: parpadeo verde semitransparente sobre todo el tablero 
    if (estadoVictoria != 0 && faseVictoria == 2) {
        if (static_cast<int>(temporizadorFase * 10) % 2 == 0) {// Alterna cada décima de segundo
            sf::RectangleShape fondoVerde(vistaEstatica.getSize());// Rectángulo del tamaño de la vista
            fondoVerde.setOrigin({ vistaEstatica.getSize().x / 2.f, vistaEstatica.getSize().y / 2.f });// Origen en el centro
            fondoVerde.setPosition(vistaEstatica.getCenter()); // Posicionado en el centro de la vista
            fondoVerde.setFillColor(sf::Color(0, 255, 0, 120)); // Verde semitransparente
            ventanaJuego.draw(fondoVerde); // Dibuja el destello verde
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
// Comprueba si un movimiento es válido consultando a la propia pieza
bool Tablero::esMovimientoValido(Casilla* origen, Casilla* destino) {
    if (!origen || !destino) return false;// Si alguna casilla es nula, no es válido
    Pieza* p = origen->getPieza();// Obtiene la pieza en la casilla de origen
    if (p == nullptr) return false; // Si no hay pieza, no es válido
    return p->mover(origen, destino, matriz);// Delega la validación a la pieza (polimorfismo)
}
// Resetea la bandera de combate tras lanzar la Arena desde MotorArchon
void Tablero::limpiarBanderaCombate() {
    hayCombatePendiente = false;// Indica que el combate ya fue procesado
}
// Aplica el resultado de un combate al estado del tablero
void Tablero::procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal) {
    gestorCombate.procesarResultadoCombate(ganador, perdedor, atacanteOriginal, this);// Delega al gestor
    atacante = nullptr;// Limpia el puntero al atacante
    defensor = nullptr; // Limpia el puntero al defensor
    turnosContados++; // Incrementa el contador de turnos
    turnoActual = (turnoActual == Bando::LUZ) ? Bando::OSCURIDAD : Bando::LUZ;// Cambia el turno
}
// Ejecuta el hechizo activo sobre una casilla objetivo
void Tablero::procesarMagia(Casilla* objetivo) {
    if (!modoHechizoActivo || !origenSeleccionado) return;// No hace nada si no hay hechizo activo
    gestorHechizos.ejecutarHechizo(hechizoSeleccionado, objetivo, this);// Ejecuta el hechizo
    bool magiaCompletada = (turnoActual == Bando::LUZ) ? bandoLuzUsoMagia : bandoOscuroUsoMagia;// Comprueba si el hechizo se completó
    if (magiaCompletada) finalizarTurno();// Si el hechizo se ejecutó con éxito, finaliza el turno
}
// Crea y posiciona los botones de hechizos en el lateral derecho del tablero
void Tablero::inicializarBotones() {
    std::string nombres[] = { "Teleport", "Heal", "Shift Time", "Exchange", "Summon", "Revive", "Imprison" };// Nombres de los 7 hechizos
    float posX = (9 * tamCasilla) + 20;// a la derecha del tablero con 20px de margen
    for (int i = 0; i < 7; i++)// Crea un botón por cada hechizo
        botonesHechizos.push_back(new Boton(posX, 50.0f + ((float)i * 70.0f), 180, 50, nombres[i], fuente));
    // cada botón separado 70px del anterior, empezando a 50px del borde superior
}
// Devuelve el color actual de la casilla en las coordenadas dadas
ColorCasilla Tablero::getColorCasilla(int x, int y) {
    if (x < 0 || x >= 9 || y < 0 || y >= 9) return ColorCasilla::GRIS_CLARO;// Coordenadas fuera del tablero: gris por defecto
    return matriz[x][y]->getColorActual();// devuelve el color ya calculado por Casilla::dibujar()
}