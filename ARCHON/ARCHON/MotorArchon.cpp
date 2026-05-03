#include "MotorArchon.h"
#include "Tablero.h"
//#include "Arena"
#include "MenuPrincipal.h"




MotorArchon::MotorArchon(){
	ventana.create(sf::VideoMode({ 800, 800 }), "ARCHON"); //creo la ventana 
	estadoActual = EstadoJuego::MENU; //Empezamos en el menú
	//dejamos vacios los punteros al inicio y ya se les asignará valores cuando toque, evitamos accesos no habilitados
	pantallaActiva = nullptr; 
	jugador1 = nullptr;
	jugador2 = nullptr; 
	ejecutando = true; //ponemos a 1 nada mas iniciar para que el bucle se pueda ejecutar
}

MotorArchon::~MotorArchon(){
	//como los voy a crear dinamicamente usando new, debo hacer el delete
	if (pantallaActiva!=nullptr) delete pantallaActiva;
	if (jugador1 !=nullptr) delete jugador1;
	if (jugador2 !=nullptr) delete jugador2;
}

void MotorArchon::cambiarEstado(EstadoJuego NuevoEstado, Pieza* p1 , Pieza* p2) {
	//en el caso de	que vengamos de un estado previo borramos la pantalla y apuntamos a la nada.
	//si el puntero esta vacio no tengo que hacer nada
	if (pantallaActiva != nullptr) {
		delete pantallaActiva;
		pantallaActiva = nullptr;
	}
	estadoActual = NuevoEstado; //asigno directamente el estado actual por el estado que he pasado al metodo.

	
	//hago que el nuevo estado se represente
	switch(estadoActual) {
		case EstadoJuego::MENU://en el caso de que queramos representar el menú
			pantallaActiva = new MenuPrincipal();//creamos dinamicamente el objeto menu (llamamos a su constructor) y asigno su direccion a pantallaActica para despues acceder a el mediante los metodos del objeto
			//tipo pantallaActiva->metodo();
			break;

		case EstadoJuego::TABLERO://en el caso de que queramos representar el tabelro
			pantallaActiva = new Tablero();//se le asigna a pantallaActiva la clase Tablero que representara el tablero
			break;

		case EstadoJuego::ARENA://en el caso de que queramos representar la arena
			//pantallaActiva = new Arena(p1,p2);//lo mismo
			break;

		case EstadoJuego::FIN://en el caso de que se termine el juego
			ejecutando = false;//ejecutando a 0 para que salga del bucle principal.
			break;

	}
}

void MotorArchon::bucle() {
	while (ejecutando && ventana.isOpen()) {
		if (pantallaActiva != nullptr)
		{
			ventana.clear(); //limpio la ventana anterior antes de pintar

			pantallaActiva->procesarEntrada(ventana); //gracias al polimorfismo puedo decirle a pantallaActiva (sea el que sea el objeto)
			//que ejecute su propio metodo de procesar entrada(es un metodo de la interfaz(y pantallaActiva es un puntero de tipo interfaz) que heredan todas las clases hijas con el mismo nombre pero que implementan individualmente).
			
			//FUERZO QUE ENTRE A ESTADO TABLERO 
			if (estadoActual == EstadoJuego::MENU && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
				cambiarEstado(EstadoJuego::TABLERO);
			}

			if (estadoActual == EstadoJuego::TABLERO)
			{
				Tablero* tab = dynamic_cast<Tablero*>(pantallaActiva); //hago un cast para acceder a los metodos especificos del tablero, como comprobar victoria
			
				if (tab!=nullptr && tab->getHaycombate()){//uso la bandera de combate
					Pieza* atacante = tab->getAtacante(); //tomo las piezas atacante y defensor para el combate
					Pieza* defensor = tab->getDefensor();

					tab->resetCombate(); //reseteo el combate para que no se quede bloqueado en la fase de combate

					cambiarEstado(EstadoJuego::ARENA,atacante,defensor); //cambio a la pantalla de combate
				}
			}
			pantallaActiva->dibujarPantalla(ventana); //lo mismo pero para dibujar la pantalla
		
			ventana.display(); //muestro la ventana actualizada
		}
	}
}

void MotorArchon::inicializar() {

	//inicializamos jugadores y los estados
	//conceptual, se debe modificar 
	//no se si ponerlo en el constructor o en un metodo aparte, lo dejo aqui por ahora
	cambiarEstado(EstadoJuego::MENU); //para empezar en el menu
	jugador1 = new Jugador();
	jugador2 = new Jugador();

}