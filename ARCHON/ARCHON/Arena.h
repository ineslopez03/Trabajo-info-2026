#pragma once 
#include "InterfazUsuario.h" 
#include "Proyectiles.h" 
#include "Pieza.h"
#include "Obstaculos.h" 
#include "GraficosArena.h" 
#include "ControladorPelea.h" 
#include "MotorFisicasArena.h" 
#include <list> 
#include <SFML/Graphics.hpp> 
#include <string> 

//la clase controla el combate en tiempo real entre dos piezas. hereda de interfazusuario
class Arena : public InterfazUsuario {
private:
    Pieza* piezaIzquierda; //puntero a la pieza del jugador 1 (bando de luz)
    Pieza* piezaDerecha; //puntero a la pieza del jugador 2 (bando de oscuridad)
    std::list<Proyectiles*> lista_proyectiles; //lista que guarda todos los disparos activos cruzando la pantalla
    std::list<EfectoOnda> lista_ondas; //guarda las ondas de choque expansivas de los ataques cuerpo a cuerpo

    sf::Texture texturaFondoArena; //textura de la imagen de fondo del campo de batalla
    sf::Sprite* spriteFondoArena; //sprite para dibujar la textura del fondo
    sf::Vector2f posIzquierda, posDerecha; //vectores con las coordenadas (x,y) actuales de cada pieza
    sf::Clock relojArena, relojCuentaAtras; //relojes para los frames por segundo (dt) y para el "3,2,1" inicial
    std::string skinArena; //guarda la tematica actual para cargar el fondo correcto
    int faseCuentaAtras; //guarda por que numero va la cuenta atras (3, 2, 1, 0)
    bool teclaDisparoIzquierdaLibre = true; //flag para evitar que el jugador 1 dispare como ametralladora si deja pulsado
    bool teclaDisparoDerechaLibre = true; //igual para el jugador 2

    float tiempoRestanteCooldownIzq = 0.f; //cronometro para saber cuando puede volver a atacar la pieza izquierda
    float tiempoRestanteCooldownDer = 0.f; //igual para la pieza derecha

    bool combateFinalizado = false; //flag que se pone a true cuando la vida de alguna pieza llega a 0
    float temporizadorSalida = 2.0f; //tiempo de espera en pantalla desde que alguien muere hasta que volvemos al tablero
    sf::Font fuenteVictoria; //fuente para las letras del mensaje final de ganador
    sf::Text textoVictoria; //texto que dice quien ha ganado el combate al terminar

    Obstaculos obstaculos; //objeto que gestiona la aparicion y colision de los orbes del mapa
    GraficosArena graficos; //objeto que maneja el dibujo de nombres y barras de salud arriba
    Pieza* atacanteOriginal; //puntero a la pieza que ataco primero en el tablero para saber quien invadio a quien

public:
    //crea la arena con las dos piezas, el fondo, la skin y la pieza que origino el combate
    Arena(Pieza* p1, Pieza* p2, const std::string& skin, Pieza* atacante);
    //destructor: limpia la memoria de listas y sprites creados con new
    ~Arena() override;

    //lee el teclado, mueve proyectiles y baja los tiempos de cooldown
    void procesarEntrada(sf::RenderWindow& ventana) override;
    //dibuja todo en orden fondo, obstaculos, piezas, ataques y la interfaz encima
    void dibujarPantalla(sf::RenderWindow& ventana) override;
    //asigna quien va a la izquierda y quien a la derecha y resetea sus coordenadas iniciales
    void iniciarBatalla(Pieza* p1, Pieza* p2);
    //comprueba si un proyectil choca con una pieza para hacer daño o si choca con los orbes para destruirse
    void gestionarColisiones();

    //para consultar quien esta en cada lado
    Pieza* getPiezaIzquierda() { return piezaIzquierda; } //devuelve la pieza que juega por la izquierda
    Pieza* getPiezaDerecha() { return piezaDerecha; } //devuelve la pieza que juega por la derecha
    Pieza* getPiezaAtacanteReal() const { return atacanteOriginal; } //devuelve la pieza que inicio la agresion en el tablero

    //para que el tablero sepa quien ha sobrevivido a la pelea
    Pieza* getGanador() { return (piezaIzquierda->getVidaBase() > 0) ? piezaIzquierda : piezaDerecha; } //si el izq tiene vida mayor a 0 gana, si no gana el der
    Pieza* getPerdedor() { return (piezaIzquierda->getVidaBase() <= 0) ? piezaIzquierda : piezaDerecha; } //si el izq tiene 0 de vida pierde, si no pierde el der

    //flag que le dice al motor si ya acabo la animacion de muerte y esta listo para volver al tablero
    bool isTransicionLista() const { return combateFinalizado && temporizadorSalida <= 0.f; }
};