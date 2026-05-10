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
	//obtengo la posicion del raton en pixeles y la traduzco a coordenadas 
	sf::Vector2f PosRaton = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));
	//Creo una funcion Lambda (ya que solo la voy a usar para esto) 
	//Le permito leer las variables de este fragmento con [&] que viene a ser la clausula de captura.	
	//basicamente le digo que recorra el vector de tipo boton que se le pase y que actualice el color 
	//en funcion de la posicion de mi raton.
	auto actualizar = [&](std::vector<Boton>& lista) {

		for (auto& b : lista) b.actualizarColorBoton(PosRaton);//con este for recorre todos los botones y consulta si estoy encima
		//tener en cuenta que mi clase boton para actualizar el color antes pregunta si tengo el raton encima
		};

	switch (EstadoInterno)
	{
	case OpcionesMenu::PRINCIPAL:
		actualizar(BotonesMenuPrincipal);//aqui uso mi funcion lambda
		break;
	case OpcionesMenu::RANKING:
		actualizar(BotonesRanking);
		break;
	case OpcionesMenu::SELECCION_MODO:
		actualizar(BotonesSeleccionModo);
		break;
	case OpcionesMenu::IA_NODISPONIBLE:
		actualizar(BotonVolverIA);
		break;
	case OpcionesMenu::SELECCION_SKIN:
		actualizar(BotonesSeleccionSkin);
		break;
	}

	while (auto evento = ventana.pollEvent()) {
		if (evento->is<sf::Event::Closed>()) {
			ventana.close();
		}
			else if (const auto* click = evento->getIf<sf::Event::MouseButtonPressed>()) {
				if (click->button == sf::Mouse::Button::Left) {
					//Menu principal
					if (EstadoInterno == OpcionesMenu::PRINCIPAL)
					{
						if (BotonesMenuPrincipal[0].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
						//if (BotonesMenuPrincipal[1].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::; por si queremos hacer el cargar partida es aqui 
						if (BotonesMenuPrincipal[2].botonContieneRaton(PosRaton)) system("start manual.txt");
						if (BotonesMenuPrincipal[3].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::RANKING;
						if (BotonesMenuPrincipal[4].botonContieneRaton(PosRaton)) ventana.close();
					}
					else if (EstadoInterno == OpcionesMenu::SELECCION_MODO) {//Modos
						if (BotonesSeleccionModo[0].botonContieneRaton(PosRaton))EstadoInterno = OpcionesMenu::SELECCION_SKIN;
						if (BotonesSeleccionModo[1].botonContieneRaton(PosRaton))EstadoInterno = OpcionesMenu::IA_NODISPONIBLE;
						if (BotonesSeleccionModo[2].botonContieneRaton(PosRaton))EstadoInterno = OpcionesMenu::PRINCIPAL;

					}//Ranking
					else if (EstadoInterno == OpcionesMenu::RANKING) {
						if (BotonesRanking[0].botonContieneRaton(PosRaton))EstadoInterno = OpcionesMenu::PRINCIPAL;
					}//Volver de la IA
					else if (EstadoInterno == OpcionesMenu::IA_NODISPONIBLE) {
						if (BotonVolverIA[0].botonContieneRaton(PosRaton))EstadoInterno = OpcionesMenu::SELECCION_MODO;
					}//Seleccion de skin
					else if (EstadoInterno == OpcionesMenu::SELECCION_SKIN) {
						if (BotonesSeleccionSkin[0].botonContieneRaton(PosRaton)) { SkinSeleccionada = "ARCHON"; IniciarJuego = true; }
						if (BotonesSeleccionSkin[1].botonContieneRaton(PosRaton)) { SkinSeleccionada = "HARRY POTTER"; IniciarJuego = true; }
						if (BotonesSeleccionSkin[2].botonContieneRaton(PosRaton)) { SkinSeleccionada = "STAR WARS"; IniciarJuego = true; }
						if (BotonesSeleccionSkin[2].botonContieneRaton(PosRaton)) EstadoInterno = OpcionesMenu::SELECCION_MODO;
					}
					
				}
			}

		}
	
}

void MenuPrincipal::dibujarPantalla(sf::RenderWindow& ventana) {
	auto dibujarLista = [&](std::vector<Boton>& lista) {
		for (auto& b : lista) b.dibujar(ventana);
		};//otra funcion lambda que funciona igual que la de arriba 
	switch (EstadoInterno)
	{
	case OpcionesMenu::PRINCIPAL:
		dibujarLista(BotonesMenuPrincipal);
		break;
	case OpcionesMenu::RANKING:
		dibujarLista(BotonesRanking);
		break;
	case OpcionesMenu::SELECCION_MODO:
		dibujarLista(BotonesSeleccionModo);
		break;
	case OpcionesMenu::IA_NODISPONIBLE:
		dibujarLista(BotonVolverIA);
		break;
	case OpcionesMenu::SELECCION_SKIN:
		dibujarLista(BotonesSeleccionSkin);
		break;
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
	//ESTOS SON LOS BOTONES PARA INICIAR PARTIDA
	BotonesSeleccionModo.push_back(Boton(vboton.centroMenu_x, 250.0f, vboton.anchoBoton, vboton.altoBoton, "JUGADOR VS JUGADOR", FuenteMenu));
	BotonesSeleccionModo.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado, vboton.anchoBoton, vboton.altoBoton, "JUGADOR VS IA", FuenteMenu));
	BotonesSeleccionModo.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado * 2, vboton.anchoBoton, vboton.altoBoton, "VOLVER", FuenteMenu));
	//ESTOS SON LOS BOTONES PARA LAS SKINS
	BotonesSeleccionSkin.push_back(Boton(vboton.centroMenu_x, 250.0f, vboton.anchoBoton, vboton.altoBoton, "ARCHON", FuenteMenu));
	BotonesSeleccionSkin.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado, vboton.anchoBoton, vboton.altoBoton, "HARRY POTTER", FuenteMenu));
	BotonesSeleccionSkin.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado * 2, vboton.anchoBoton, vboton.altoBoton, "STAR WARS", FuenteMenu));
	BotonesSeleccionSkin.push_back(Boton(vboton.centroMenu_x, 250.0f + vboton.espaciado * 3, vboton.anchoBoton, vboton.altoBoton, "VOLVER", FuenteMenu));
	//BOTON DE VOLVER DE LA IA
	BotonVolverIA.push_back(Boton(vboton.centroMenu_x, 250.0f, vboton.anchoBoton, vboton.altoBoton, "ARCHON", FuenteMenu));
	//BOTON PARA VOLVER DEL RANKING
	BotonesRanking.push_back(Boton(vboton.centroMenu_x, 250.0f, vboton.anchoBoton, vboton.altoBoton, "ARCHON", FuenteMenu));
}