#include "MotorArchon.h"
#include "Tablero.h"
#include "Arena.h"
#include "MenuPrincipal.h"
#include "PantallaNombre.h"
#include "PantallaRanking.h"
#include <optional>

MotorArchon::MotorArchon() {//constructor
    estadoActual = EstadoJuego::MENU;//iniciamos el juego en el menu
    pantallaActiva = nullptr;//asignamos puntero vacío para que no apunte a nada y evitar problemas de acceso indebido
    jugador1 = nullptr;
    jugador2 = nullptr;
    miTablero = nullptr;
    ejecutando = true;//flag de que el juego esté en ejecución
    skinActual = "ARCHON";//La skin por defecto es la de Archon

    hayCambioPendiente = false;
    p1Pendiente = nullptr;
    p2Pendiente = nullptr;

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
    if (miTablero != nullptr && pantallaActiva != miTablero) {
        delete miTablero;
    }
}

void MotorArchon::encolarCambioEstado(EstadoJuego nuevoEstado, Pieza* p1, Pieza* p2, std::string skinSeleccionada) {
    hayCambioPendiente = true;
    estadoPendiente = nuevoEstado;
    p1Pendiente = p1;
    p2Pendiente = p2;
    if (!skinSeleccionada.empty()) {
        skinPendiente = skinSeleccionada;
    }
}

void MotorArchon::purgarTablero() {
    if (miTablero != nullptr) {
        if (pantallaActiva == miTablero) pantallaActiva = nullptr;
        delete miTablero;
        miTablero = nullptr;
    }
}

void MotorArchon::aplicarCambioEstado() {
    hayCambioPendiente = false;

    if (estadoPendiente == EstadoJuego::TABLERO || estadoPendiente == EstadoJuego::ARENA) {//para que no moleste la musica
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
    estadoActual = estadoPendiente;//se asigna el nuevo estado

    if (!skinPendiente.empty()) {//si la skinseleccionada no esta vacia, se asigna a la skin actual.
        skinActual = skinPendiente;
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
        if (p1Pendiente != nullptr && p2Pendiente != nullptr) {//si tenemos dos piezas que van a combatir, se crea la arena con
            //esas piezas y la skin seleccionada en el menu
            pantallaActiva = new Arena(p1Pendiente, p2Pendiente, skinActual, p1Pendiente);//se crea la pantalla de combate con las piezas que van
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

    p1Pendiente = nullptr;
    p2Pendiente = nullptr;
}

void MotorArchon::bucle() {
    while (ejecutando && ventana.isOpen()) {

        // 1. Resolución segura de memoria dinámica al INICIO del ciclo
        if (hayCambioPendiente) {
            aplicarCambioEstado();
        }

        // 2. Delegación polimórfica de la lógica de entrada
        if (pantallaActiva != nullptr) {
            pantallaActiva->procesarEntrada(ventana, this);
        }

        // 3. ZONA DE DIBUJADO
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
    encolarCambioEstado(EstadoJuego::MENU);//se inicia el juego en el menu por defecto
}