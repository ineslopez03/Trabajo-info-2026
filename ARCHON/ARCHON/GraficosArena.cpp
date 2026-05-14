#include "GraficosArena.h"
#include <iostream>

GraficosArena::GraficosArena() 

    : textoCuentaAtras(fuenteArena),
    textoEtiquetaIzq(fuenteArena),
    textoEtiquetaDer(fuenteArena) 
{
    if (!fuenteArena.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {
        std::cerr << "Aviso: No se pudo cargar la fuente en GraficosArena." << std::endl;
    }

    textoCuentaAtras.setFont(fuenteArena);
    textoCuentaAtras.setCharacterSize(150);
    textoCuentaAtras.setFillColor(sf::Color(255, 215, 0));
    textoCuentaAtras.setOutlineThickness(6.f);
    textoCuentaAtras.setOutlineColor(sf::Color::Black);

    sf::Vector2f tamanoBarra(250.f, 25.f);

    textoEtiquetaIzq.setFont(fuenteArena);
    textoEtiquetaIzq.setString("JUGADOR 1");
    textoEtiquetaIzq.setCharacterSize(20);
    textoEtiquetaIzq.setFillColor(sf::Color::White);
    textoEtiquetaIzq.setPosition({ 30.f, 15.f });

    textoEtiquetaDer.setFont(fuenteArena);
    textoEtiquetaDer.setString("JUGADOR 2");
    textoEtiquetaDer.setCharacterSize(20);
    textoEtiquetaDer.setFillColor(sf::Color::White);
    textoEtiquetaDer.setPosition({ 770.f - textoEtiquetaDer.getLocalBounds().size.x, 15.f });

    marcoIzq.setSize(tamanoBarra); marcoIzq.setFillColor(sf::Color::Transparent);
    marcoIzq.setOutlineThickness(3.f); marcoIzq.setPosition({ 30.f, 45.f });

    barraFondoIzq.setSize(tamanoBarra); barraFondoIzq.setFillColor(sf::Color(80, 20, 20, 200));
    barraFondoIzq.setPosition({ 30.f, 45.f });

    barraVidaIzq.setFillColor(sf::Color(50, 205, 50));
    barraVidaIzq.setPosition({ 30.f, 45.f });

    marcoDer.setSize(tamanoBarra); marcoDer.setFillColor(sf::Color::Transparent);
    marcoDer.setOutlineThickness(3.f); marcoDer.setPosition({ 520.f, 45.f });

    barraFondoDer.setSize(tamanoBarra); barraFondoDer.setFillColor(sf::Color(80, 20, 20, 200));
    barraFondoDer.setPosition({ 520.f, 45.f });

    barraVidaDer.setFillColor(sf::Color(50, 205, 50));
    barraVidaDer.setPosition({ 520.f, 45.f });
}

void GraficosArena::actualizar(float ratioIzq, float ratioDer, int faseCuenta) {
    barraVidaIzq.setSize({ 250.f * ratioIzq, 25.f });
    barraVidaDer.setSize({ 250.f * ratioDer, 25.f });

    if (faseCuenta > 0) textoCuentaAtras.setString(std::to_string(faseCuenta));
    else if (faseCuenta == 0) textoCuentaAtras.setString("YA");

    sf::FloatRect limites = textoCuentaAtras.getLocalBounds();
    textoCuentaAtras.setOrigin({ limites.size.x / 2.0f, limites.size.y / 2.0f });
    textoCuentaAtras.setPosition({ 400.f, 400.f });
}

void GraficosArena::dibujar(sf::RenderWindow& ventana, bool mostrarCuentaAtras) const {
    ventana.draw(textoEtiquetaIzq); ventana.draw(barraFondoIzq); ventana.draw(barraVidaIzq); ventana.draw(marcoIzq);
    ventana.draw(textoEtiquetaDer); ventana.draw(barraFondoDer); ventana.draw(barraVidaDer); ventana.draw(marcoDer);
    if (mostrarCuentaAtras) ventana.draw(textoCuentaAtras);
}