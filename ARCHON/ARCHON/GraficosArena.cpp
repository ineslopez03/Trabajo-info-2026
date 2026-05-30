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
    textoCuentaAtras.setCharacterSize(150);
    textoCuentaAtras.setFillColor(sf::Color(255, 215, 0));
    textoCuentaAtras.setOutlineThickness(6.f);
    textoCuentaAtras.setOutlineColor(sf::Color::Black);

    textoEtiquetaIzq.setString("JUGADOR 1");
    textoEtiquetaIzq.setCharacterSize(20);
    textoEtiquetaIzq.setFillColor(sf::Color::White);
    textoEtiquetaIzq.setPosition({ 30.f, 15.f });

    float posX_Derecha = 820.0f;
    sf::Vector2f tamanoBarra(250.f, 25.f);

    textoEtiquetaDer.setString("JUGADOR 2");
    textoEtiquetaDer.setCharacterSize(20);
    textoEtiquetaDer.setFillColor(sf::Color::White);
    textoEtiquetaDer.setPosition({ posX_Derecha + 250.f - textoEtiquetaDer.getLocalBounds().size.x, 15.f });

    marcoIzq.setSize(tamanoBarra); marcoIzq.setFillColor(sf::Color::Transparent);
    marcoIzq.setOutlineThickness(3.f); marcoIzq.setPosition({ 30.f, 45.f });
    barraFondoIzq.setSize(tamanoBarra); barraFondoIzq.setFillColor(sf::Color(80, 20, 20, 200));
    barraFondoIzq.setPosition({ 30.f, 45.f });
    barraVidaIzq.setFillColor(sf::Color(50, 205, 50));
    barraVidaIzq.setPosition({ 30.f, 45.f });

    marcoDer.setSize(tamanoBarra); marcoDer.setFillColor(sf::Color::Transparent);
    marcoDer.setOutlineThickness(3.f);
    marcoDer.setOutlineColor(sf::Color::White);
    marcoDer.setPosition({ posX_Derecha, 45.f });
    barraFondoDer.setSize(tamanoBarra); barraFondoDer.setFillColor(sf::Color(80, 20, 20, 200));
    barraFondoDer.setPosition({ posX_Derecha, 45.f });
    barraVidaDer.setFillColor(sf::Color(50, 205, 50));
    barraVidaDer.setPosition({ 520.f, 45.f });
}

void GraficosArena::vincularPiezas(const Pieza* pIzq, const Pieza* pDer) {
    refPiezaIzq = pIzq;
    refPiezaDer = pDer;
}

void GraficosArena::actualizar(int faseCuenta) {
    // Extracción interna de la telemetría vital
    float ratioIzq = 0.f;
    if (refPiezaIzq) {
        ratioIzq = (float)refPiezaIzq->getVidaBase() / (float)refPiezaIzq->getVidaMaximaOriginal();
    }

    float ratioDer = 0.f;
    if (refPiezaDer) {
        ratioDer = (float)refPiezaDer->getVidaBase() / (float)refPiezaDer->getVidaMaximaOriginal();
    }

    float rIzqClamped = (ratioIzq > 1.0f) ? 1.0f : ratioIzq;
    float rDerClamped = (ratioDer > 1.0f) ? 1.0f : ratioDer;
    if (rIzqClamped < 0.f) rIzqClamped = 0.f;
    if (rDerClamped < 0.f) rDerClamped = 0.f;

    float posX_Derecha = 820.f;
    barraVidaIzq.setSize({ 250.f * rIzqClamped, 25.f });
    float anchoActualDer = 250.f * rDerClamped;
    barraVidaDer.setSize({ anchoActualDer, 25.f });
    barraVidaDer.setPosition({ posX_Derecha + (250.f - anchoActualDer), 45.f });

    if (ratioIzq > 1.0f) barraVidaIzq.setFillColor(sf::Color(0, 191, 255)); // Azul (Sobreescudo)
    else barraVidaIzq.setFillColor(sf::Color(50, 205, 50)); // Verde

    if (ratioDer > 1.0f) barraVidaDer.setFillColor(sf::Color(0, 191, 255));
    else barraVidaDer.setFillColor(sf::Color(50, 205, 50));

    if (faseCuenta > 0) textoCuentaAtras.setString(std::to_string(faseCuenta));
    else if (faseCuenta == 0) textoCuentaAtras.setString("YA");

    sf::FloatRect limites = textoCuentaAtras.getLocalBounds();
    textoCuentaAtras.setOrigin({ limites.size.x / 2.0f, limites.size.y / 2.0f });
    textoCuentaAtras.setPosition({ 550.f, 427.f });
}

void GraficosArena::dibujar(sf::RenderWindow& ventana, bool mostrarCuentaAtras) const {
    ventana.draw(textoEtiquetaIzq);
    ventana.draw(barraFondoIzq);
    ventana.draw(barraVidaIzq);
    ventana.draw(marcoIzq);
    ventana.draw(textoEtiquetaDer);
    ventana.draw(barraFondoDer);
    ventana.draw(barraVidaDer);
    ventana.draw(marcoDer);
    if (mostrarCuentaAtras) ventana.draw(textoCuentaAtras);
}