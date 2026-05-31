#include "Pieza.h"
#include "Casilla.h"
#include <algorithm>

void Pieza::aplicarBonoColor(int porcentaje) {//aplica un bono temporal de vida basado en el color de la casilla
    int incremento = (vidaMaximaBase * porcentaje) / 100;//el incremento se calcula sobre la base de diseño
    this->vidaMaxima = vidaMaximaBase + incremento;//sube el techo de vida
    this->vida += incremento;//aumenta la vida actual en la misma proporcion
}

void Pieza::restaurarValoresOriginales(int vidaAntesDelCombate) {//restaura la vida al terminar la pelea
    this->vidaMaxima = vidaMaximaBase;//restaura el tope fisico al valor normal
    this->vida = vidaAntesDelCombate;//aplica el daño sufrido en la arena
    if (this->vida > this->vidaMaxima) {//si sobrepasa el limite
        this->vida = this->vidaMaxima;//no puede quedar con mas vida que el maximo base
    }
}

void Pieza::curar(int cantidad) {//cura la pieza
    this->vida += cantidad;//suma los puntos de curacion
    if (this->vida > this->vidaMaximaBase) {//si se pasa del tope
        this->vida = this->vidaMaximaBase;//lo limita al maximo base
    }
}

void Pieza::procesarEfectoVisual() {//gestiona el tinte rojo del daño
    if (estaParpadeandoDanyo()) {//si esta en la ventana de tiempo del daño
        sprite.setColor(sf::Color(255, 30, 30, 220));//rojo intenso semitransparente
    }
    else {
        sprite.setColor(sf::Color::White);//color normal sin tintes
    }
}

bool Pieza::estaParpadeandoDanyo() {//controla el timing del parpadeo
    if (mostrandoDanyo) {//si recibio un golpe
        float t = relojDanyo.getElapsedTime().asSeconds();//tiempo desde el impacto
        if (t < 0.5f) {//dura medio segundo en total
            return ((t > 0.0f && t < 0.15f) || (t > 0.25f && t < 0.40f));//parpadea en dos intervalos intermitentes
        }
        else {
            mostrandoDanyo = false;//apaga el efecto
        }
    }
    return false;//no esta parpadeando
}

void Pieza::recibirDanyo(int cantidad) {//aplica el castigo matematico
    vida -= cantidad;//resta el daño
    if (vida < 0) vida = 0;//evita vidas negativas
    mostrandoDanyo = true;//activa la señal visual
    relojDanyo.restart();//inicia el cronometro del parpadeo
}
