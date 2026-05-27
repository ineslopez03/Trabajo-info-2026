#pragma once
#include "InterfazUsuario.h"
#include "Casilla.h"
#include "Constantes.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "Boton.h"
#include <vector>

class Tablero : public InterfazUsuario {
private:
    std::string skinActual;
    Casilla* matriz[9][9];
    sf::View vistaEstatica;
    float tamCasilla;
    Bando turnoActual;
    bool primerClicRealizado;
    Casilla* origenSeleccionado;
    Casilla* piezaAuxiliar = nullptr;
    bool hayCombatePendiente;
    Pieza* atacante;
    Pieza* defensor;
    int turnosContados;
    bool modoHechizoActivo = false;
    int hechizoSeleccionado = 0;
    bool hechizosLuzUsados[8] = { false };
    bool hechizosOscurosUsados[8] = { false };
    sf::Font fuente;
    std::vector<Boton*> botonesHechizos;
    void inicializarBotones();
    std::vector<Pieza*> piezasMuertasLuz;
    std::vector<Pieza*> piezasMuertasOscuridad;
    sf::Vector2i coordenadasCombate;

    // Subsistema de máquina de estados para la cinemática de cierre
    sf::Clock relojTablero;
    int estadoVictoria = 0;     // 1: LUZ, 2: OSCURIDAD, 3: EMPATE
    int motivoVictoria = 0;     // 1: Nodos, 2: Aniquilación, 3: Prisión
    int faseVictoria = 0;       // 0: Gameplay, 1: Delay Prisión, 2: Parpadeo Verde, 3: Pantalla Negra
    float temporizadorFase = 0.f;
    sf::Text textoVictoria;
    bool volverAlMenu = false;

public:
    Tablero();
    Tablero(float tam, std::string skin);
    virtual ~Tablero() override;
    void inicializarTablero();
    void procesarEntrada(sf::RenderWindow& ventanaJuego);
    void dibujarPantalla(sf::RenderWindow& ventanaJuego);
    void gestionarTurno(Casilla* casillaClic);
    bool esMovimientoValido(Casilla* origen, Casilla* destino);
    bool esAtaqueValido(Casilla* origen, Casilla* destino);
    bool getHaycombate() const { return hayCombatePendiente; }
    Pieza* getAtacante() const { return atacante; }
    Pieza* getDefensor() const { return defensor; }

    void limpiarBanderaCombate();
    void procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal);

    void procesarMagia(Casilla* objetivo);
    void finalizarTurno();
    std::string getSkin() const { return skinActual; }
    void registrarMuerte(Pieza* p);
    void eliminarPiezaDelMapa(Pieza* p);
    void moverPiezaACasilla(Pieza* p, sf::Vector2i destino);
    sf::Vector2i getCoordenadasCombate() const { return coordenadasCombate; }
    ColorCasilla getColorCasilla(int x, int y);

    int verificarVictoria();
    bool debeVolverAlMenu() const { return volverAlMenu; }
};