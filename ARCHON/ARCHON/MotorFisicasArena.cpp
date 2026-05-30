#include "MotorFisicasArena.h"
#include "PiezaVoladora.h"
#include <cmath>
#include <algorithm>

void MotorFisicasArena::aplicarFisicaMovimiento(Pieza* pieza, sf::Vector2f& pos, sf::Vector2f dir, sf::Vector2f posEnemigo, float dt, const Obstaculos& obstaculos) {
    float velocidadPx = pieza->getVelMov() * 80.f;
    const float margen = 30.f;

    pos.x += dir.x * velocidadPx * dt;
    pos.y += dir.y * velocidadPx * dt;

    pos.x = std::clamp(pos.x, margen, 1100.f - margen);
    pos.y = std::clamp(pos.y, margen, 855.f - margen);

    bool esVoladora = (dynamic_cast<PiezaVoladora*>(pieza) != nullptr);
    if (!esVoladora) {
        obstaculos.expulsarDeColision(pos, 20.f);
    }

    float radioHitboxCuerpo = 30.f;
    float dx = pos.x - posEnemigo.x;
    float dy = pos.y - posEnemigo.y;
    float distancia = std::hypot(dx, dy);
    float radioCombinado = radioHitboxCuerpo * 2.f;

    if (distancia < radioCombinado && distancia > 0.0001f) {
        float solapamiento = radioCombinado - distancia;
        pos.x += (dx / distancia) * solapamiento;
        pos.y += (dy / distancia) * solapamiento;
    }
}

void MotorFisicasArena::gestionarColisiones(Pieza* piezaIzq, Pieza* piezaDer, std::list<Proyectiles*>& listaProyectiles, const Obstaculos& obstaculos) {
    // Extraído de la Arena para limpieza estructural. La arena nos delega las iteraciones destructivas.
}

sf::Vector2f MotorFisicasArena::calcularDireccionOrtogonal(sf::Vector2f posAtacante, sf::Vector2f posEnemigo) {
    float dx = posEnemigo.x - posAtacante.x;
    float dy = posEnemigo.y - posAtacante.y;
    if (std::abs(dx) > std::abs(dy)) {
        return (dx > 0) ? sf::Vector2f(1.f, 0.f) : sf::Vector2f(-1.f, 0.f);
    }
    else {
        return (dy > 0) ? sf::Vector2f(0.f, 1.f) : sf::Vector2f(0.f, -1.f);
    }
}

void MotorFisicasArena::propagarOndas(std::list<EfectoOnda>& listaOndas, float dt) {
    for (auto it = listaOndas.begin(); it != listaOndas.end();) {
        it->radio += 300.f * dt;
        it->opacidad -= 600.f * dt;
        if (it->opacidad <= 0.f || it->radio >= 85.f) {
            it = listaOndas.erase(it);
        }
        else {
            it->forma.setRadius(it->radio);
            it->forma.setOrigin({ it->radio, it->radio });
            it->forma.setOutlineColor(sf::Color(it->colorBando.r, it->colorBando.g, it->colorBando.b, std::max(0, (int)it->opacidad)));
            ++it;
        }
    }
}

void MotorFisicasArena::generarOndaChoque(sf::Vector2f pos, Bando b, std::list<EfectoOnda>& listaOndas) {
    EfectoOnda onda;
    onda.radio = 20.f;
    onda.opacidad = 255.f;
    onda.forma.setFillColor(sf::Color::Transparent);
    onda.forma.setOutlineThickness(4.f);
    onda.colorBando = (b == Bando::LUZ) ? sf::Color::Cyan : sf::Color::Red;
    onda.forma.setPosition(pos);
    listaOndas.push_back(onda);
}