#include "MenuPrincipal.h"
#include <iostream>
#include<fstream>
#include<sstream>

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
    cargarTextoManual();
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
    case OpcionesMenu::MANUAL_GRAFICO: actualizar(BotonesManual); break;
    }

    while (auto evento = ventana.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventana.close();
        }
        else if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (click->button == sf::Mouse::Button::Left) {
                if (EstadoInterno == OpcionesMenu::PRINCIPAL) {
                    if (BotonesMenuPrincipal[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
                    if (BotonesMenuPrincipal[1].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::RANKING;
                    if (BotonesMenuPrincipal[2].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::MANUAL_GRAFICO;
                    if (BotonesMenuPrincipal[3].botonContieneRaton(PosRaton)) ventana.close();
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
                else if (EstadoInterno == OpcionesMenu::MANUAL_GRAFICO) {
                    if (BotonesManual[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::PRINCIPAL; 
                
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
    case OpcionesMenu::MANUAL_GRAFICO: Titulos.setCharacterSize(60); titulo = "MANUAL"; break;
    }

    
    Titulos.setString(titulo); 
    sf::FloatRect limites = Titulos.getLocalBounds();
    if (EstadoInterno == OpcionesMenu::MANUAL_GRAFICO) {
        sf::Vector2f posManual = sf::Vector2f(120.0f, 130.0f);
        Titulos.setPosition({ posManual.x + (860.0f - limites.size.x) / 2.0f, posManual.y - 80.0f });
    }
    else {
        Titulos.setPosition({ ((1100.0f - limites.size.x) / 2.0f) + 10.0f, 50.0f - limites.position.y });
    }
    ventana.draw(Titulos);
   
    auto dibujarLista = [&](std::vector<Boton>& lista) { for (auto& b : lista) b.dibujar(ventana); };
    switch (EstadoInterno) {
    case OpcionesMenu::PRINCIPAL: dibujarLista(BotonesMenuPrincipal); break;
    case OpcionesMenu::RANKING: dibujarLista(BotonesRanking); break;
    case OpcionesMenu::SELECCION_MODO: dibujarLista(BotonesSeleccionModo); break;
    case OpcionesMenu::IA_NODISPONIBLE: dibujarLista(BotonVolverIA); break;
    case OpcionesMenu::SELECCION_SKIN: dibujarLista(BotonesSeleccionSkin); break;
    case OpcionesMenu::MANUAL_GRAFICO:
    {
        sf::Vector2f posManual = sf::Vector2f(120.0f, 130.0f);
        sf::RectangleShape fondoMorado;
        fondoMorado.setSize(sf::Vector2f(860.0f, 660.0f)); 
        fondoMorado.setFillColor(sf::Color(35, 15, 55, 230)); 
        fondoMorado.setOutlineColor(sf::Color(150, 60, 240)); 
        fondoMorado.setOutlineThickness(3.0f);
        fondoMorado.setPosition(posManual);
        ventana.draw(fondoMorado);


        for (const auto& linea : lineasReglas) {
            ventana.draw(linea);
        }
        for (const auto& linea : lineasHechizos) {
            ventana.draw(linea);
        }
        dibujarLista(BotonesManual);
    }
    break;
        
    }
}

void MenuPrincipal::inicializarBotones() {
    float anchoVentana = 1100.0f;
    float ancho = 300.0f;
    float cx = 400.0f;
    float alto = 50.0f, esp = 80.0f;
    BotonesMenuPrincipal.clear();
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f, ancho, alto, "INICIAR PARTIDA", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp, ancho, alto, "RANKING", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp *2, ancho, alto, "MANUAL", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp * 3, ancho, alto, "SALIR", FuenteMenu));

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
    
    BotonesManual.clear();
    BotonesManual.push_back(Boton(cx, 800.0f,ancho,alto, "VOLVER", FuenteMenu));
   
}
void MenuPrincipal::cargarTextoManual() {
    std::ifstream archivo("Manual.txt");
    std::string linea;

    
    lineasReglas.clear();
    lineasHechizos.clear();

    if (!archivo.is_open()) {
        sf::Text errorTxt(FuenteMenu);
        errorTxt.setString("Error: No se pudo encontrar el archivo 'Manual.txt'");
        errorTxt.setCharacterSize(20);
        errorTxt.setFillColor(sf::Color::Red);
        errorTxt.setPosition(sf::Vector2f(100.f, 150.f));
        lineasReglas.push_back(errorTxt);
        return;
    }

    float yOffset = 150.0f; 
    bool leyendoHechizos = false; 
    while (std::getline(archivo, linea)) {
        
        if (linea == "== HECHIZOS ==") {
            leyendoHechizos = true;
            yOffset = 150.0f; 
            continue;         
        }

        sf::Text t(FuenteMenu);
        t.setString(linea);
        t.setCharacterSize(14);
        t.setFillColor(sf::Color::White);

        if (!leyendoHechizos) {
           
            t.setPosition(sf::Vector2f(140.0f, yOffset));
            lineasReglas.push_back(t);
        }
        else {
           
            t.setPosition(sf::Vector2f(560.0f, yOffset));
            lineasHechizos.push_back(t);
        }

        yOffset += 20.0f; 
    }
    archivo.close();
}