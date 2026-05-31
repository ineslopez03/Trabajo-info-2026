#include "Pieza.h"
#include "Casilla.h"
#include <algorithm>
// Aplica un bono temporal de vida basado en el color de la casilla donde combate la pieza
void Pieza::aplicarBonoColor(int porcentaje)
{
    // el incremento se calcula de forma estricta sobre la vida base de diseño
    int incremento = (vidaMaximaBase * porcentaje) / 100;// Sube el techo de vida temporalmente
    this->vidaMaxima = vidaMaximaBase + incremento; // Aumenta la vida actual en la misma cantidad
    this->vida += incremento;
}
// Restaura los valores de vida al estado previo al combate
void Pieza::restaurarValoresOriginales(int vidaAntesDelCombate)
{
    // Restaura el tope físico al valor de diseño (elimina el bono de color)
    this->vidaMaxima = vidaMaximaBase;
    // Aplica el daño sufrido en la arena: la vida que tenía antes del combate menos el daño recibido
    this->vida = vidaAntesDelCombate;
    if (this->vida > this->vidaMaxima) {
        this->vida = this->vidaMaxima; // No puede quedar con más vida que el máximo base
    }
}
// Cura la pieza en 'cantidad' puntos de vida sin superar el máximo base
void Pieza::curar(int cantidad) {
    this->vida += cantidad;
    if (this->vida > this->vidaMaximaBase) { // Suma los puntos de curación
        this->vida = this->vidaMaximaBase;// Limita al máximo base (sin bonos)
    }
}
void Pieza ::procesarEfectoVisual() {
    // Aplica el color correcto al sprite según si está recibiendo daño o no
    if (estaParpadeandoDanyo()) {
        sprite.setColor(sf::Color(255, 30, 30, 220));// Rojo intenso semitransparente al recibir daño
    }
    else {
        sprite.setColor(sf::Color::White);// Color normal (blanco = sin tinte)
    }
}
bool Pieza::estaParpadeandoDanyo() {
    // Devuelve true durante las ventanas de tiempo en que el sprite debe verse rojo
    if (mostrandoDanyo) {// Tiempo desde el último golpe recibido
        float t = relojDanyo.getElapsedTime().asSeconds();
        if (t < 0.5f) {
            // Durante los primeros 0.5 segundos: parpadea en dos intervalos
            return ((t > 0.0f && t < 0.15f) || (t > 0.25f && t < 0.40f));
            // Rojo de 0.00s a 0.15s, blanco de 0.15s a 0.25s, rojo de 0.25s a 0.40s, blanco después
        }
        else {
            mostrandoDanyo = false;// Pasados 0.5 segundos, desactiva el efecto
        }
    }
    return false;// Si no está parpadeando, devuelve false
}
void Pieza::recibirDanyo(int cantidad) {
    vida -= cantidad;// Resta el daño de la vida actual
    if (vida < 0) vida = 0;// La vida no puede bajar de 0

    mostrandoDanyo = true;//aparece un parpadeo rojo indicando que está recibiendo daño
    relojDanyo.restart();// Reinicia el reloj del efecto visual
}
