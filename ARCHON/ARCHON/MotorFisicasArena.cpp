#include "MotorFisicasArena.h" 
#include "PiezaVoladora.h" 
#include <cmath> 
#include <algorithm> 

// mueve a los personajes respetando el suelo, las paredes y chocar con el otro
void MotorFisicasArena::aplicarFisicaMovimiento(Pieza* pieza, sf::Vector2f& pos, sf::Vector2f dir, sf::Vector2f posEnemigo, float dt, const Obstaculos& obstaculos) {
    float velocidadPx = pieza->getVelMov() * 80.f; // saca su velocidad base y la escala a pixeles reales
    const float margen = 30.f;

    pos.x += dir.x * velocidadPx * dt; // le aplica el movimiento en el eje x por el delta time
    pos.y += dir.y * velocidadPx * dt; // aplica el movimiento en el eje y

    pos.x = std::clamp(pos.x, margen, 1100.f - margen); // fuerza la x para no salirse de la pantalla
    pos.y = std::clamp(pos.y, margen, 855.f - margen); // fuerza la y para no salirse de la pantalla

    bool esVoladora = (dynamic_cast<PiezaVoladora*>(pieza) != nullptr); // mira si es una pieza voladora con cast
    if (!esVoladora) { // si va a pie
        obstaculos.expulsarDeColision(pos, 20.f); // los obstaculos la rebotan
    }

    float radioHitboxCuerpo = 30.f; // radio para chocar los cuerpos de las piezas entre si
    float dx = pos.x - posEnemigo.x; // diferencia x entre las piezas
    float dy = pos.y - posEnemigo.y; // diferencia y
    float distancia = std::hypot(dx, dy); // pitagoras para la distancia entre ellos
    float radioCombinado = radioHitboxCuerpo * 2.f; // distancia a la que se tocan los bordes

    if (distancia < radioCombinado && distancia > 0.0001f) { // si estan solapandose y no se divide por cero
        float solapamiento = radioCombinado - distancia; // cuanto se han pisado
        pos.x += (dx / distancia) * solapamiento; // las empuja hacia afuera en x
        pos.y += (dy / distancia) * solapamiento; // y las empuja en y para que resbalen
    }
}

//tira laseres solo en forma de cruz
sf::Vector2f MotorFisicasArena::calcularDireccionOrtogonal(sf::Vector2f posAtacante, sf::Vector2f posEnemigo) {
    float dx = posEnemigo.x - posAtacante.x; // saca el delta x
    float dy = posEnemigo.y - posAtacante.y; // saca delta y
    if (std::abs(dx) > std::abs(dy)) { // si el enemigo esta mas lejos en horizontal que en vertical
        return (dx > 0) ? sf::Vector2f(1.f, 0.f) : sf::Vector2f(-1.f, 0.f); // le tira la bola a la derecha o a la izq
    }
    else { // si esta mas lejos a lo alto/bajo
        return (dy > 0) ? sf::Vector2f(0.f, 1.f) : sf::Vector2f(0.f, -1.f); // tira la bola pabajo o parriba
    }
}


void MotorFisicasArena::propagarOndas(std::list<EfectoOnda>& listaOndas, float dt) {
    for (auto it = listaOndas.begin(); it != listaOndas.end();) { // itera la lista de ondas
        it->radio += 300.f * dt; // hace que el radio crezca a 300 pixeles por segundo
        it->opacidad -= 600.f * dt; // hace que se vuelva invisible super rapido
        if (it->opacidad <= 0.f || it->radio >= 85.f) { // si ya no se ve o es muy amplia
            it = listaOndas.erase(it); // la borra de la existencia
        }
        else { // si todavia vive
            it->forma.setRadius(it->radio); // actualiza su tamaño visual
            it->forma.setOrigin({ it->radio, it->radio }); 
            it->forma.setOutlineColor(sf::Color(it->colorBando.r, it->colorBando.g, it->colorBando.b, std::max(0, (int)it->opacidad))); // redibuja con su nueva transparencia
            ++it; // para la siguiente de la lista
        }
    }
}


void MotorFisicasArena::generarOndaChoque(sf::Vector2f pos, Bando b, std::list<EfectoOnda>& listaOndas) {
    EfectoOnda onda; // crea un struct
    onda.radio = 20.f; // nace con 20 de radio
    onda.opacidad = 255.f; // totalmente solida
    onda.forma.setFillColor(sf::Color::Transparent); // vacia por el medio
    onda.forma.setOutlineThickness(4.f); // solo se ve el borde de 4 pixeles
    onda.colorBando = (b == Bando::LUZ) ? sf::Color::Cyan : sf::Color::Red; // cian para los buenos, rojo para los malos
    onda.forma.setPosition(pos); // se coloca debajo del tipo que da el golpe
    listaOndas.push_back(onda); 
}