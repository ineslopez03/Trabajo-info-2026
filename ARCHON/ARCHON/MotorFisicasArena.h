#pragma once 
#include <SFML/Graphics.hpp> 
#include <list> 
#include "Pieza.h" 
#include "Obstaculos.h" 

// área de efecto cuerpo a cuerpo
struct EfectoOnda {
    sf::CircleShape forma; // un circulo de onda
    float radio; // como de grande es
    float opacidad; // para que se vaya haciendo transparente al crecer
    sf::Color colorBando; // cian o rojo segun quien pegue
};

//calculos matematicos fuera de la arena
class MotorFisicasArena {
public:
    // mueve la pieza, choca con los limites de la pantalla y con los obstaculos del centro
    static void aplicarFisicaMovimiento(Pieza* pieza, sf::Vector2f& pos, sf::Vector2f dir, sf::Vector2f posEnemigo, float dt, const Obstaculos& obstaculos);

    //autoapuntado para saber si tirar la bola arriba, abajo, izq o der
    static sf::Vector2f calcularDireccionOrtogonal(sf::Vector2f posAtacante, sf::Vector2f posEnemigo);

    //hace que el circulito de cuerpo a cuerpo crezca
    static void propagarOndas(std::list<EfectoOnda>& listaOndas, float dt);

    // crea la onda nueva cuando alguien le da al boton de pegar cuerpo a cuerpo
    static void generarOndaChoque(sf::Vector2f pos, Bando b, std::list<EfectoOnda>& listaOndas);
};