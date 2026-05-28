#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <optional>
#include "Boton.h"

class PantallaRanking {
private:
    sf::Font fuente;
    sf::Text* titulo;
    std::vector<sf::Text> lineasRanking;
    std::vector<Boton> botones;
    sf::Texture* fondoTextura;
    sf::Sprite* fondoSprite;
    bool transicionMenu;

public:
    PantallaRanking() : transicionMenu(false) {
        if (!fuente.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {
            std::cout << "Error fuente" << std::endl;
        }

        titulo = new sf::Text(fuente, "RANKING", 60);
        titulo->setFillColor(sf::Color::White);
        titulo->setOutlineThickness(4.0f);
        titulo->setOutlineColor(sf::Color::Black);

        fondoTextura = new sf::Texture();
        if (fondoTextura->loadFromFile("../ARCHON/imagenes/Fondo Menu 800x800.png")) {
            fondoSprite = new sf::Sprite(*fondoTextura);
            fondoSprite->setScale({ 1100.0f / fondoTextura->getSize().x, 855.0f / fondoTextura->getSize().y });
        }
        else {
            fondoSprite = new sf::Sprite(*fondoTextura);
        }

        botones.emplace_back(400.0f, 750.0f, 300.0f, 50.0f, "VOLVER", fuente);
        cargarDatos();
    }

    ~PantallaRanking() {
        delete titulo;
        delete fondoSprite;
        delete fondoTextura;
    }

    void cargarDatos() {
        lineasRanking.clear();
        std::ifstream archivo("ranking.txt");
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
                posY += 60.f;
                puesto++;
            }
            archivo.close();
        }
    }

    void procesarEntrada(sf::RenderWindow& ventana) {
        sf::Vector2f mousePos = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
        while (const std::optional<sf::Event> evento = ventana.pollEvent()) {
            if (evento->is<sf::Event::Closed>()) ventana.close();
            if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {
                if (click->button == sf::Mouse::Button::Left) {
                    if (botones[0].botonContieneRaton(mousePos)) transicionMenu = true;
                }
            }
        }
        botones[0].actualizarColorBoton(mousePos);
    }

    void dibujarPantalla(sf::RenderWindow& ventana) {
        ventana.draw(*fondoSprite);
        sf::FloatRect bounds = titulo->getLocalBounds();
        titulo->setPosition({ (1100.0f - bounds.size.x) / 2.0f, 50.0f });
        ventana.draw(*titulo);

        sf::RectangleShape fondoMorado({ 700.0f, 600.0f });
        fondoMorado.setFillColor(sf::Color(35, 15, 55, 230));
        fondoMorado.setOutlineColor(sf::Color(150, 60, 240));
        fondoMorado.setOutlineThickness(3.0f);
        fondoMorado.setPosition({ 200.0f, 130.0f });
        ventana.draw(fondoMorado);

        for (auto& l : lineasRanking) ventana.draw(l);
        for (auto& b : botones) b.dibujar(ventana);
    }

    bool esTransicionLista() const { return transicionMenu; }
};