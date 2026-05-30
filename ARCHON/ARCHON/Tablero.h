#pragma once
#include "InterfazUsuario.h"
#include "Casilla.h"
#include "Constantes.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "Boton.h"
#include <vector>
#include "GestorHechizos.h"
#include "GestorVictoria.h"
#include "GestorCombate.h"
#include "GestorTurno.h"

class MotorArchon; // Declaración adelantada para inyección de dependencia

class Tablero : public InterfazUsuario {
    friend class GestorHechizos;
    friend class GestorVictoria;
    friend class GestorCombate;
    friend class GestorTurno;

private:
    std::string skinActual;
    std::vector<Casilla*> casillasValidas;
    Casilla* matriz[9][9];
    sf::View vistaEstatica;
    float tamCasilla;
    Bando turnoActual;
    bool primerClicRealizado;
    Casilla* origenSeleccionado;
    Casilla* piezaAuxiliar = nullptr;

    // Restauradas para mantener compatibilidad con GestorTurno.cpp
    bool hayCombatePendiente;
    Pieza* atacante;
    Pieza* defensor;

    int turnosContados;
    bool modoHechizoActivo = false;
    int hechizoSeleccionado = 0;
    bool hechizosLuzUsados[8] = { false };
    bool hechizosOscurosUsados[8] = { false };
    bool bandoLuzUsoMagia;
    bool bandoOscuroUsoMagia;
    sf::Font fuente;
    std::vector<Boton*> botonesHechizos;
    std::vector<Pieza*> piezasMuertasLuz;
    std::vector<Pieza*> piezasMuertasOscuridad;
    sf::Vector2i coordenadasCombate;

    sf::Clock relojTablero;
    int estadoVictoria = 0;
    int motivoVictoria = 0;
    int faseVictoria = 0;
    float temporizadorFase = 0.f;
    sf::Text textoVictoria;
    bool volverAlMenu = false;

    GestorHechizos gestorHechizos;
    GestorVictoria gestorVictoria;
    GestorCombate  gestorCombate;
    GestorTurno    gestorTurno;

    void inicializarBotones();
    // Firma corregida: eliminamos MotorArchon* para que coincida con el .cpp
    void gestionarTurno(Casilla* casillaClicada);
    bool esMovimientoValido(Casilla* origen, Casilla* destino);
    void procesarMagia(Casilla* objetivo);
    void finalizarTurno();
    void registrarMuerte(Pieza* p);
    void eliminarPiezaDelMapa(Pieza* p);
    void moverPiezaACasilla(Pieza* p, sf::Vector2i destino);

public:
    Tablero();
    Tablero(float tam, std::string skin);
    virtual ~Tablero() override;

    void inicializarTablero();
    void procesarEntrada(sf::RenderWindow& ventanaJuego, MotorArchon* motor) override;
    void dibujarPantalla(sf::RenderWindow& ventanaJuego) override;

    void procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal);
    void aplicarBonosCombate();

    void limpiarBanderaCombate(); // Restaurada para limpiar el flag tras transitar

    std::string getSkin() const { return skinActual; }
    sf::Vector2i getCoordenadasCombate() const { return coordenadasCombate; }
    ColorCasilla getColorCasilla(int x, int y);
    int getEstadoVictoria() const { return estadoVictoria; }
};