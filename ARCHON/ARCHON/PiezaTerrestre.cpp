#include"PiezaTerrestre.h"
#include"Casilla.h"


PiezaTerrestre::PiezaTerrestre(int _v, int _d, int _vm, int _va, sf::Texture& _tex, Bando _b)
    : Pieza(_v, _d, _vm, _va, _tex, _b)
{

}
bool PiezaTerrestre::mover(Casilla* origen, Casilla* destino, Casilla* matriz[9][9]) {
	int dx = destino->getX() - origen->getX();
	int dy = destino->getY() - origen->getY();
	int dist = std::abs(dx) + std::abs(dy);
	if (dist > velMov) return false;
    int stepX = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int stepY = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int curX = origen->getX() + stepX;
    int curY = origen->getY() + stepY;

    while (curX != destino->getX() || curY != destino->getY()) {
        if (matriz[curX][curY]->estaOcupada()) return false; 
        curX += stepX;
        curY += stepY;
    }
    return true;
}

void PiezaTerrestre::dibujar(sf::RenderWindow& ventana, float x, float y, float tamano) {
    float escala = (tamano * 0.8f) / textura.getSize().x;
    sprite.setScale({escala, escala});
    float offset = (tamano - (textura.getSize().x * escala)) / 2.0f;
    float posX = x + offset;
    float posY = y + offset;

    sprite.setPosition({ posX, posY });
    ventana.draw(sprite);

}