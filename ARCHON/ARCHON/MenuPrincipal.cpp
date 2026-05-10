#include "MenuPrincipal.h"
MenuPrincipal::MenuPrincipal() {
	EstadoInterno = OpcionesMenu::PRINCIPAL;
	ContraIA = false;
	IniciarJuego = false;
	SkinSeleccionada = "Archon";
	FuenteMenu.openFromFile("fuentes-game-over.otf");//si se quiere cambiar la fuente cambiar gameover por otra fuente
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
	ventana.clear(sf::Color::Black);
	ventana.display();
}
