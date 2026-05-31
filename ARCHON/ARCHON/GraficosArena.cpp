#include "GraficosArena.h" 
#include <iostream> 

// constructor que inicializa los textos
GraficosArena::GraficosArena()
    : textoCuentaAtras(fuenteArena),
    textoEtiquetaIzq(fuenteArena),
    textoEtiquetaDer(fuenteArena)
{
    if (!fuenteArena.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) { //carga la fuente
        std::cerr << "Aviso: No se pudo cargar la fuente en GraficosArena." << std::endl;
    }

    textoCuentaAtras.setCharacterSize(150); // texto para la cuenta atras
    textoCuentaAtras.setFillColor(sf::Color(255, 215, 0)); // color dorado
    textoCuentaAtras.setOutlineThickness(6.f); // borde grueso 
    textoCuentaAtras.setOutlineColor(sf::Color::Black); // borde color negro

    textoEtiquetaIzq.setString("JUGADOR 1"); // prepara el titulo izquierdo
    textoEtiquetaIzq.setCharacterSize(20); // tamaño normal
    textoEtiquetaIzq.setFillColor(sf::Color::White); // letras blancas
    textoEtiquetaIzq.setPosition({ 30.f, 15.f }); // colocado arriba a la izquierda

    float posX_Derecha = 820.0f; // guarda donde empieza la zona derecha para cuadrar todo
    sf::Vector2f tamanoBarra(250.f, 25.f); // tamaño fijo de las barras de vida para no repetir numeros

    textoEtiquetaDer.setString("JUGADOR 2"); // prepara el titulo derecha
    textoEtiquetaDer.setCharacterSize(20); // tamaño normal
    textoEtiquetaDer.setFillColor(sf::Color::White); // letras blancas
    textoEtiquetaDer.setPosition({ posX_Derecha + 250.f - textoEtiquetaDer.getLocalBounds().size.x, 15.f }); // la alinea a la derecha del todo

    marcoIzq.setSize(tamanoBarra); marcoIzq.setFillColor(sf::Color::Transparent); // el marco es vacio por dentro
    marcoIzq.setOutlineThickness(3.f); marcoIzq.setPosition({ 30.f, 45.f }); // le pone borde y lo posiciona
    barraFondoIzq.setSize(tamanoBarra); barraFondoIzq.setFillColor(sf::Color(80, 20, 20, 200)); // el fondo que se ve al perder vida es rojo oscuro
    barraFondoIzq.setPosition({ 30.f, 45.f }); // en la misma posicion que el marco
    barraVidaIzq.setFillColor(sf::Color(50, 205, 50)); // barra de vida color verde
    barraVidaIzq.setPosition({ 30.f, 45.f }); // misma posicion

    marcoDer.setSize(tamanoBarra); marcoDer.setFillColor(sf::Color::Transparent); // marco derecho
    marcoDer.setOutlineThickness(3.f); // borde grueso
    marcoDer.setOutlineColor(sf::Color::White); // color de borde blanco
    marcoDer.setPosition({ posX_Derecha, 45.f }); // ubicado usando la variable
    barraFondoDer.setSize(tamanoBarra); barraFondoDer.setFillColor(sf::Color(80, 20, 20, 200)); // fondo rojo
    barraFondoDer.setPosition({ posX_Derecha, 45.f }); // misma posicion
    barraVidaDer.setFillColor(sf::Color(50, 205, 50)); // barra verde derecha
    barraVidaDer.setPosition({ 520.f, 45.f }); 
}

// guarda los punteros de las piezas que estan combatiendo
void GraficosArena::vincularPiezas(const Pieza* pIzq, const Pieza* pDer) {
    refPiezaIzq = pIzq; // guarda la ref 1
    refPiezaDer = pDer; // guarda la ref 2
}

// encoge las barras segun el daño
void GraficosArena::actualizar(int faseCuenta) {
    
    float ratioIzq = 0.f; // ratio de 0 a 1 para la vida izq
    if (refPiezaIzq) { // si el puntero existe
        ratioIzq = (float)refPiezaIzq->getVidaBase() / (float)refPiezaIzq->getVidaMaximaOriginal(); // divide vida actual entre la maxima original para sacar el porcentaje
    }

    float ratioDer = 0.f; // ratio para la pieza derecha
    if (refPiezaDer) { // si no es nula
        ratioDer = (float)refPiezaDer->getVidaBase() / (float)refPiezaDer->getVidaMaximaOriginal(); // calcula su porcentaje vital
    }

    float rIzqClamped = (ratioIzq > 1.0f) ? 1.0f : ratioIzq; // si tiene bono y su ratio pasa de 1, se pone el tope a 1 para el tamaño
    float rDerClamped = (ratioDer > 1.0f) ? 1.0f : ratioDer; // lo mismo para el derecho
    if (rIzqClamped < 0.f) rIzqClamped = 0.f; // no deja que baje de cero
    if (rDerClamped < 0.f) rDerClamped = 0.f; // igual para el derecho

    float posX_Derecha = 820.f; // constante de la posicion derecha
    barraVidaIzq.setSize({ 250.f * rIzqClamped, 25.f }); // la barra izq encoge multiplicando su ancho por el porcentaje
    float anchoActualDer = 250.f * rDerClamped; // calcula el ancho de la barra derecha
    barraVidaDer.setSize({ anchoActualDer, 25.f }); // le aplica el tamaño
    barraVidaDer.setPosition({ posX_Derecha + (250.f - anchoActualDer), 45.f }); // desplaza la barra derecha para que parezca que se vacia hacia la derecha

    if (ratioIzq > 1.0f) barraVidaIzq.setFillColor(sf::Color(0, 191, 255)); // azul (sobreescudo) si tiene bono de vida
    else barraVidaIzq.setFillColor(sf::Color(50, 205, 50)); // verde si esta normal

    if (ratioDer > 1.0f) barraVidaDer.setFillColor(sf::Color(0, 191, 255)); // azul para el derecho si tiene mas vida que su base
    else barraVidaDer.setFillColor(sf::Color(50, 205, 50)); // verde normal

    if (faseCuenta > 0) textoCuentaAtras.setString(std::to_string(faseCuenta)); // pasamos el int de la cuenta a string
    else if (faseCuenta == 0) textoCuentaAtras.setString("YA"); // cuando llega a 0 ponemos ya

    sf::FloatRect limites = textoCuentaAtras.getLocalBounds(); // mide el tamaño del texto 
    textoCuentaAtras.setOrigin({ limites.size.x / 2.0f, limites.size.y / 2.0f }); 
    textoCuentaAtras.setPosition({ 550.f, 427.f }); 
}


void GraficosArena::dibujar(sf::RenderWindow& ventana, bool mostrarCuentaAtras) const {
    ventana.draw(textoEtiquetaIzq); // pinta nombre izq
    ventana.draw(barraFondoIzq); // pinta fondo vida izq
    ventana.draw(barraVidaIzq); // pinta barra vida izq
    ventana.draw(marcoIzq); // pinta marco izq
    ventana.draw(textoEtiquetaDer); // y asi con todo lo derecho...
    ventana.draw(barraFondoDer);
    ventana.draw(barraVidaDer);
    ventana.draw(marcoDer);
    if (mostrarCuentaAtras) ventana.draw(textoCuentaAtras); // y solo pinta el 3,2,1 si la arena le dice que lo pinte
}