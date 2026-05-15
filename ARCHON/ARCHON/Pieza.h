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

public:
    Pieza(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)
        : vida(_v), vidaMaxima(_v), danio(_d), velMov(_vm), velAta(_va), bando(_b),
        textura(_tex), sprite(_tex), posicion(nullptr), rangoMovimiento(0) {}

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

    void recibirDanyo(int cantidad) {
        vida -= cantidad;
        if (vida < 0) vida = 0;
    }


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