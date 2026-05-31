#include "Obstaculos.h" 
#include <random>
#include <cmath>

// constructor llama a reiniciar para colocar la primera ronda de orbes
Obstaculos::Obstaculos(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    reiniciar(p1, p2);
}

// limpia la pantalla y pone la estructura inicial de muros
void Obstaculos::reiniciar(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    lista_orbes.clear(); // vacia la lista de la memoria
    relojCrecimiento.restart(); // pone a cero el reloj de aparicion

    
    for (int i = 0; i < 3; ++i) { // bucle para crear 3 paresnal arrancar
        agregarParDeOrbes(p1, p2);
    }
}

// comprueba si el orbe que se va a crear choca con los que ya estan dibujados
bool Obstaculos::verificarSolapamiento(sf::Vector2f nuevaPos, float nuevoRadio) {
    for (const auto& orbe : lista_orbes) { // revisa orbe por orbe
        float dx = nuevaPos.x - orbe.posicion.x; // diferencia x
        float dy = nuevaPos.y - orbe.posicion.y; // diferencia y
        float distancia = std::sqrt(dx * dx + dy * dy); // pitagoras

        // tolerancia de 5 píxeles para evitar colisiones entre los rebordes
        if (distancia < (nuevoRadio + orbe.radio + 5.f)) return true; // si chocan devuelve true
    }
    return false; // si tiene hueco limpio
}

// saca un orbe aleatorio a la izquierda y calcula el espejo a la derecha
void Obstaculos::agregarParDeOrbes(const sf::Vector2f& posAtacante, const sf::Vector2f& posDefensor) {
    std::random_device rd; //semilla de numeros aleatorios 
    std::mt19937 gen(rd()); // arranca el generador

   
    // acota la generación del nodo primario a la región izquierda para asegurar el cálculo del espejo.
    std::uniform_real_distribution<float> distX(100.f, 500.f); // rango de x para el lado izquierdo
    std::uniform_real_distribution<float> distY(100.f, 755.f); // rango de y

    int intentos = 0; // variable para no atascar el bucle
    while (intentos < 50) { 
        float x = distX(gen); // sacamos posicion aleatoria
        float y = distY(gen); // igual
        sf::Vector2f nuevaPos(x, y); // la empaqueta en un vector

        //para no generar muretes directamente sobre los jugadores
        float dAtacante = std::hypot(x - posAtacante.x, y - posAtacante.y); // mide si le cae encima al jugador 1
        float dDefensor = std::hypot(x - posDefensor.x, y - posDefensor.y); // mide si le cae al jugador 2

        // comprueba que haya sitio limpio y no pise a ningun jugador 
        if (!verificarSolapamiento(nuevaPos, RADIO_BASE) && dAtacante > 60.f && dDefensor > 60.f) {
            Orbe o; // prepara el struct del izquierdo
            o.radio = RADIO_BASE; // tamaño
            o.posicion = nuevaPos; // le pasa donde va

            
            o.cuerpo.setRadius(RADIO_BASE); // mete radio
            o.cuerpo.setOrigin({ RADIO_BASE, RADIO_BASE }); 
            o.cuerpo.setPosition(nuevaPos); // coloca
            o.cuerpo.setFillColor(sf::Color(70, 130, 180, 180)); // color azul translucido
            o.cuerpo.setOutlineThickness(2.f); // margen exterior
            o.cuerpo.setOutlineColor(sf::Color::White); // de color blanco

            
            o.nucleo.setRadius(RADIO_BASE * 0.4f); 
            o.nucleo.setOrigin({ RADIO_BASE * 0.4f, RADIO_BASE * 0.4f }); // centrado
            o.nucleo.setPosition(nuevaPos); // misma posicion
            o.nucleo.setFillColor(sf::Color::Cyan); // brilla en azul claro

         
            Orbe simetrico = o; // copia tal cual el orbe que acabamos de hacer
            simetrico.posicion.x = 1100.f - x; 

            simetrico.cuerpo.setPosition(simetrico.posicion); // lo manda a su sitio
            simetrico.nucleo.setPosition(simetrico.posicion); // el brillo tambien

            lista_orbes.push_back(o); // mete el primero al vector
            lista_orbes.push_back(simetrico); // mete su gemelo
            break;
        }
        intentos++; // si no habia hueco suma y prueba otro random
    }
}

// lee cuanto tiempo lleva y si toca mete mas orbes
void Obstaculos::actualizar(float dt, const sf::Vector2f& p1, const sf::Vector2f& p2) {
    
    if (relojCrecimiento.getElapsedTime().asSeconds() >= 5.0f) { // si pasan 5 segundos
        if (lista_orbes.size() >= MAX_OBSTACULOS) { // si ya hay mas del limite maximo
            reiniciar(p1, p2); // resetea todo y vuelven a quedar solo 6
        }
        else {
            agregarParDeOrbes(p1, p2); // si caben, salen 2 nuevos
        }
        relojCrecimiento.restart(); // pone a cero la cuenta de los 5 segundos
    }
}

// mira si el proyectil toca algun orbe
bool Obstaculos::hayColisionCircular(const sf::Vector2f& posEntidad, float radioEntidad) const {
    for (const auto& orbe : lista_orbes) { // revisa cada obstaculo vivo
        float dx = posEntidad.x - orbe.posicion.x; 
        float dy = posEntidad.y - orbe.posicion.y;
        float distancia = std::hypot(dx, dy); // hipotenusa de los catetos

        // ecuación de intersección de circunferencias
        if (distancia < (orbe.radio + radioEntidad)) return true; // si chocan devuelve true
    }
    return false; // si vuela libre, false
}

// si el jugador choca andando contra los orbes, lo empuja hacia afuera
void Obstaculos::expulsarDeColision(sf::Vector2f& posEntidad, float radioEntidad) const {
    for (const auto& orbe : lista_orbes) { // revisa orbe a orbe
        float dx = posEntidad.x - orbe.posicion.x; // vectores
        float dy = posEntidad.y - orbe.posicion.y;
        float distancia = std::hypot(dx, dy); // calculo de distancia actual
        float distanciaMinima = orbe.radio + radioEntidad; // a que distancia deberian estar chocando los bordes

        // si hay penetración, calcula la fuerza normal de expulsión
        if (distancia < distanciaMinima && distancia > 0.0001f) { // si se ha metido dentro del circulo solido
            float solapamiento = distanciaMinima - distancia; // resta para ver cuanto hay que expulsarlo
            float nx = dx / distancia; // vector director normalizado x
            float ny = dy / distancia; // vector director normalizado y

            // empuja hacia afuera
            posEntidad.x += nx * solapamiento; // le quita los pixeles metidos en el eje x
            posEntidad.y += ny * solapamiento; // igual para y
        }
    }
}

// dibuja los structs en la pantalla
void Obstaculos::dibujar(sf::RenderWindow& ventana) const {
    for (const auto& orbe : lista_orbes) { // recorre la lista
        ventana.draw(orbe.cuerpo); // primero dibuja la base
        ventana.draw(orbe.nucleo); // y despues la luz
    }
}