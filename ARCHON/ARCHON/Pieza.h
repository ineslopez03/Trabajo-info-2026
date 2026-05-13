#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constantes.h"
#include <map>

// Declaración adelantada para evitar dependencias circulares
class Casilla;
class Jugador;

/**
 * @class Pieza
 * @brief Clase base abstracta que define el comportamiento común de todas las unidades.
 */
class Pieza {
protected:
    int vida, vidaMaxima, danio, velMov, velAta;
    Bando bando;
    sf::Texture& textura; // Textura original para el tablero
    sf::Sprite sprite;
    Casilla* posicion;
    int rangoMovimiento;
    int turnosBloqueado = 0;

public:
    Pieza(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)
        : vida(_v), vidaMaxima(_v), danio(_d), velMov(_vm), velAta(_va), bando(_b),
        textura(_tex), sprite(_tex), posicion(nullptr), rangoMovimiento(0) {
    }

    /**
     * @brief Gestiona la carga de texturas para evitar duplicados en memoria.
     */
    static sf::Texture& obtenerTexturaDesdeRuta(std::string ruta) {
        static std::map<std::string, sf::Texture> diccionario;
        if (diccionario.find(ruta) == diccionario.end()) {
            if (!diccionario[ruta].loadFromFile(ruta)) {
                std::cerr << "ERROR: Archivo no encontrado en " << ruta << std::endl;
            }
        }
        return diccionario[ruta];
    }

    virtual ~Pieza() {}

    // --- Lógica de Estado ---
    void resetVida() { vida = vidaMaxima; }
    void pasarTurnoBloqueo() { if (turnosBloqueado > 0) turnosBloqueado--; }
    void setEncarcelada(int cantidad) { turnosBloqueado = cantidad; }
    bool estaEncarcelada() const { return turnosBloqueado > 0; }
    float getVidaEfectiva(ColorCasilla colorActual);

    // --- Getters y Setters ---
    void setPosicion(Casilla* c) { posicion = c; }
    Casilla* getPosicion() { return posicion; }
    Bando getBando() { return bando; }
    int getVidaBase() const { return vida; }
    virtual void setJugador(Jugador* j) {}

    // --- Métodos Virtuales Puros (Obligatorios para los hijos) ---

    // Mueve la pieza en la lógica del tablero[cite: 941].
    virtual bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) = 0;

    // Dibuja la pieza en el tablero (vista táctica)[cite: 935].
    virtual void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) = 0;

    /**
     * @brief Nuevo método para el renderizado en la Arena de combate.
     * @param ventana Referencia a la ventana de SFML.
     * @param pos Coordenadas (x, y) en píxeles dentro de la arena.
     * @param mirandoDerecha Booleano para invertir el sprite horizontalmente.
     * @param skin La temática actual seleccionada (ARCHON, HARRY_POTTER, etc.).
     */
    virtual void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) = 0;
};