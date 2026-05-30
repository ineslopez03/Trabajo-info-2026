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

void MotorArchon::bucle() {
    while (ejecutando && ventana.isOpen()) {
        if (pantallaActiva != nullptr) {
            pantallaActiva->procesarEntrada(ventana);

            // 1. Comprobamos si la pantalla de Nombre ha terminado
            if (estadoActual == EstadoJuego::INGRESAR_NOMBRE) {
                PantallaNombre* pNombre = dynamic_cast<PantallaNombre*>(pantallaActiva);
                if (pNombre != nullptr && pNombre->esTransicionLista()) {
                    cambiarEstado(EstadoJuego::MENU);
                }
            }
            // 2. Comprobamos si la pantalla de Ranking ha terminado
            else if (estadoActual == EstadoJuego::RANKING) {
                PantallaRanking* pRanking = dynamic_cast<PantallaRanking*>(pantallaActiva);
                if (pRanking != nullptr && pRanking->esTransicionLista()) {
                    cambiarEstado(EstadoJuego::MENU);
                }
            }
            // 3. ¡AQUÍ ESTÁ EL CÓDIGO QUE FALTA! Escuchamos al Menú Principal
            else if (estadoActual == EstadoJuego::MENU) {
                MenuPrincipal* menu = dynamic_cast<MenuPrincipal*>(pantallaActiva);
                if (menu != nullptr) {
                    if (menu->getIniciarJuego()) {
                        cambiarEstado(EstadoJuego::TABLERO, nullptr, nullptr, menu->getSkinSeleccionada());
                    }
                    else if (menu->getVerRanking()) {
                        // ¡Esta línea es la que te salva y crea la nueva pantalla!
                        cambiarEstado(EstadoJuego::RANKING);
                    }
                }
            }
            // 4. Lógica del Tablero
            else if (estadoActual == EstadoJuego::TABLERO) {
                Tablero* tab = dynamic_cast<Tablero*>(pantallaActiva);
                if (tab != nullptr) {
                    if (tab->debeVolverAlMenu()) {
                        cambiarEstado(EstadoJuego::INGRESAR_NOMBRE);
                        delete miTablero;
                        miTablero = nullptr;
                    }
                    else if (tab->getHaycombate()) {
                        Pieza* pAtacante = tab->getAtacante();
                        Pieza* pDefensor = tab->getDefensor();
                        tab->limpiarBanderaCombate();
                        cambiarEstado(EstadoJuego::ARENA, pAtacante, pDefensor, tab->getSkin());
                    }
                }
            }
            // 5. Lógica de la Arena
            else if (estadoActual == EstadoJuego::ARENA) {
                Arena* arena = dynamic_cast<Arena*>(pantallaActiva);
                if (arena != nullptr) {
                    if (arena->isTransicionLista()) {

                        // Le pasamos el ganador y el perdedor directamente desde la Arena al Tablero
                        if (miTablero != nullptr) {
                            miTablero->procesarResultadoCombate(arena->getGanador(), arena->getPerdedor(), arena->getPiezaAtacanteReal());
                        }

                        // Volvemos al tablero
                        cambiarEstado(EstadoJuego::TABLERO);
                    }
                }
            }
        }

        // --- ZONA DE DIBUJADO ---
        ventana.clear();

        if (pantallaActiva != nullptr) {
            pantallaActiva->dibujarPantalla(ventana);
        }

        ventana.display();
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