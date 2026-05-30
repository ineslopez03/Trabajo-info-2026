#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Pieza.h" // Requerido para conocer la estructura de los punteros

class GraficosArena {
private:
    sf::Font fuenteArena;
    sf::Text textoCuentaAtras;
    sf::Text textoEtiquetaIzq, textoEtiquetaDer;
    sf::RectangleShape marcoIzq, barraFondoIzq, barraVidaIzq;
    sf::RectangleShape marcoDer, barraFondoDer, barraVidaDer;

    // Referencias constantes inyectadas para lectura de estado
    const Pieza* refPiezaIzq = nullptr;
    const Pieza* refPiezaDer = nullptr;

public:
    GraficosArena();

    // Método de inyección de dependencias
    void vincularPiezas(const Pieza* pIzq, const Pieza* pDer);

    // La actualización ya no requiere recibir los ratios por parámetro
    void actualizar(int faseCuenta);
    void dibujar(sf::RenderWindow& ventana, bool mostrarCuentaAtras) const;
};