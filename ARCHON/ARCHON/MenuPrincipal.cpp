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
	struct vboton{
		float centroMenu_x{300};
		float anchoBoton{300};
		float altoBoton{50};
		float espaciado{ 80 };
	}vboton;
	//ESTOS SON LOS BOTONES PARA EL MENU PRINCIPAL
	BotonesMenuPrincipal.push_back(Boton(vboton.centroMenu_x, 250.0f,vboton.anchoBoton, vboton.altoBoton, "INICIAR PARTIDA", FuenteMenu));
	BotonesMenuPrincipal.push_back(Boton(vboton.centroMenu_x, 250.0f+vboton.espaciado, vboton.anchoBoton, vboton.altoBoton, "CARGAR PARTIDA", FuenteMenu));
	BotonesMenuPrincipal.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado*2, vboton.anchoBoton, vboton.altoBoton, "RANKING", FuenteMenu));
	BotonesMenuPrincipal.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado*3, vboton.anchoBoton, vboton.altoBoton, "MANUAL", FuenteMenu));
	BotonesMenuPrincipal.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado*4, vboton.anchoBoton, vboton.altoBoton, "SALIR", FuenteMenu));

}