#include "MenuPrincipal.h"
MenuPrincipal::MenuPrincipal() {
	
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