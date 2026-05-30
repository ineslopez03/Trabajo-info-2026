#include "MotorArchon.h"
#include "Tablero.h"
#include "Arena.h"
#include "MenuPrincipal.h"
#include "PantallaNombre.h" 
#include"PantallaRanking.h"
#include <optional>

MotorArchon::MotorArchon() {//constructor
    estadoActual = EstadoJuego::MENU;//iniciamos el juego en el menu
    pantallaActiva = nullptr;//asignamos puntero vacío para que no apunte a nada y evitar problemas de acceso indebido
    jugador1 = nullptr;
    jugador2 = nullptr;
    miTablero = nullptr;
    ejecutando = true;//flag de que el juego esté en ejecución
	skinActual = "ARCHON";//La skin por defecto es la de Archon
    //Inicia la musica de fondo
    if (MusicaFondo.openFromFile("../ARCHON/Musica/MusicaFondo.mp3")) {//Lo abre DESDE el archivo
        MusicaFondo.setVolume(30.0f);//asignamos el volumen 
        MusicaFondo.setLooping(true);//lo ponemos en bulce
        MusicaFondo.play();//play 
    }
}

MotorArchon::~MotorArchon() {//destructor de todo lo creado con new
    delete pantallaActiva;
    delete jugador1;
    delete jugador2;
}

void MotorArchon::cambiarEstado(EstadoJuego nuevoEstado, Pieza* p1, Pieza* p2, std::string skinSeleccionada) {
    if (nuevoEstado == EstadoJuego::TABLERO || nuevoEstado == EstadoJuego::ARENA) {//para que no moleste la musica
        MusicaFondo.pause(); 
    }
    else {
        if (MusicaFondo.getStatus() != sf::SoundSource::Status::Playing) {
            MusicaFondo.play();//si no estamos en arena o tablero y la musica no suena, se pone a sonar.
        }
    }
    if (pantallaActiva != nullptr && pantallaActiva != miTablero) {
        delete pantallaActiva;//deleteamos la pantalla activa si no es el tablero (se mantiene vivo de esta forma)
    }
	pantallaActiva = nullptr;//se asigna puntero vacío para evitar problemas de acceso indebido
    estadoActual = nuevoEstado;//se asigna el nuevo estado 
    if (!skinSeleccionada.empty()) {//si la skinseleccionada no esta vacia, se asigna a la skin actual.
        skinActual = skinSeleccionada;
    }
    switch (estadoActual) {//switch para cambiar de estado y repartir la ventana activa
    case EstadoJuego::MENU:
		ventana.setView(ventana.getDefaultView());//se asigna la vista por defecto a la ventana para el menu
        pantallaActiva = new MenuPrincipal();  //se crea un menu nuevo cada vez que se regresa, para no tenerlo consumiendo
        break;
    case EstadoJuego::TABLERO:
        if (miTablero == nullptr) {//si el tablero no existe
            miTablero = new Tablero(95.0f, skinActual);//se crea con la skin que se haya seleccionado en el menu 
        }
        ventana.setView(sf::View(sf::FloatRect({ 0.f, 0.f }, { 1100.f, 855.f })));//se asigna vista personalizada al tablero 
		pantallaActiva = miTablero;// se asigna el tablero como pantalla activa para que se dibuje y procese entrada,
        //pero no se borra al cambiar de estado para mantener su estado durante el juego. 
        //Solo se borra si se vuelve al menu desde el tablero, para luego volver a crear uno nuevo al iniciar una nueva partida.
        break;
    case EstadoJuego::ARENA:
		if (p1 != nullptr && p2 != nullptr) {//si tenemos dos piezas que van a combatir, se crea la arena con 
            //esas piezas y la skin seleccionada en el menu
			pantallaActiva = new Arena(p1, p2, skinActual, p1);//se crea la pantalla de combate con las piezas que van
            //a combatir, la skin seleccionada y la pieza atacante real (para procesar el resultado del 
            //combate en el tablero)
        }
        break;
    case EstadoJuego::INGRESAR_NOMBRE://cuando se acaba la partida
    {
        Bando bandoGanadorCompleto = Bando::LUZ;//por defecto se asigna la luz como ganador
        if (miTablero != nullptr && miTablero->getEstadoVictoria() == 2) {
            bandoGanadorCompleto = Bando::OSCURIDAD;//El tablero ya sabe quién ganó,solo lo consultamos
        }
        pantallaActiva = new PantallaNombre(bandoGanadorCompleto);//se crea la pantalla de ingreso de nombre con el bando ganador 
    }
    break;
    case EstadoJuego::RANKING://EN CASO DE QUE SE QUIERA VER EL RANKING DESDE EL MENU 
		ventana.setView(ventana.getDefaultView());//Se asigna la vista por defecto a la ventana para el ranking
        pantallaActiva = new PantallaRanking();//se crea la pantalla 
        break;
	case EstadoJuego::FIN://si el estado es fin, se asigna el flag a 0 para salir del juego.
        ejecutando = false;
        break;
    }
}

