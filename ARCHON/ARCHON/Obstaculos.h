#pragma once 
#include <SFML/Graphics.hpp> 
#include <vector> 

//gestiona los orbes que salen en medio de la arena
class Obstaculos {
private:
    //como se compone visualmente un orbe
    struct Orbe {
        sf::CircleShape cuerpo; //base semitransparente 
        sf::CircleShape nucleo; //centro brillante 
        float radio; // tamaño del orbe para las colisiones
        sf::Vector2f posicion; // coordenadas x e y del mapa
    };

    std::vector<Orbe> lista_orbes; // vector donde guarda todos los orbes que estan en pantalla
    sf::Clock relojCrecimiento; // temporizador para que nazcan orbes nuevos cada x segundos

   
    const int MAX_OBSTACULOS = 34; // tope maximo para no saturar el mapa de orbes
    const float RADIO_BASE = 18.f; // tamaño por defecto de los orbes

   
    void agregarParDeOrbes(const sf::Vector2f& posAtacante, const sf::Vector2f& posDefensor); // crea un orbe y su clon espejo
    bool verificarSolapamiento(sf::Vector2f nuevaPos, float nuevoRadio); // comprueba si un orbe va a salir encima de uno viejo

public:
    //recibe donde estan los jugadores para no ponerles un orbe encima
    Obstaculos(const sf::Vector2f& p1, const sf::Vector2f& p2);

    //bucle para que vayan creciendo los obstaculos
    void actualizar(float dt, const sf::Vector2f& p1, const sf::Vector2f& p2);

    // dibuja todos los orbes del vector
    void dibujar(sf::RenderWindow& ventana) const;

    // borra los orbes viejos y crea los primeros 3 pares al empezar un combate
    void reiniciar(const sf::Vector2f& p1, const sf::Vector2f& p2);

    // mira si un laser ha tocado el muro
    bool hayColisionCircular(const sf::Vector2f& posEntidad, float radioEntidad) const;

    //empuja a los personajes si chocan en arena
    void expulsarDeColision(sf::Vector2f& posEntidad, float radioEntidad) const;
};