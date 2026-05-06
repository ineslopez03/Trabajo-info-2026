#pragma once
#include <SFML/Graphics.hpp>

// Forward declarations (Declaraciones adelantadas) para evitar inclusiones circulares
enum class Bando { LUZ, OSCURIDAD };
class Casilla;
class Jugador;

class Pieza {
protected:
    int vida;
    int danio;
    int velMov;
    int velAta;

    // Relaciones de agregación según el esquema
    Casilla* posicion;
    Jugador* duenio; // Relación con el Jugador necesaria para Jugador.cpp 

    sf::Texture textura;
    sf::Sprite sprite;
    Bando bando;

public:
    Pieza(int _v, int _d, int _vm, int _va, sf::Texture& textura, Bando _b);
    virtual ~Pieza() {}

    // Métodos virtuales puros (Clase Abstracta)
    virtual void mover() = 0;
    virtual void atacar() = 0;

    // Lógica de representación y combate
    virtual void dibujar(sf::RenderWindow& ventana, float x, float y, float tamano);

    virtual sf::FloatRect getBounds(float x, float y, float tamano) {
        return sf::FloatRect({ x, y }, { tamano, tamano });
    }

    void recibirDanyo(int cantidad) {
        vida -= cantidad;
        if (vida < 0) vida = 0;
    }

    // Getters y Setters
    virtual Bando getBando() const { return bando; }
    int getVelmov() const { return velMov; }

    void setPosicion(Casilla* c);
    Casilla* getPosicion();

   
    void setJugador(Jugador* j) { duenio = j; }
};