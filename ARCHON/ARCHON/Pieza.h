#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constantes.h"
#include <map>

class Casilla;
class Jugador;

class Pieza {
protected:
    int vida, vidaMaxima, danio, velMov, velAta;
    Bando bando;
    sf::Texture& textura;
    sf::Sprite sprite;
    Casilla* posicion;
    int rangoMovimiento;
    int turnosBloqueado = 0;

    // Parámetro de cinemática de ataque
    bool ataqueCuerpoACuerpo = false;

    // Subsistema temporal para el cálculo de daño
    sf::Clock relojDanyo;
    bool mostrandoDanyo = false;

public:
    Pieza(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)
        : vida(_v), vidaMaxima(_v), danio(_d), velMov(_vm), velAta(_va), bando(_b),
        textura(_tex), sprite(_tex), posicion(nullptr), rangoMovimiento(0) {
    }

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

    void resetVida() { vida = vidaMaxima; }
    void setVida(int nuevaVida) { this->vida = nuevaVida; }
    int getVidaBase() const { return vida; }
    int getVidaMaxima() const { return vidaMaxima; }

    int getVelAta() const { return velAta; }
    int getDanio() const { return danio; }
    int getVelMov() const { return velMov; }

    // Métodos de control para la arena
    bool esCuerpoACuerpo() const { return ataqueCuerpoACuerpo; }
    void setAtaqueCuerpoACuerpo(bool estado) { ataqueCuerpoACuerpo = estado; }

    void recibirDanyo(int cantidad) {
        vida -= cantidad;
        if (vida < 0) vida = 0;

        mostrandoDanyo = true;
        relojDanyo.restart();
    }

    // Máquina de estados temporal: Evalúa dos destellos asíncronos en 0.5s
    bool estaParpadeandoDanyo() {
        if (mostrandoDanyo) {
            float t = relojDanyo.getElapsedTime().asSeconds();
            if (t < 0.5f) {
                // Fases activas del parpadeo (ON - OFF - ON - OFF)
                return ((t > 0.0f && t < 0.15f) || (t > 0.25f && t < 0.40f));
            }
            else {
                mostrandoDanyo = false;
            }
        }
        return false;
    }

    // Función de tinte de shader
    void procesarEfectoVisual() {
        if (estaParpadeandoDanyo()) {
            sprite.setColor(sf::Color(255, 30, 30, 220)); // Rojo severo
        }
        else {
            sprite.setColor(sf::Color::White);
        }
    }

    void aplicarBonoColor(int porcentaje);
    void restaurarValoresOriginales(int vidaAntesDelCombate);
    int getVidaMaximaOriginal() const { return vidaMaxima; }

    void pasarTurnoBloqueo() { if (turnosBloqueado > 0) turnosBloqueado--; }
    void setEncarcelada(int cantidad) { turnosBloqueado = cantidad; }
    bool estaEncarcelada() const { return turnosBloqueado > 0; }

    void setPosicion(Casilla* c) { posicion = c; }
    Casilla* getPosicion() { return posicion; }
    Bando getBando() { return bando; }

    virtual void setJugador(Jugador* j) {}
    virtual bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) = 0;
    virtual void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) = 0;
    virtual void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) = 0;
};