#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constantes.h"
#include <map>

class Casilla;// Declaración adelantada (Pieza tiene un puntero a Casilla)
class Jugador;// Declaración adelantada (Pieza puede pertenecer a un Jugador)

class Pieza {
    // Clase base abstracta de todas las piezas del juego
// Define los atributos y comportamientos comunes a cualquier pieza
protected:
    
    int vida, vidaMaxima, vidaMaximaBase, danio, velMov, velAta;
    //vida:Puntos de vida actuales
    // vidaMaxima:Vida máxima actual (puede estar aumentada por bono de color)
    //vidaMaximaBase:Vida máxima original de diseño (para restaurar tras el combate)
    // danio:Daño base que inflige en combate
    //velMov: Velocidad de movimiento
    //velAlta:Velocidad de ataque (cuántos golpes da por ronda en la arena)
    Bando bando;// A qué bando pertenece la pieza (LUZ u OSCURIDAD)
    sf::Texture& textura;// Referencia a la textura cargada (se almacena en el caché estático)
    sf::Sprite sprite;// Sprite que usa la textura para dibujarse en pantalla
    Casilla* posicion;// Puntero a la casilla donde está ubicada actualmente
    int rangoMovimiento;// Número máximo de casillas que puede recorrer por turno
    int turnosBloqueado = 0;// Turnos restantes de encarcelamiento (0 = libre)

    bool ataqueCuerpoACuerpo = false;// Reloj para controlar la duración del efecto visual de daño

    sf::Clock relojDanyo;
    bool mostrandoDanyo = false;

public:
    // Constructor: recibe vida, daño, velocidades, textura y bando
    Pieza(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)
        : vida(_v), vidaMaxima(_v), vidaMaximaBase(_v), danio(_d), velMov(_vm), velAta(_va), bando(_b),
        textura(_tex), sprite(_tex), posicion(nullptr), rangoMovimiento(0) {
    }
    // Caché estático de texturas: evita cargar el mismo archivo múltiples veces
    static sf::Texture& obtenerTexturaDesdeRuta(std::string ruta) {
        static std::map<std::string, sf::Texture> diccionario;
        // El mapa es estático: persiste entre llamadas y es compartido por todas las piezas
        if (diccionario.find(ruta) == diccionario.end()) {
            // Si la ruta no está en el caché, carga la textura
            if (!diccionario[ruta].loadFromFile(ruta)) {
                std::cerr << "ERROR: Archivo no encontrado en " << ruta << std::endl;
            }
        }
        return diccionario[ruta];// Devuelve la referencia a la textura (ya cargada o recién cargada)
    }

    virtual ~Pieza() {}// Destructor virtual para la jerarquía polimórfica

    void resetVida() { vida = vidaMaximaBase; vidaMaxima = vidaMaximaBase; }
    // Restaura la vida y el límite máximo al valor de diseño original
    void setVida(int nuevaVida) { this->vida = nuevaVida; }
    // Setter directo de vida (usado por el sistema de combate)
    int getVidaBase() const { return vida; }
    // Getter: vida actual de la pieza
    int getVidaMaxima() const { return vidaMaxima; }
    // Getter: vida máxima actual (con posible bono de color aplicado)
    int getVelAta() const { return velAta; }
    // Getter: velocidad de ataque
    int getDanio() const { return danio; }
    // Getter: daño por golpe
    int getVelMov() const { return velMov; }
    // Getter: velocidad de movimiento
    bool esCuerpoACuerpo() const { return ataqueCuerpoACuerpo; }
    // Getter: ¿es un atacante cuerpo a cuerpo?
    void setAtaqueCuerpoACuerpo(bool estado) { ataqueCuerpoACuerpo = estado; }
    // Setter: cambia el modo de ataque de la pieza
    void recibirDanyo(int cantidad);
    bool estaParpadeandoDanyo();
    void procesarEfectoVisual();
    void aplicarBonoColor(int porcentaje);
    // Aumenta la vida máxima y actual un % sobre la vida base de diseño (bono por color de casilla)
    void restaurarValoresOriginales(int vidaAntesDelCombate);
    // Restaura la vida máxima al valor base y asimila el daño recibido en la arena
    int getVidaMaximaOriginal() const { return vidaMaximaBase; }
    // Getter: vida máxima de diseño original (sin bonos)
    void pasarTurnoBloqueo() { if (turnosBloqueado > 0) turnosBloqueado--; }
    // Reduce en 1 el contador de encarcelamiento al final de cada turno
    void setEncarcelada(int cantidad) { turnosBloqueado = cantidad; }
    // Encarcela la pieza durante 'cantidad' turnos
    bool estaEncarcelada() const { return turnosBloqueado > 0; }
    // Devuelve true si la pieza está actualmente encarcelada (no puede moverse)
    void setPosicion(Casilla* c) { posicion = c; }
    // Actualiza la casilla donde se encuentra la pieza
    Casilla* getPosicion() { return posicion; }
    // Getter: casilla actual de la pieza
    Bando getBando() { return bando; }
    // Getter: bando de la pieza (LUZ u OSCURIDAD)
    virtual void setJugador(Jugador* j) {}
    // Virtual no puro: la mayoría de piezas ignoran esto; las subclases que lo necesiten lo sobreescriben
    virtual bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) = 0;
    // Método puro abstracto: cada tipo de pieza implementa sus propias reglas de movimiento
    virtual void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) = 0;
    // Método puro abstracto: cada tipo dibuja su sprite en el tablero
    virtual void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) = 0;
    // Método puro abstracto: cada tipo dibuja su sprite durante el combate en la arena
    void curar(int cantidad);
    // Restaura 'cantidad' puntos de vida sin superar la vida máxima base
};