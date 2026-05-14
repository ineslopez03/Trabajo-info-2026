#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class GraficosArena {
private:
    sf::Font fuenteArena;
    sf::Text textoCuentaAtras;
    sf::Text textoEtiquetaIzq, textoEtiquetaDer;

    sf::RectangleShape marcoIzq, barraFondoIzq, barraVidaIzq;
    sf::RectangleShape marcoDer, barraFondoDer, barraVidaDer;

public:
    GraficosArena();

    void actualizar(float ratioIzq, float ratioDer, int faseCuenta);
    void dibujar(sf::RenderWindow& ventana, bool mostrarCuentaAtras) const;
};