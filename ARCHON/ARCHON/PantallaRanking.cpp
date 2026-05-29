#include "PantallaRanking.h"
#include <fstream>
#include <iostream>

PantallaRanking::PantallaRanking() : transicionMenu(false), titulo(fuente), fondoSprite(fondoTextura) {

    // 1. Blindamos la fuente como hicimos en PantallaNombre
    if (!fuente.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {
        if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cout << "Error cargando la fuente en Ranking" << std::endl;
        }
    }

    titulo.setString("RANKING");
    titulo.setCharacterSize(60);
    titulo.setFillColor(sf::Color::White);
    titulo.setOutlineThickness(4.0f);
    titulo.setOutlineColor(sf::Color::Black);

    if (fondoTextura.loadFromFile("../ARCHON/imagenes/Fondo Menu 800x800.png")) {
        fondoSprite.setTexture(fondoTextura, true);//le asignamos la textura del fondo al sprite
        float EscalaX = 1100.0f / fondoTextura.getSize().x;//se le da un tamaño de 1100x855 para que se adapte al ancho de la ventana
        float EscalaY = 855.0f / fondoTextura.getSize().y;//se le da un tamaño de 1100x855 para que se adapte al alto de la ventana
        fondoSprite.setScale(sf::Vector2f(EscalaX, EscalaY));//se le asignan los tamaños al sprite
    }

    botones.push_back(Boton(400.0f, 670.0f, 300.0f, 50.0f, "VOLVER", fuente));

    cargarDatos();
}

void PantallaRanking::cargarDatos() {
    lineasRanking.clear();
    std::ifstream archivo("ranking.txt"); // Busca el archivo
    std::string nombre;
    int victorias;
    float posY = 180.f;

    if (archivo.is_open()) {
        int puesto = 1;
        while (archivo >> nombre >> victorias && puesto <= 10) {
            sf::Text t(fuente, std::to_string(puesto) + ". " + nombre + " - " + std::to_string(victorias) + " Victorias", 30);
            t.setFillColor(sf::Color::White);

            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin({ b.size.x / 2.f, 0.f });
            t.setPosition({ 550.f, posY });

            lineasRanking.push_back(t);
            posY += 45.f; // Lo juntamos un poquito más para que quepan 10
            puesto++;
        }
        archivo.close();
    }

    // 3. MAGIA: Si el archivo no existía o estaba vacío, mostramos este aviso
    if (lineasRanking.empty()) {
        sf::Text aviso(fuente, "Aun no hay registros. Juega una partida!", 30);
        aviso.setFillColor(sf::Color(200, 200, 200)); // Gris claro

        sf::FloatRect b = aviso.getLocalBounds();
        aviso.setOrigin({ b.size.x / 2.f, 0.f });
        aviso.setPosition({ 550.f, 350.f }); // En todo el centro del recuadro

        lineasRanking.push_back(aviso);
    }
}

void PantallaRanking::procesarEntrada(sf::RenderWindow& ventana) {
    sf::Vector2f mousePos = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));

    while (const std::optional<sf::Event> evento = ventana.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventana.close();
        }

        if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {
            if (click->button == sf::Mouse::Button::Left) {
                // Comprobamos si ha hecho click en el primer botón (Volver)
                if (!botones.empty() && botones[0].botonContieneRaton(mousePos)) {
                    transicionMenu = true;
                }
            }
        }
    }

    // Iluminar botón al pasar el ratón
    if (!botones.empty()) {
        botones[0].actualizarColorBoton(mousePos);
    }
}

void PantallaRanking::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.draw(fondoSprite);

    sf::FloatRect bounds = titulo.getLocalBounds();
    titulo.setPosition({ (1100.0f - bounds.size.x) / 2.0f, 50.0f });
    ventana.draw(titulo);

    sf::RectangleShape fondoMorado({ 700.0f, 600.0f });
    fondoMorado.setFillColor(sf::Color(35, 15, 55, 230));
    fondoMorado.setOutlineColor(sf::Color(150, 60, 240));
    fondoMorado.setOutlineThickness(3.0f);
    fondoMorado.setPosition({ 200.0f, 130.0f });
    ventana.draw(fondoMorado);

    // Dibujar textos
    for (auto& l : lineasRanking) {
        ventana.draw(l);
    }

    // Dibujar botones
    for (auto& b : botones) {
        b.dibujar(ventana);
    }
}