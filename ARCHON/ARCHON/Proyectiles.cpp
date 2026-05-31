#include "Proyectiles.h" 
#include <cmath> 

// rellenamos todos los atributos en cuanto nace el proyectil
Proyectiles::Proyectiles(float _x, float _y, int _danyo, float _vel, sf::Vector2f _dir, Bando _bando, std::string skin)
    : x(_x), y(_y), danyo(_danyo), velocidad(_vel), direccion(_dir), bandoOrigen(_bando) // lista de inicializacion
{
    sf::Color colorBase; // variable para el centro solido
    sf::Color colorLuz; // variable para el brillo externo

    // temática visual segun qué haya elegido el usuario en el menu
    if (skin == "STAR_WARS") { 
        colorBase = sf::Color::White; // nucleo blanco de laser 
        colorLuz = (bandoOrigen == Bando::LUZ) ? sf::Color(0, 150, 255, 150) : sf::Color(255, 0, 0, 150); // brillo externo azul para bando luz y roja para bando oscuridad
        nucleo.setSize({ 25.f, 4.f }); // forma alargada de rayo
        resplandor.setSize({ 35.f, 12.f }); // el brillo externo es mas ancho
    }
    else if (skin == "HARRY_POTTER") { 
        colorBase = (bandoOrigen == Bando::LUZ) ? sf::Color(255, 100, 100) : sf::Color(100, 255, 100); // colores base rojizos
        colorLuz = (bandoOrigen == Bando::LUZ) ? sf::Color(220, 20, 60, 180) : sf::Color(0, 255, 0, 180); // brillo externo super luminoso
        nucleo.setSize({ 15.f, 6.f }); // forma mas como bolitas
        resplandor.setSize({ 22.f, 16.f }); // brillo externo mas esferico
    }
    else { // archon clásico por defecto
        colorBase = sf::Color::White; // centro magico clasico blanco
        colorLuz = (bandoOrigen == Bando::LUZ) ? sf::Color(255, 215, 0, 150) : sf::Color(138, 43, 226, 150); // brillo externo dorado vs morado
        nucleo.setSize({ 18.f, 5.f }); // tamaño estandar
        resplandor.setSize({ 28.f, 14.f }); // brillo externo mediano
    }

    nucleo.setFillColor(colorBase); // ponemos el color base en el centro del proyectil
    resplandor.setFillColor(colorLuz); // igual para el borde

    // en esta parte cuando el codigo le dice al proyectil que vaya en una direccion especifica
    // el proyectil se ve como un cuerpo rigido y apunta rectilineamente hacia su objetivo
    nucleo.setOrigin({ nucleo.getSize().x / 2.f, nucleo.getSize().y / 2.f }); // se centra el nucleo del proyectil y se divide su tamaño a la mitad
    resplandor.setOrigin({ resplandor.getSize().x / 2.f, resplandor.getSize().y / 2.f }); // igual para brillo externo
    nucleo.setPosition({ x, y }); // nucleo en las coordenadas iniciales del disparo
    resplandor.setPosition({ x, y }); // igual para brillo externo

    // cálculo del ángulo de rotación respecto al vector de desplazamiento
    float anguloRadianes = std::atan2(direccion.y, direccion.x); // se calcula los grados para saber a donde mira el disparo

    
    nucleo.setRotation(sf::radians(anguloRadianes)); // lo inclinamos para que encare la direccion
    resplandor.setRotation(sf::radians(anguloRadianes)); // igual para brillo externo
}

// destructor vacio porque no se usa memoria dinamica
Proyectiles::~Proyectiles() {}


void Proyectiles::mover(float dt) {
    // cálculo de trayectoria del laser con formula del movimiento rectilineo uniformepara que el movimiento no dependa de los fotogramas por segundo del pc
    x += direccion.x * velocidad * dt; // eje horizontal
    y += direccion.y * velocidad * dt; // eje vertical

    nucleo.setPosition({ x, y }); // laser a las nuevas coordenadas calculadas
    resplandor.setPosition({ x, y }); // igual para brillo externo
}


void Proyectiles::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(resplandor); // dibujamos el brillo externo
    ventana.draw(nucleo); // y el nucleo despues 
}