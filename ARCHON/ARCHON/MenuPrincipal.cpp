#include "MenuPrincipal.h"
#include <iostream>
#include <fstream>
#include <sstream>

MenuPrincipal::MenuPrincipal() : SpriteFondo(FondoMenu), Titulos(FuenteMenu) {//sfml 3.0 me pide que lo inicialice al crearlo
    EstadoInterno = OpcionesMenu::PRINCIPAL;//se inicia en principal
    IniciarJuego = false;//flag para iniciar el juego a 0
    SkinSeleccionada = "ARCHON";//la skin por defecto es la de archon
    if (!FuenteMenu.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {//se carga la fuente, si no la encuerta error
        std::cout << "Error cargando la fuente\n\n";
    }
    Titulos.setCharacterSize(100);//tamaño de letra para los titulos
    Titulos.setFillColor(sf::Color::White);//seleccion del color de letra
    Titulos.setOutlineThickness(4.0f);//seleccion del grosor del borde de la letra
    Titulos.setOutlineColor(sf::Color::Black);//seleccion del color del borde de la letra

    // carga de texturas de fondo en la memoria ram o error si no encuentra el archivo
    if (!FondoMenu.loadFromFile("../ARCHON/imagenes/Fondo Menu 800x800.png")) std::cout << "Error fondo menu\n";
    if (!FondoStarWars.loadFromFile("../ARCHON/imagenes/DarthVaderEpica.png")) std::cout << "Error fondo SW\n";
    if (!FondoHarryPotter.loadFromFile("../ARCHON/imagenes/VoldemortEpico.png")) std::cout << "Error fondo HP\n";
    if (!FondoArchon.loadFromFile("../ARCHON/imagenes/FondoArchon.png")) std::cout << "Error fondo Archon\n";

    // configuración inicial del sprite de fondo
    SpriteFondo.setTexture(FondoMenu, true);//le asignamos la textura del fondo al sprite
    float EscalaX = 1100.0f / FondoMenu.getSize().x;//se le da un tamaño de 1100x855 para que se adapte al ancho de la ventana
    float EscalaY = 855.0f / FondoMenu.getSize().y;//se le da un tamaño de 1100x855 para que se adapte al alto de la ventana
    SpriteFondo.setScale(sf::Vector2f(EscalaX, EscalaY));//se le asignan los tamaños al sprite

    inicializarBotones();//metodo que inicializa todos los botones del menú
    cargarTextoManual();//metodo que carga el texto del manual desde un archivo de texto
}

MenuPrincipal::~MenuPrincipal() {}//destructor vacío, no hay nada creado con new

void MenuPrincipal::procesarEntrada(sf::RenderWindow& ventana) {//metodo para procesar las entradas del usuario
    sf::Vector2f PosRaton = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));//obtenemos la posicion del raton relativa a la ventana
    auto actualizar = [&](std::vector<Boton>& lista) {//funcion lambda que actualiza el color de los botones segun el raton
        for (auto& b : lista) b.actualizarColorBoton(PosRaton);
        };

    SpriteFondo.setTexture(FondoMenu, true);//se asigna el fondo por defecto al sprite
    if (EstadoInterno == OpcionesMenu::SELECCION_SKIN) {//si estamos seleccionando la skin
        sf::Texture* texturaPuntero = &FondoMenu;//puntero a textura para cambiar el fondo segun la skin que se seleccione
        if (BotonesSeleccionSkin[0].botonContieneRaton(PosRaton)) texturaPuntero = &FondoArchon;//se le asigna la textura del fondo segun el boton que se seleccione usando el puntero
        else if (BotonesSeleccionSkin[1].botonContieneRaton(PosRaton)) texturaPuntero = &FondoHarryPotter;
        else if (BotonesSeleccionSkin[2].botonContieneRaton(PosRaton)) texturaPuntero = &FondoStarWars;

        SpriteFondo.setTexture(*texturaPuntero, true);//se le asigna la textura al sprite
        SpriteFondo.setScale(sf::Vector2f(1100.0f / texturaPuntero->getSize().x, 855.0f / texturaPuntero->getSize().y));
    }//asigno la escala del fondo segun la textura que se le asigne al sprite(hace conversion al tamaño de la ventana)

    switch (EstadoInterno) {//se usa un switch para actualizar el color de los botones segun el estado del menú(la funcion lambda)
    case OpcionesMenu::PRINCIPAL: actualizar(BotonesMenuPrincipal); break;
    case OpcionesMenu::RANKING: actualizar(BotonesRanking); break;
    case OpcionesMenu::SELECCION_MODO: actualizar(BotonesSeleccionModo); break;
    case OpcionesMenu::SELECCION_SKIN: actualizar(BotonesSeleccionSkin); break;
    case OpcionesMenu::MANUAL_GRAFICO: actualizar(BotonesManual); break;
    }

    while (auto evento = ventana.pollEvent()) {//bucle para procesar los eventos de la ventana, asigno a evento el resultado de la comprobacion de evento
        if (evento->is<sf::Event::Closed>()) {//si hay un evento de cierre de ventana, se cierra la ventana
            ventana.close();//el cierre
        }
        else if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {//si es un click del raton
            if (click->button == sf::Mouse::Button::Left) {//y es el click izquierdo
                if (EstadoInterno == OpcionesMenu::PRINCIPAL) {//transitamos al menu que toque segun el boton pulsado
                    if (BotonesMenuPrincipal[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
                    if (BotonesMenuPrincipal[1].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::RANKING;
                    if (BotonesMenuPrincipal[2].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::MANUAL_GRAFICO;
                    if (BotonesMenuPrincipal[3].botonContieneRaton(PosRaton)) ventana.close();
                }//el boton usa su metodo botonContieneRaton para comprobar si el click se ha producido dentro de sus limites
                else if (EstadoInterno == OpcionesMenu::SELECCION_MODO) {
                    if (BotonesSeleccionModo[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_SKIN;
                    if (BotonesSeleccionModo[1].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::PRINCIPAL;
                }
                else if (EstadoInterno == OpcionesMenu::SELECCION_SKIN) {//aquí seleccionamos la skin(se modifica la variable) y se inicia el juego
                    if (BotonesSeleccionSkin[0].botonContieneRaton(PosRaton)) { SkinSeleccionada = "ARCHON"; IniciarJuego = true; }
                    else if (BotonesSeleccionSkin[1].botonContieneRaton(PosRaton)) { SkinSeleccionada = "HARRY_POTTER"; IniciarJuego = true; }
                    else if (BotonesSeleccionSkin[2].botonContieneRaton(PosRaton)) { SkinSeleccionada = "STAR_WARS"; IniciarJuego = true; }
                    else if (BotonesSeleccionSkin[3].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
                }
                else if (EstadoInterno == OpcionesMenu::RANKING) {
                    if (BotonesRanking[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::PRINCIPAL;
                }
                else if (EstadoInterno == OpcionesMenu::MANUAL_GRAFICO) {
                    if (BotonesManual[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::PRINCIPAL;
                }
            }
        }
    }
}

void MenuPrincipal::dibujarPantalla(sf::RenderWindow& ventana) {//metodo que usamos para pintar la ventana
    ventana.draw(SpriteFondo);//pinta el fondo
    std::string titulo = "";//string para el titulo del menú, se asigna segun el estado del menú en el switch siguiente

    switch (EstadoInterno) {//switch para pintar el titulo sobre el fondo segun el estado
    case OpcionesMenu::PRINCIPAL: Titulos.setCharacterSize(150); titulo = "ARCHON"; break;
    case OpcionesMenu::RANKING: Titulos.setCharacterSize(100); titulo = "RANKING"; break;
    case OpcionesMenu::SELECCION_MODO: Titulos.setCharacterSize(80); titulo = "MODO DE JUEGO"; break;
    case OpcionesMenu::SELECCION_SKIN: Titulos.setCharacterSize(100); titulo = "TEMATICA"; break;
    case OpcionesMenu::MANUAL_GRAFICO: Titulos.setCharacterSize(60); titulo = "MANUAL"; break;
    }

    Titulos.setString(titulo); //asignamos el titulo al objeto de texto
    sf::FloatRect limites = Titulos.getLocalBounds();//obtenemos los limites del texto para centrarlo

    if (EstadoInterno == OpcionesMenu::MANUAL_GRAFICO) {//si estamos en el manual, el titulo se centra dentro del fondo morado, no de la ventana
        sf::Vector2f posManual = sf::Vector2f(120.0f, 130.0f);//para poder centrarlo a mano
        Titulos.setPosition({ posManual.x + (860.0f - limites.size.x) / 2.0f, posManual.y - 80.0f });
    }
    else {
        Titulos.setPosition({ ((1100.0f - limites.size.x) / 2.0f) + 10.0f, 50.0f - limites.position.y });
    }
    ventana.draw(Titulos);//aquí le digo que pinte el titulo

    auto dibujarLista = [&](std::vector<Boton>& lista) { for (auto& b : lista) b.dibujar(ventana); };//funcion lambda para recorrer los botones y dibujarlos

    switch (EstadoInterno) {//switch para dibujar los botones segun el estado del menú usando la función lambda
    case OpcionesMenu::PRINCIPAL: dibujarLista(BotonesMenuPrincipal); break;
    case OpcionesMenu::RANKING: dibujarLista(BotonesRanking); break;
    case OpcionesMenu::SELECCION_MODO: dibujarLista(BotonesSeleccionModo); break;
    case OpcionesMenu::SELECCION_SKIN: dibujarLista(BotonesSeleccionSkin); break;
    case OpcionesMenu::MANUAL_GRAFICO:
    {//ajustes del texto del manual, se dibuja un fondo morado semitransparente para que se vea el fondo
    //y se dibujan las lineas del manual del texto
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
        dibujarLista(BotonesManual);//dibujamos el boton de volver del manual
    }
    break;
    }
}

void MenuPrincipal::inicializarBotones() {//se inicializan los botones del menu y se les asignan los paramteros
    float ancho = 300.0f;
    float cx = 400.0f;
    float alto = 50.0f, esp = 80.0f;//parametros para los botones

    BotonesMenuPrincipal.clear();//vaciamos el vector de botones para evitar acomular botones al movernos por los menus
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f, ancho, alto, "INICIAR PARTIDA", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp, ancho, alto, "RANKING", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp * 2, ancho, alto, "MANUAL", FuenteMenu));
    BotonesMenuPrincipal.push_back(Boton(cx, 250.0f + esp * 3, ancho, alto, "SALIR", FuenteMenu));

    BotonesSeleccionModo.clear();
    BotonesSeleccionModo.push_back(Boton(cx, 250.0f, ancho, alto, "JUGADOR VS JUGADOR", FuenteMenu));
    BotonesSeleccionModo.push_back(Boton(cx, 250.0f + esp, ancho, alto, "VOLVER", FuenteMenu));

    BotonesSeleccionSkin.clear();
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f, ancho, alto, "ARCHON", FuenteMenu));
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f + esp, ancho, alto, "HARRY POTTER", FuenteMenu));
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f + esp * 2, ancho, alto, "STAR WARS", FuenteMenu));
    BotonesSeleccionSkin.push_back(Boton(cx, 250.0f + esp * 3, ancho, alto, "VOLVER", FuenteMenu));

    BotonesManual.clear();
    BotonesManual.push_back(Boton(cx, 800.0f, ancho, alto, "VOLVER", FuenteMenu));
}

void MenuPrincipal::cargarTextoManual() {// se carga el texto del manual desde un archivo de texto
    std::ifstream archivo("Manual.txt");//abro el archivo de texto del manual
    std::string linea;//declaro una variable para almacenar cada linea del archivo

    lineasReglas.clear();//vaciamos los vectores de texto para evitar acumular texto al entrar y salir del manual
    lineasHechizos.clear();
    if (!archivo.is_open()) {//si el archivo no se ha podido abrir lanza error de texto
        std::cout << "Error cargando la fuente\n\n";
        return;//para salir del metodo si no se ha podido abrir el archivo
    }
    float yOffset = 150.0f;//offset para la posicion vertical del texto
    bool leyendoHechizos = false;//bandera para saber si estamos leyendo la parte de hechizos o la de reglas

    while (std::getline(archivo, linea)) {//vamos obteniendo linea a linea del archivo hasta el final
        if (linea == "== HECHIZOS ==") {//si nos cruzamos con esta linea
            leyendoHechizos = true;//ponemos la bandera a true para empezar a leer los hechizos
            yOffset = 150.0f; //reiniciamos el offset para que los hechizos empiecen a la misma altura que las reglas
            continue;
        }
        sf::Text t(FuenteMenu);//objeto para el texto y lo configuramos
        t.setString(linea);
        t.setCharacterSize(14);
        t.setFillColor(sf::Color::White);

        if (!leyendoHechizos) {//si no estamos leyendo hechizos
            t.setPosition(sf::Vector2f(140.0f, yOffset));//pinta las reglas a la izquierda del fondo morado
            lineasReglas.push_back(t);//va metiendo las lineas de texto en el vector de reglas
        }
        else {//si estamos leyendo hechizos
            t.setPosition(sf::Vector2f(560.0f, yOffset));//lo mismo pero a la derecha
            lineasHechizos.push_back(t);//lo mismo
        }
        yOffset += 20.0f; //se le va dando un offset vertical para que no se solapen las lineas
    }
    archivo.close();//cuando se acaba cierro el archivo
}