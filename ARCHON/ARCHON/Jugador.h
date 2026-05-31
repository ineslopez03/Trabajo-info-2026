#pragma once
#include <vector>
#include <string>
#include "Pieza.h"

class Jugador {
    // Clase base abstracta que representa a un jugador (humano)
// Contiene la lista de piezas del jugador y su información básica
protected:
    std::string nombre; // Nombre del jugador
    Bando bando;// Bando al que pertenece (LUZ u OSCURIDAD)
    std::vector<Pieza*> piezas;// Lista de piezas activas del jugador
    int piezasRestantes; // Contador de piezas vivas

public:
    Jugador(std::string _nombre, Bando _bando);// Constructor: recibe nombre y bando
    virtual ~Jugador();
    // Destructor virtual para que las clases derivadas puedan limpiar correctamente
    virtual void decidirMovimiento() = 0;
    // Método puro abstracto: cada tipo de jugador humano implementa su propia lógica de decisión
    void anadirPieza(Pieza* p);
    // Añade una pieza a la lista y le notifica a la pieza quién es su jugador
    void eliminarPieza(Pieza* p);
    // Elimina una pieza de la lista (cuando muere en combate)
    Bando getBando() const { return bando; }
    // Getter: devuelve el bando del jugador
    std::string getNombre() const { return nombre; }
    // Getter: devuelve el nombre del jugador
    int getNumPiezas() const { return static_cast<int>(piezas.size()); }// Devuelve el número de piezas activas

    std::vector<Pieza*>& getPiezas() { return piezas; }
    // Devuelve una referencia al vector de piezas
};