#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <optional>

class PantallaRanking {
private:
    sf::Font fuente;
    sf::Text* titulo;             // Convertido a puntero
    std::vector<sf::Text*> lineasRanking; // Vector de punteros para evitar el error en la carga dinámica
    sf::RectangleShape botonVolver;
    sf::Text* textoVolver;        // Convertido a puntero
    bool transicionMenu;

public:
    PantallaRanking() {
        transicionMenu = false;

        if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cout << "Error al cargar la fuente en Ranking." << std::endl;
        }

        // Configurar Título pasando la fuente obligatoria al constructor
        titulo = new sf::Text(fuente);
        titulo->setString("TOP RANKING - ARCHON");
        titulo->setCharacterSize(45);
        titulo->setFillColor(sf::Color::Yellow);
        titulo->setPosition({ 350.f, 50.f });

        // Configurar Botón Volver
        botonVolver.setSize({ 200.f, 60.f });
        botonVolver.setFillColor(sf::Color(50, 50, 50));
        botonVolver.setOutlineThickness(2.f);
        botonVolver.setOutlineColor(sf::Color::White);
        botonVolver.setPosition({ 450.f, 700.f });

        // Configurar Texto del Botón pasando la fuente obligatoria al constructor
        textoVolver = new sf::Text(fuente);
        textoVolver->setString("VOLVER");
        textoVolver->setCharacterSize(25);
        textoVolver->setFillColor(sf::Color::White);

        // Centrar texto en el botón
        sf::FloatRect bounds = textoVolver->getLocalBounds();
        textoVolver->setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        textoVolver->setPosition({ 450.f + 100.f, 700.f + 30.f });

        cargarDatos();
    }

    // Destructor para liberar la memoria de los punteros creados
    ~PantallaRanking() {
        if (titulo != nullptr) delete titulo;
        if (textoVolver != nullptr) delete textoVolver;
        for (auto* linea : lineasRanking) {
            if (linea != nullptr) delete linea;
        }
        lineasRanking.clear();
    }

    void cargarDatos() {
        std::ifstream archivo("ranking.txt");
        std::string nombre;
        int victorias;
        float posY = 150.f;

        if (archivo.is_open()) {
            int puesto = 1;
            while (archivo >> nombre >> victorias && puesto <= 10) { // Mostramos el Top 10
                // Creamos el texto pasando la fuente obligatoria
                sf::Text* textoLinea = new sf::Text(fuente);
                textoLinea->setString(std::to_string(puesto) + ". " + nombre + "  -  " + std::to_string(victorias) + " " + (victorias == 1 ? "Victoria" : "Victorias"));
                textoLinea->setCharacterSize(30);
                textoLinea->setFillColor(sf::Color::White);
                textoLinea->setPosition({ 350.f, posY });

                lineasRanking.push_back(textoLinea);
                posY += 50.f;
                puesto++;
            }
            archivo.close();
        }

        if (lineasRanking.empty()) {
            sf::Text* textoVacio = new sf::Text(fuente);
            textoVacio->setString("No hay partidas registradas aun.");
            textoVacio->setCharacterSize(30);
            textoVacio->setFillColor(sf::Color::Cyan);
            textoVacio->setPosition({ 350.f, 250.f });
            lineasRanking.push_back(textoVacio);
        }
    }

    void procesarEntrada(sf::RenderWindow& ventana) {
        while (const std::optional<sf::Event> evento = ventana.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) {
                ventana.close();
            }

            if (const auto* mouseClick = evento->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseClick->button == sf::Mouse::Button::Left) {
                    sf::Vector2i posRaton = sf::Mouse::getPosition(ventana);
                    sf::Vector2f posMapeada = ventana.mapPixelToCoords(posRaton);

                    if (botonVolver.getGlobalBounds().contains(posMapeada)) {
                        transicionMenu = true;
                    }
                }
            }
        }
    }

    void dibujarPantalla(sf::RenderWindow& ventana) {
        if (titulo != nullptr) ventana.draw(*titulo);
        for (auto* linea : lineasRanking) {
            if (linea != nullptr) ventana.draw(*linea);
        }
        ventana.draw(botonVolver);
        if (textoVolver != nullptr) ventana.draw(*textoVolver);
    }

    bool esTransicionLista() const { return transicionMenu; }
};