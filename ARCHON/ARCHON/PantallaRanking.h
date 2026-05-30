#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "InterfazUsuario.h"
#include "Boton.h"

class MotorArchon;

class PantallaRanking : public InterfazUsuario {
private:
    sf::Font fuente;//para configurar los textos
    sf::Text titulo;
    sf::Texture fondoTextura;//para el fondo
    sf::Sprite fondoSprite;
    std::vector<sf::Text> lineasRanking;//vector de textos para mostrar las lineas del ranking
    //se usa para almacenar cada linea del ranking leida del archivo para luego dibujarlas en pantalla
    std::vector<Boton> botones;//vector para botones(solo el de volver por ahora)

    void cargarDatos();//para cargar los datos del ranking desde el archivo
public:
    PantallaRanking();//constructor
    ~PantallaRanking() = default;//destructor por defecto
    void procesarEntrada(sf::RenderWindow& ventana, MotorArchon* motor) override;//metodos de interfaz usuario
    void dibujarPantalla(sf::RenderWindow& ventana) override;
};