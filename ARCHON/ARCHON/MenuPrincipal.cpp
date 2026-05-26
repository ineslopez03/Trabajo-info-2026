#include "MenuPrincipal.h"
#include <iostream>

MenuPrincipal::MenuPrincipal() : SpriteFondo(FondoMenu), Titulos(FuenteMenu) {
    EstadoInterno = OpcionesMenu::PRINCIPAL;
    ContraIA = false;
    IniciarJuego = false;
    SkinSeleccionada = "ARCHON"; // Consistencia con el resto del proyecto

    if (!FuenteMenu.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {
        std::cout << "Error cargando la fuente\n\n";
    }

    Titulos.setCharacterSize(100);
    Titulos.setFillColor(sf::Color::White);
    Titulos.setOutlineThickness(4.0f);
    Titulos.setOutlineColor(sf::Color::Black);

    if (!MusicaFondo.openFromFile("../ARCHON/Musica/MusicaFondo.mp3")) {
        std::cout << "Error cargando la Musica\n\n";
    }
    else {
        MusicaFondo.setVolume(50.0f);
        MusicaFondo.setLooping(true);
        MusicaFondo.play();
    }

    // Carga de texturas de fondo
    if (!FondoMenu.loadFromFile("../ARCHON/imagenes/Fondo Menu 800x800.png")) std::cout << "Error fondo menu\n";
    if (!FondoStarWars.loadFromFile("../ARCHON/imagenes/DarthVaderEpica.png")) std::cout << "Error fondo SW\n";
    if (!FondoHarryPotter.loadFromFile("../ARCHON/imagenes/VoldemortEpico.png")) std::cout << "Error fondo HP\n";
    if (!FondoArchon.loadFromFile("../ARCHON/imagenes/FondoArchon.png")) std::cout << "Error fondo Archon\n";

    // Configuración inicial del sprite de fondo
    SpriteFondo.setTexture(FondoMenu, true);
    float EscalaX = 1100.0f / FondoMenu.getSize().x;
    float EscalaY = 855.0f / FondoMenu.getSize().y;
    SpriteFondo.setScale(sf::Vector2f(EscalaX, EscalaY));

    inicializarBotones();
}

MenuPrincipal::~MenuPrincipal() {}

void MenuPrincipal::procesarEntrada(sf::RenderWindow& ventana) {
    sf::Vector2f PosRaton = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));

    auto actualizar = [&](std::vector<Boton>& lista) {
        for (auto& b : lista) b.actualizarColorBoton(PosRaton);
        };

    // Lógica de cambio de fondo dinámico según hover en selección de skin
    SpriteFondo.setTexture(FondoMenu, true);
    if (EstadoInterno == OpcionesMenu::SELECCION_SKIN) {
        sf::Texture* texturaPuntero = &FondoMenu;

        if (BotonesSeleccionSkin[0].botonContieneRaton(PosRaton)) texturaPuntero = &FondoArchon;
        else if (BotonesSeleccionSkin[1].botonContieneRaton(PosRaton)) texturaPuntero = &FondoHarryPotter;
        else if (BotonesSeleccionSkin[2].botonContieneRaton(PosRaton)) texturaPuntero = &FondoStarWars;

        SpriteFondo.setTexture(*texturaPuntero, true);
        SpriteFondo.setScale(sf::Vector2f(1100.0f / texturaPuntero->getSize().x, 1100.0f / texturaPuntero->getSize().y));
    }

    switch (EstadoInterno) {
    case OpcionesMenu::PRINCIPAL: actualizar(BotonesMenuPrincipal); break;
    case OpcionesMenu::RANKING: actualizar(BotonesRanking); break;
    case OpcionesMenu::SELECCION_MODO: actualizar(BotonesSeleccionModo); break;
    case OpcionesMenu::IA_NODISPONIBLE: actualizar(BotonVolverIA); break;
    case OpcionesMenu::SELECCION_SKIN: actualizar(BotonesSeleccionSkin); break;
    }

    while (auto evento = ventana.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventana.close();
        }
        else if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (click->button == sf::Mouse::Button::Left) {
                if (EstadoInterno == OpcionesMenu::PRINCIPAL) {
                    if (BotonesMenuPrincipal[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
                    if (BotonesMenuPrincipal[2].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::RANKING;
                    if (BotonesMenuPrincipal[3].botonContieneRaton(PosRaton)) system("start Manual.txt");
                    if (BotonesMenuPrincipal[4].botonContieneRaton(PosRaton)) ventana.close();
                }
                else if (EstadoInterno == OpcionesMenu::SELECCION_MODO) {
                    if (BotonesSeleccionModo[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_SKIN;
                    if (BotonesSeleccionModo[1].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::IA_NODISPONIBLE;
                    if (BotonesSeleccionModo[2].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::PRINCIPAL;
                }
                else if (EstadoInterno == OpcionesMenu::SELECCION_SKIN) {
                    // Normalización de cadenas para el MotorArchon
                    if (BotonesSeleccionSkin[0].botonContieneRaton(PosRaton)) { SkinSeleccionada = "ARCHON"; IniciarJuego = true; }
                    else if (BotonesSeleccionSkin[1].botonContieneRaton(PosRaton)) { SkinSeleccionada = "HARRY_POTTER"; IniciarJuego = true; }
                    else if (BotonesSeleccionSkin[2].botonContieneRaton(PosRaton)) { SkinSeleccionada = "STAR_WARS"; IniciarJuego = true; }
                    else if (BotonesSeleccionSkin[3].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
                }
                else if (EstadoInterno == OpcionesMenu::RANKING) {
                    if (BotonesRanking[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::PRINCIPAL;
                }
                else if (EstadoInterno == OpcionesMenu::IA_NODISPONIBLE) {
                    if (BotonVolverIA[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
                }
            }
        }
    }
}

void MenuPrincipal::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.draw(SpriteFondo);
    std::string titulo = "";

    switch (EstadoInterno) {
    case OpcionesMenu::PRINCIPAL: Titulos.setCharacterSize(150); titulo = "ARCHON"; break;
    case OpcionesMenu::RANKING: Titulos.setCharacterSize(100); titulo = "RANKING"; break;
    case OpcionesMenu::SELECCION_MODO: Titulos.setCharacterSize(80); titulo = "MODO DE JUEGO"; break;
    case OpcionesMenu::IA_NODISPONIBLE: Titulos.setCharacterSize(50); titulo = "DLC-IA POR 99 EUROS"; break;
    case OpcionesMenu::SELECCION_SKIN: Titulos.setCharacterSize(100); titulo = "TEMATICA"; break;
    }

    
    Titulos.setString(titulo); 
    sf::FloatRect limites = Titulos.getLocalBounds();
    Titulos.setPosition({ (1100.0f - limites.size.x) / 2.0f, 50.0f - limites.position.y });
    ventana.draw(Titulos);
   
    auto dibujarLista = [&](std::vector<Boton>& lista) { for (auto& b : lista) b.dibujar(ventana); };
    switch (EstadoInterno) {
    case OpcionesMenu::PRINCIPAL: dibujarLista(BotonesMenuPrincipal); break;
    case OpcionesMenu::RANKING: dibujarLista(BotonesRanking); break;
    case OpcionesMenu::SELECCION_MODO: dibujarLista(BotonesSeleccionModo); break;
    case OpcionesMenu::IA_NODISPONIBLE: dibujarLista(BotonVolverIA); break;
    case OpcionesMenu::SELECCION_SKIN: dibujarLista(BotonesSeleccionSkin); break;
    }
}

void MenuPrincipal::inicializarBotones() {
    float anchoVentana = 1100.0f;
    float ancho = 300.0f;
    float cx = 400.0f;
    float alto = 50.0f, esp = 80.0f;
    BotonesMenuPrincipal.clear();
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f, ancho, alto, "INICIAR PARTIDA", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp, ancho, alto, "CARGAR PARTIDA", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp * 2, ancho, alto, "RANKING", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp * 3, ancho, alto, "MANUAL", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp * 4, ancho, alto, "SALIR", FuenteMenu));

    BotonesSeleccionModo.clear();
    BotonesSeleccionModo.push_back(Boton(cx, 250.0f, ancho, alto, "JUGADOR VS JUGADOR", FuenteMenu));
    BotonesSeleccionModo.push_back(Boton(cx, 250.0f + esp, ancho, alto, "JUGADOR VS IA", FuenteMenu));
    BotonesSeleccionModo.push_back(Boton(cx, 250.0f + esp * 2, ancho, alto, "VOLVER", FuenteMenu));

    BotonesSeleccionSkin.clear();
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f, ancho, alto, "ARCHON", FuenteMenu));
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f + esp, ancho, alto, "HARRY POTTER", FuenteMenu));
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f + esp * 2, ancho, alto, "STAR WARS", FuenteMenu));
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f + esp * 3, ancho, alto, "VOLVER", FuenteMenu));

    BotonVolverIA.clear();
    BotonVolverIA.push_back(Boton(cx, 250.0f, ancho, alto, "NO DISPONIBLE-VOLVER", FuenteMenu));
    BotonesRanking.clear();
    BotonesRanking.push_back(Boton(cx, 250.0f, ancho, alto, "VOLVER", FuenteMenu));
}