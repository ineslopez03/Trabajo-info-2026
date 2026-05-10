#include "MenuPrincipal.h"
#include <iostream>
MenuPrincipal::MenuPrincipal() {
	EstadoInterno = OpcionesMenu::PRINCIPAL;
	ContraIA = false;
	IniciarJuego = false;
	SkinSeleccionada = "Archon";
    //https://fontmeme.com/fuentes
	//if (!FuenteMenu.openFromFile("../ARCHON/fuentes/gameover.otf")) {//es la carpeta del slnx desde la cual puedo hacer ruta relativa
	//if (!FuenteMenu.openFromFile("../ARCHON/fuentes/Phantom Shadow Display Demo.otf")) {//es la carpeta del slnx desde la cual puedo hacer ruta relativa
	//if (!FuenteMenu.openFromFile("../ARCHON/fuentes/Mega Binory.otf")) {
	//if (!FuenteMenu.openFromFile("../ARCHON/fuentes/Darkest Saturday Aged.otf")) {
	//if (!FuenteMenu.openFromFile("../ARCHON/fuentes/Chopera.otf")) {
	if (!FuenteMenu.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {
		std::cout << "Error cargando la fuente\n\n";
		system("pause");
		exit(1);
	}
	inicializarBotones();
}

MenuPrincipal::~MenuPrincipal() {

}

void MenuPrincipal::procesarEntrada(sf::RenderWindow& ventana) {
	while (auto evento = ventana.pollEvent()) {
		if (evento->is<sf::Event::Closed>()) {
			ventana.close();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {}
}

void MenuPrincipal::dibujarPantalla(sf::RenderWindow& ventana) {
	if (EstadoInterno == OpcionesMenu::PRINCIPAL) {
		for (auto& boton : BotonesMenuPrincipal)
			boton.dibujar(ventana);
	}
}
void MenuPrincipal::inicializarBotones() {
	int centroMenu_x{ 800 };

	BotonesMenuPrincipal.push_back(Boton((800-300)/2, 250, 300, 50, "INICIAR PARTIDA", FuenteMenu));
}