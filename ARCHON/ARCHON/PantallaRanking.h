#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "InterfazUsuario.h"
#include "Boton.h"

class PantallaRanking : public InterfazUsuario {
private:
    sf::Font fuente;

    // Quitamos los punteros (*) porque es más seguro y limpio gestionarlos directamente
    sf::Text titulo;
    sf::Texture fondoTextura;
    sf::Sprite fondoSprite;

    std::vector<sf::Text> lineasRanking;
    std::vector<Boton> botones;
    bool transicionMenu;

    void cargarDatos();

public:
    PantallaRanking();

    // Ya no hace falta destructor porque al no usar "new", C++ limpia todo automáticamente
    ~PantallaRanking() = default;

    // Métodos obligatorios de InterfazUsuario
    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;

    bool esTransicionLista() const { return transicionMenu; }
};