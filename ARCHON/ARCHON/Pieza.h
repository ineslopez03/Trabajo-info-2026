#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constantes.h"
#include <map>

class Casilla;//declaracion adelantada para evitar dependencias circulares

class Pieza {//clase base abstracta de todas las piezas del juego
protected:
    int vida, vidaMaxima, vidaMaximaBase, danio, velMov, velAta;//atributos numericos de combate
    Bando bando;//bando al que pertenece la pieza
    sf::Texture& textura;//referencia a la textura cargada en memoria
    sf::Sprite sprite;//sprite que usa la textura para dibujarse
    Casilla* posicion;//puntero a la casilla donde esta ubicada
    int rangoMovimiento;//numero maximo de casillas por turno
    int turnosBloqueado = 0;//turnos restantes de encarcelamiento
    bool ataqueCuerpoACuerpo = false;//flag para el tipo de ataque en la arena
    sf::Clock relojDanyo;//reloj para controlar el efecto visual de recibir daño
    bool mostrandoDanyo = false;//flag para saber si esta parpadeando en rojo

public:
    Pieza(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)//constructor generico
        : vida(_v), vidaMaxima(_v), vidaMaximaBase(_v), danio(_d), velMov(_vm), velAta(_va), bando(_b),
        textura(_tex), sprite(_tex), posicion(nullptr), rangoMovimiento(0) {
    }

    static sf::Texture& obtenerTexturaDesdeRuta(std::string ruta) {//cache estatico de texturas
        static std::map<std::string, sf::Texture> diccionario;//mapa compartido por todas las piezas
        if (diccionario.find(ruta) == diccionario.end()) {//si la ruta no esta en el cache
            if (!diccionario[ruta].loadFromFile(ruta)) {//carga la textura desde el archivo
                std::cerr << "ERROR: Archivo no encontrado en " << ruta << std::endl;
            }
        }
        return diccionario[ruta];//devuelve la referencia
    }

    virtual ~Pieza() {}//destructor virtual para la jerarquia polimorfica

    void resetVida() { vida = vidaMaximaBase; vidaMaxima = vidaMaximaBase; }//restaura vida al maximo original
    void setVida(int nuevaVida) { this->vida = nuevaVida; }//setter directo de vida
    int getVidaBase() const { return vida; }//getter de vida actual
    int getVidaMaxima() const { return vidaMaxima; }//getter de vida maxima
    int getVelAta() const { return velAta; }//getter de velocidad de ataque
    int getDanio() const { return danio; }//getter de daño por golpe
    int getVelMov() const { return velMov; }//getter de velocidad de movimiento
    bool esCuerpoACuerpo() const { return ataqueCuerpoACuerpo; }//getter de tipo de ataque
    void setAtaqueCuerpoACuerpo(bool estado) { ataqueCuerpoACuerpo = estado; }//setter de tipo de ataque
    
    void recibirDanyo(int cantidad);//resta vida y activa parpadeo
    bool estaParpadeandoDanyo();//devuelve true si debe verse roja
    void procesarEfectoVisual();//aplica el color correcto al sprite
    void aplicarBonoColor(int porcentaje);//aumenta la vida maxima por posicion
    void restaurarValoresOriginales(int vidaAntesDelCombate);//elimina el bono de color
    int getVidaMaximaOriginal() const { return vidaMaximaBase; }//getter de vida de diseño
    void pasarTurnoBloqueo() { if (turnosBloqueado > 0) turnosBloqueado--; }//descuenta turnos de encarcelamiento
    void setEncarcelada(int cantidad) { turnosBloqueado = cantidad; }//aplica el hechizo imprison
    bool estaEncarcelada() const { return turnosBloqueado > 0; }//devuelve true si esta bloqueada
    void setPosicion(Casilla* c) { posicion = c; }//actualiza la casilla actual
    Casilla* getPosicion() { return posicion; }//getter de la casilla
    Bando getBando() { return bando; }//getter del bando

    virtual bool mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) = 0;//metodo puro para movimiento logico
    virtual void dibujar(sf::RenderWindow& ventana, Casilla* seleccionada, int turno, float tamano) = 0;//metodo puro para dibujo en tablero
    virtual void dibujarEnArena(sf::RenderWindow& ventana, sf::Vector2f pos, bool mirandoDerecha, std::string skin) = 0;//metodo puro para dibujo en arena
    void curar(int cantidad);//suma vida sin superar el limite
};