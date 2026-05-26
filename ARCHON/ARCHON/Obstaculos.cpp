#include "Obstaculos.h"
#include <random>
#include <cmath>

Obstaculos::Obstaculos(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    reiniciar(p1, p2);
}

void Obstaculos::reiniciar(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    lista_orbes.clear();
    relojCrecimiento.restart();
    // Instanciación inicial del clúster de obstáculos
    for (int i = 0; i < 3; ++i) {
        agregarParDeOrbes(p1, p2);
    }
}

bool Obstaculos::verificarSolapamiento(sf::Vector2f nuevaPos, float nuevoRadio) {
    for (const auto& orbe : lista_orbes) {
        float dx = nuevaPos.x - orbe.posicion.x;
        float dy = nuevaPos.y - orbe.posicion.y;
        float distancia = std::sqrt(dx * dx + dy * dy);

        // Tolerancia de 5 píxeles para evitar colisiones gráficas entre los rebordes
        if (distancia < (nuevoRadio + orbe.radio + 5.f)) return true;
    }
    return false;
}

void Obstaculos::agregarParDeOrbes(const sf::Vector2f& posAtacante, const sf::Vector2f& posDefensor) {
    std::random_device rd;
    std::mt19937 gen(rd());

    // El eje de simetría de la nueva ventana (1100 px) se sitúa en X = 550.
    // Acotamos la generación del nodo primario a la región izquierda para asegurar el cálculo del espejo.
    std::uniform_real_distribution<float> distX(100.f, 500.f);
    std::uniform_real_distribution<float> distY(100.f, 755.f);

    int intentos = 0;
    while (intentos < 50) { // Sistema de seguridad (Fail-safe) contra bucles infinitos
        float x = distX(gen);
        float y = distY(gen);
        sf::Vector2f nuevaPos(x, y);

        // Comprobación vectorial para no generar muretes directamente sobre los jugadores
        float dAtacante = std::hypot(x - posAtacante.x, y - posAtacante.y);
        float dDefensor = std::hypot(x - posDefensor.x, y - posDefensor.y);

        if (!verificarSolapamiento(nuevaPos, RADIO_BASE) && dAtacante > 60.f && dDefensor > 60.f) {
            Orbe o;
            o.radio = RADIO_BASE;
            o.posicion = nuevaPos;

            // Renderizado del cuerpo base
            o.cuerpo.setRadius(RADIO_BASE);
            o.cuerpo.setOrigin({ RADIO_BASE, RADIO_BASE });
            o.cuerpo.setPosition(nuevaPos);
            o.cuerpo.setFillColor(sf::Color(70, 130, 180, 180));
            o.cuerpo.setOutlineThickness(2.f);
            o.cuerpo.setOutlineColor(sf::Color::White);

            // Renderizado del núcleo de iluminación
            o.nucleo.setRadius(RADIO_BASE * 0.4f);
            o.nucleo.setOrigin({ RADIO_BASE * 0.4f, RADIO_BASE * 0.4f });
            o.nucleo.setPosition(nuevaPos);
            o.nucleo.setFillColor(sf::Color::Cyan);

            // Inserción del nodo secundario aplicando la transformada de reflexión
            Orbe simetrico = o;
            simetrico.posicion.x = 1100.f - x; // Proyección especular corregida

            simetrico.cuerpo.setPosition(simetrico.posicion);
            simetrico.nucleo.setPosition(simetrico.posicion);

            lista_orbes.push_back(o);
            lista_orbes.push_back(simetrico);
            break;
        }
        intentos++;
    }
}

void Obstaculos::actualizar(float dt, const sf::Vector2f& p1, const sf::Vector2f& p2) {
    // Evaluación temporal para el ciclo dinámico de la arena
    if (relojCrecimiento.getElapsedTime().asSeconds() >= 5.0f) {
        if (lista_orbes.size() >= MAX_OBSTACULOS) {
            reiniciar(p1, p2);
        }
        else {
            agregarParDeOrbes(p1, p2);
        }
        relojCrecimiento.restart();
    }
}

bool Obstaculos::hayColisionCircular(const sf::Vector2f& posEntidad, float radioEntidad) const {
    for (const auto& orbe : lista_orbes) {
        float dx = posEntidad.x - orbe.posicion.x;
        float dy = posEntidad.y - orbe.posicion.y;
        float distancia = std::hypot(dx, dy);

        // Ecuación de intersección de circunferencias
        if (distancia < (orbe.radio + radioEntidad)) return true;
    }
    return false;
}

void Obstaculos::expulsarDeColision(sf::Vector2f& posEntidad, float radioEntidad) const {
    for (const auto& orbe : lista_orbes) {
        float dx = posEntidad.x - orbe.posicion.x;
        float dy = posEntidad.y - orbe.posicion.y;
        float distancia = std::hypot(dx, dy);
        float distanciaMinima = orbe.radio + radioEntidad;

        // Si hay penetración, calculamos la fuerza normal de expulsión
        if (distancia < distanciaMinima && distancia > 0.0001f) {
            float solapamiento = distanciaMinima - distancia;
            float nx = dx / distancia; // Vector director normalizado X
            float ny = dy / distancia; // Vector director normalizado Y

            // Reposicionamos la entidad empujándola hacia afuera
            posEntidad.x += nx * solapamiento;
            posEntidad.y += ny * solapamiento;
        }
    }
}

void Obstaculos::dibujar(sf::RenderWindow& ventana) const {
    for (const auto& orbe : lista_orbes) {
        ventana.draw(orbe.cuerpo);
        ventana.draw(orbe.nucleo);
    }
}