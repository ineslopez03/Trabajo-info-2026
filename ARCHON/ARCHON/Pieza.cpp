#include"Pieza.h"
#include"Casilla.h"

Pieza::Pieza(int _v, int _d, int _vm, int _va,sf::Texture& textura): vida(_v), danio(_d), velMov(_vm), velAta(_va), sprite(textura) 
{
    posicion = nullptr;
}
void Pieza::setPosicion(Casilla* c) { posicion = c; }
Casilla* Pieza::getPosicion() { return posicion; }
void Pieza::dibujar(sf::RenderWindow& ventana, float x, float y, float tamano) {
    sprite.setPosition(sf::Vector2f(x + tamano / 2.f, y + tamano / 2.f));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.f, bounds.size.y / 2.f));

    ventana.draw(sprite);
}