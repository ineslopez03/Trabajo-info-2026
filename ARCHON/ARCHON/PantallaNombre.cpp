#include "PantallaNombre.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

struct RegistroRanking {
    std::string nombre;
    int victorias;
};


PantallaNombre::PantallaNombre(Bando ganador)
    : textoTitulo(fuente),
    textoInstrucciones(fuente),
    textoNombre(fuente),
    textoBotonEnter(fuente)
{
    bandoGanador = ganador;
    terminado = false;
    nombreIngresado = "";

   
    if (!fuente.openFromFile("fuentes/Rush Zone.otf")) {
        if (!fuente.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Error cargando fuente en PantallaNombre" << std::endl;
        }
    }


    if (bandoGanador == Bando::LUZ) {
        textoTitulo.setString("VICTORIA DEL BANDO DE LA LUZ");
        textoTitulo.setFillColor(sf::Color::Cyan);
    }
    else {
        textoTitulo.setString("VICTORIA DEL BANDO DE LA OSCURIDAD");
        textoTitulo.setFillColor(sf::Color::Red);
    }
    textoTitulo.setCharacterSize(40);
    textoTitulo.setPosition(sf::Vector2f(150.f, 150.f));

   
    textoInstrucciones.setString("Introduce tu nombre para el Ranking:");
    textoInstrucciones.setFillColor(sf::Color::White);
    textoInstrucciones.setCharacterSize(25);
    textoInstrucciones.setPosition(sf::Vector2f(150.f, 250.f));

    textoNombre.setString("_");
    textoNombre.setFillColor(sf::Color::Yellow);
    textoNombre.setCharacterSize(35);
    textoNombre.setPosition(sf::Vector2f(150.f, 320.f));

    textoBotonEnter.setString("[ Presiona ENTER para guardar y continuar ]");
    textoBotonEnter.setFillColor(sf::Color(150, 150, 150));
    textoBotonEnter.setCharacterSize(20);
    textoBotonEnter.setPosition(sf::Vector2f(150.f, 450.f));
}

void PantallaNombre::procesarEntrada(sf::RenderWindow& ventana) {
    while (const auto evento = ventana.pollEvent()) {
        if (evento->is<sf::Event::Closed>()) {
            ventana.close();
        }

        if (const auto* textEntered = evento->getIf<sf::Event::TextEntered>()) {
            
            if (textEntered->unicode == 8) {
                if (!nombreIngresado.empty()) {
                    nombreIngresado.pop_back();
                }
            }
            
            else if (textEntered->unicode == 13) {
                if (!nombreIngresado.empty()) {
                    guardarEnRanking();
                    terminado = true;
                }
            }
          
            else if (textEntered->unicode >= 32 && textEntered->unicode < 127) {
                if (textEntered->unicode != ' ' && nombreIngresado.size() < 12) {
                    nombreIngresado += static_cast<char>(textEntered->unicode);
                }
            }

            if (nombreIngresado.empty()) {
                textoNombre.setString("_");
            }
            else {
                textoNombre.setString(nombreIngresado + "_");
            }
        }
    }
}

void PantallaNombre::guardarEnRanking() {
    std::vector<RegistroRanking> listaRanking;
    bool encontrado = false;

    std::ifstream archivoLectura("ranking.txt");
    if (archivoLectura.is_open()) {
        RegistroRanking reg;
        while (archivoLectura >> reg.nombre >> reg.victorias) {
            if (reg.nombre == nombreIngresado) {
                reg.victorias += 1;
                encontrado = true;
            }
            listaRanking.push_back(reg);
        }
        archivoLectura.close();
    }

    if (!encontrado) {
        RegistroRanking nuevoReg;
        nuevoReg.nombre = nombreIngresado;
        nuevoReg.victorias = 1;
        listaRanking.push_back(nuevoReg);
    }

    std::sort(listaRanking.begin(), listaRanking.end(), [](const RegistroRanking& a, const RegistroRanking& b) {
        return a.victorias > b.victorias;
        });

    std::ofstream archivoEscritura("ranking.txt");
    if (archivoEscritura.is_open()) {
        for (const auto& jugador : listaRanking) {
            archivoEscritura << jugador.nombre << " " << jugador.victorias << "\n";
        }
        archivoEscritura.close();
    }
}

void PantallaNombre::dibujar(sf::RenderWindow& ventana) {
    ventana.clear(sf::Color(20, 10, 30));

    ventana.draw(textoTitulo);
    ventana.draw(textoInstrucciones);
    ventana.draw(textoNombre);
    if (!nombreIngresado.empty()) {
        ventana.draw(textoBotonEnter);
    }
}