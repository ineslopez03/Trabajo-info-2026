#include "PantallaRanking.h"
#include "MotorArchon.h"
#include <fstream>
#include <iostream>

PantallaRanking::PantallaRanking() : titulo(fuente), fondoSprite(fondoTextura) {
    if (!fuente.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {//cargamos la fuente, si no la encuentra error
        std::cout << "Error cargando la fuente en Ranking" << std::endl;
    }

    titulo.setString("RANKING");//configuramos el titulo
    titulo.setCharacterSize(60);
    titulo.setFillColor(sf::Color::White);
    titulo.setOutlineThickness(4.0f);
    titulo.setOutlineColor(sf::Color::Black);

    if (fondoTextura.loadFromFile("../ARCHON/imagenes/Fondo Menu 800x800.png")) {//cargamos la textura del fondo
        fondoSprite.setTexture(fondoTextura, true);//le asignamos la textura del fondo al sprite
        float EscalaX = 1100.0f / fondoTextura.getSize().x;//se le da un tamaño de 1100x855 para que se adapte al ancho de la ventana
        float EscalaY = 855.0f / fondoTextura.getSize().y;//se le da un tamaño de 1100x855 para que se adapte al alto de la ventana
        fondoSprite.setScale(sf::Vector2f(EscalaX, EscalaY));//se le asignan los tamaños al sprite
    }

    botones.push_back(Boton(400.0f, 670.0f, 300.0f, 50.0f, "VOLVER", fuente));//creamos el boton de volver al menu
    cargarDatos();//llamamos a la funcion para cargar los datos del ranking desde el archivo
}

void PantallaRanking::cargarDatos() {
    lineasRanking.clear();//vaciamos el vector de lineas del ranking por si acaso, para evitar que se acumulen datos
    std::ifstream archivo("ranking.txt"); // Busca el archivo
    std::string nombre;//variables para almacenar el nombre y el numero de victorias leidos del archivo
    int victorias;
    float posY = 180.f;

    if (archivo.is_open()) {//si lo consigue abrir
        int puesto = 1;//variable para llevar la cuenta del puesto en el ranking, empezando por el 1
        while (archivo >> nombre >> victorias && puesto <= 10) {//mientras podamos extraer un nombre y un numero de victorias
            sf::Text t(fuente, std::to_string(puesto) + ". " + nombre + " - " + std::to_string(victorias) + " Victorias", 30);
            //configuramos el texto de cada linea del ranking con el formato "puesto. nombre - victorias Victorias"
            t.setFillColor(sf::Color::White);//de color blanco
            sf::FloatRect b = t.getLocalBounds();//obtenemos los limites del texto para centrarl
            t.setOrigin({ b.size.x / 2.f, 0.f });//centramos el texto horizontalmente
            t.setPosition({ 550.f, posY });//lo posicionamos, con posY que se va incrementando para cada linea
            lineasRanking.push_back(t);//lo agregamos al vector de lineas del ranking para luego dibujarlo en pantalla
            posY += 45.f; // se reduce el espaciado para que quepan más lineas
            puesto++;//incrementamos el puesto para la siguiente linea del rankin
        }
        archivo.close();//acabado se cierra el archivo
    }

    if (lineasRanking.empty()) {// si no se han podido cargar datos del ranking
        sf::Text aviso(fuente, "Aun no hay registros. Juega una partida!", 30);//se muestra un mensaje
        aviso.setFillColor(sf::Color(200, 200, 200)); // Gris claro
        sf::FloatRect b = aviso.getLocalBounds();//obtenemos los limites del texto para centarlo
        aviso.setOrigin({ b.size.x / 2.f, 0.f });//el texto se centra horizontalmente
        aviso.setPosition({ 550.f, 350.f }); // y se posiciona en el centro de la pantalla
        lineasRanking.push_back(aviso);//se agrega el mensaje al vector de lineas
    }
}

void PantallaRanking::procesarEntrada(sf::RenderWindow& ventana, MotorArchon* motor) {//metodo para procesar las entradas
    sf::Vector2f mousePos = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));//obtenemos la posicion del raton en terminos de coordenadas

    while (const std::optional<sf::Event> evento = ventana.pollEvent()) {//extraemos el evento
        if (evento->is<sf::Event::Closed>()) {//si se cierra la ventana
            ventana.close();//la cierra
        }
        if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {//si detecta click del raton
            if (click->button == sf::Mouse::Button::Left) {//y resulta ser el izquierdo
                // Comprobamos si ha hecho click en el primer botón (Volver)
                if (!botones.empty() && botones[0].botonContieneRaton(mousePos)) {//si hay botones y ocurre sobre el boton
                    motor->encolarCambioEstado(EstadoJuego::MENU);//transición al menú directamente gestionada
                }
            }
        }
    }

    // Iluminar botón al pasar el ratón
    if (!botones.empty()) {//si hay botones
        botones[0].actualizarColorBoton(mousePos);//llamo al metodo del raton
    }
}

void PantallaRanking::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.draw(fondoSprite);//dibujamos el fondo
    sf::FloatRect bounds = titulo.getLocalBounds();//obtenemos los limites del titulo
    titulo.setPosition({ (1100.0f - bounds.size.x) / 2.0f, 50.0f });//lo centramos horizontalmente

    sf::RectangleShape fondoMorado({ 700.0f, 600.0f });//se ajusta el rectangulo morado
    fondoMorado.setFillColor(sf::Color(35, 15, 55, 150));
    fondoMorado.setOutlineColor(sf::Color(150, 60, 240));
    fondoMorado.setOutlineThickness(3.0f);
    fondoMorado.setPosition({ 200.0f, 130.0f });

    ventana.draw(fondoMorado);//se pinta

    // Dibujar textos
    for (auto& l : lineasRanking) {//recorre todas las lineas y las pinta
        ventana.draw(l);
    }

    // Dibujar botones
    for (auto& b : botones) {//recorre el vector de botones y los pinta(solo hay uno)
        b.dibujar(ventana);
    }
}