void MotorArchon::bucle() {//bucle del juego
    while (ejecutando && ventana.isOpen()) {//si el juego está en ejecución y la ventana está abiert
        if (pantallaActiva != nullptr) {//si tenemos una pantalla activa
            pantallaActiva->procesarEntrada(ventana);//procesamos la entrada

            //Comprobamos si la pantalla de Nombre ha terminado
            if (estadoActual == EstadoJuego::INGRESAR_NOMBRE) {//si estamos en la pantalla de ingreso de nombre
                PantallaNombre* pNombre = dynamic_cast<PantallaNombre*>(pantallaActiva);//hacemos un dynamic cast
                if (pNombre != nullptr && pNombre->esTransicionLista()) {//si el cast es exitoso y podemos hacer la transicion
                    cambiarEstado(EstadoJuego::MENU);//volvemos al menu
                }
            }
            //Comprobamos si la pantalla de Ranking ha terminado
			else if (estadoActual == EstadoJuego::RANKING) {//si estamos en la pantalla de ranking
                PantallaRanking* pRanking = dynamic_cast<PantallaRanking*>(pantallaActiva);//hacemos un dynamic cast
                if (pRanking != nullptr && pRanking->esTransicionLista()) {//si el cast es exitoso y podemos hacer la transicion
					cambiarEstado(EstadoJuego::MENU);//volvemos al menu
                }
            }
			//Escuchamos al menu 
			else if (estadoActual == EstadoJuego::MENU) {//si estamos en el menu
                MenuPrincipal* menu = dynamic_cast<MenuPrincipal*>(pantallaActiva);//dynamic cast
                if (menu != nullptr) {//si el cast es exitoso
                    if (menu->getIniciarJuego()) {//y se ha seleccionado iniciar juego
                        cambiarEstado(EstadoJuego::TABLERO, nullptr, nullptr, menu->getSkinSeleccionada());
                    }//vamos al tablero, con la skin seleccionada
                    else if (menu->getVerRanking()) {//si se ha seleccionado ver ranking
                   
						cambiarEstado(EstadoJuego::RANKING);//vamos al ranking
                    }
                }
            }
            //Logica del Tablero
			else if (estadoActual == EstadoJuego::TABLERO) {//si estamos en el tablero
				Tablero* tab = dynamic_cast<Tablero*>(pantallaActiva);//dynamic cast
				if (tab != nullptr) {//si el cast es exitoso
                    if (tab->debeVolverAlMenu()) {//si el tablero nos indica que acabamos la partida
                        cambiarEstado(EstadoJuego::INGRESAR_NOMBRE);//vamos a la pantalla de ingreso de nombre
                        delete miTablero;//borramos el tablero para liberar memoria,ya no lo necesitamos
                        miTablero = nullptr;//y vaciamos el puntero 
                    }
                    else if (tab->getHaycombate()) {//si el tablero nos indica que hay un combate pendiente
                        Pieza* pAtacante = tab->getAtacante();//obtenemos el atacante
                        Pieza* pDefensor = tab->getDefensor();//obtenemos el defensor
                        tab->limpiarBanderaCombate();//limpiamos la bandera de combate
                        cambiarEstado(EstadoJuego::ARENA, pAtacante, pDefensor, tab->getSkin());//vamos a la arena
                        //con las piezas y con la skin
                    }
                }
            }
            //Logica de la Arena
            else if (estadoActual == EstadoJuego::ARENA) {//si estamos en la arena
                Arena* arena = dynamic_cast<Arena*>(pantallaActiva);//hacemos un dynamic cast
                if (arena != nullptr) {//si es exitoso
                    if (arena->isTransicionLista()) {//si la arena nos indica que podemos hacer la transicion de vuelta al tablero

                        if (miTablero != nullptr) {//si hay tablero (debería haberlo siempre, pero por si acaso)
                            miTablero->procesarResultadoCombate(arena->getGanador(), arena->getPerdedor(), arena->getPiezaAtacanteReal());
                        }//obtenemos al ganador y al perdedor de la arena, y a la pieza atacante real para procesar el resultado del combate en el tablero

                        // Volvemos al tablero
                        cambiarEstado(EstadoJuego::TABLERO);
                    }
                }
            }
        }

        
        ventana.clear();//vaciamos la pantalla antigua

        if (pantallaActiva != nullptr) {//si hay pantalla activa
            pantallaActiva->dibujarPantalla(ventana);//se llama a su metodo de dibujo
        }

		ventana.display();//se muestra la pantalla nueva
    }
}

void MotorArchon::inicializar() {//Inicializaciones
    ventana.create(sf::VideoMode({ 1100, 855 }), "ARCHON 2026");//se crea la ventana
    sf::View vistaMenu(sf::FloatRect({ 0.f, 0.f }, { 1100.f, 855.f }));// se crea una vista personalizada
    ventana.setView(vistaMenu);//se asigna la vista a la ventana 
    jugador1 = new JugadorHumano("Heroe de la Luz", Bando::LUZ);//se crean los jugadores 
    jugador2 = new JugadorHumano("Senor de la Oscuridad", Bando::OSCURIDAD);
    cambiarEstado(EstadoJuego::MENU);//se inicia el juego en el menu por defecto
}