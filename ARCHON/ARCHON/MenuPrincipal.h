#pragma once
#include "InterfazUsuario.h"
#include <SFML/Graphics.hpp>
#include "Boton.h"
#include <vector>
#include <string>

enum class OpcionesMenu {
    PRINCIPAL,
    RANKING,
    SELECCION_MODO,
    IA_NODISPONIBLE,
    SELECCION_SKIN,
    MANUAL_GRAFICO
};

class MenuPrincipal : public InterfazUsuario {
private:
    sf::Font FuenteMenu;
    OpcionesMenu EstadoInterno;

    sf::Texture FondoMenu, FondoHarryPotter, FondoStarWars, FondoArchon;
    sf::Sprite SpriteFondo;
    sf::Text Titulos;

    std::vector<Boton> BotonesMenuPrincipal;
    std::vector<Boton> BotonesSeleccionModo;
    std::vector<Boton> BotonesSeleccionSkin;
    std::vector<Boton> BotonesRanking;
    std::vector<Boton> BotonVolverIA;

    std::vector<Boton> BotonesManual;
    std::vector<sf::Text> lineasReglas;
    std::vector<sf::Text> lineasHechizos;
    void cargarTextoManual();
    bool IniciarJuego;
    bool ContraIA;
    std::string SkinSeleccionada;

    void inicializarBotones();

public:
    MenuPrincipal();
    ~MenuPrincipal() override;

    void procesarEntrada(sf::RenderWindow& ventana) override;
    void dibujarPantalla(sf::RenderWindow& ventana) override;

    // Getters para la comunicación con MotorArchon
    bool getIniciarJuego() const { return IniciarJuego; }
    bool getContraIA() const { return ContraIA; }
    std::string getSkinSeleccionada() const { return SkinSeleccionada; }
    bool getVerRanking() const { return EstadoInterno == OpcionesMenu::RANKING; }
    void resetearEstadoMenu() { EstadoInterno = OpcionesMenu::PRINCIPAL; }
};