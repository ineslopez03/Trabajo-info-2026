#pragma once
#include<SFML/Graphics.hpp>
enum class Bando{LUZ, OSCURIDAD};
class Casilla;
class Pieza {
protected:
    int vida;
    int danio;
    int velMov;
    int velAta;
    Casilla* posicion; 
    sf::Texture textura;
	sf::Sprite sprite;

public:
    Pieza(int _v, int _d, int _vm, int _va,sf::Texture& textura);
    virtual ~Pieza() {}
    virtual void mover() = 0;
    virtual void atacar() = 0;
  virtual void dibujar(sf::RenderWindow& ventana, float x, float y, float tamano);
	virtual Bando getBando() const = 0;
	int getVelmov() const { return velMov; }
    void setPosicion(Casilla* c);
    Casilla* getPosicion();

};