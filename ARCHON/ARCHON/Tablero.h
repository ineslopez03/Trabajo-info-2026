#pragma once
#include "InterfazUsuario.h"
#include "Casilla.h"
#include "Constantes.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include "Boton.h"
#include <vector>
#include "GestorHechizos.h"
#include "GestorVictoria.h"
#include "GestorCombate.h"
#include "GestorTurno.h"
//clase principal del juego:coordina el tablero 9x9, los turnos,los hechizos,
//el combate y la victoria.Implementa InterfazUsuario para integrarse con MotorArchon.
//La lógica compleja está desacoplada en 4 gestores especializados que acceden 
//a los atributos privados mediante la declaración friend.
class Tablero : public InterfazUsuario {
    friend class GestorHechizos;//accede a atributos privados para ejecutar hechizos
    friend class GestorVictoria;//accede a atributos privados para verificar y animar la victoria
    friend class GestorCombate;//accede a atributos privados para resolver combates
    friend class GestorTurno;//accede a atributos privados para gestionar los turnos

private:
    std::string skinActual;//pack de imágenes activo    
    std::vector<Casilla*> casillasValidas;//casillas resaltadas al seleccionar una pieza
    Casilla* matriz[9][9];//cuadrícula de 81 casillas del tablero 9x9        
    sf::View vistaEstatica; //área visible: 9 casillas + espacio lateral para botones        
    float tamCasilla;//tamaño en píxeles de cada casilla               
    Bando turnoActual;//bando que tiene el turno ahora mismo           
    bool primerClicRealizado;//true si ya hay una pieza seleccionada        
    Casilla* origenSeleccionado;//casilla de la pieza que está seleccionada
    Casilla* piezaAuxiliar = nullptr;//casilla auxiliar para hechizos Teleport y Exchange    
    bool hayCombatePendiente; //true cuando MotorArchon debe lanzar la Arena     
    Pieza* atacante;//pieza que inició el combate
    Pieza* defensor;//pieza que recibe el ataque                 
    int turnosContados; //contador global de turnos,controla el ciclo de colores            
    bool modoHechizoActivo = false;  //true cuando el Hechicero espera un objetivo
    int hechizoSeleccionado = 0;  //id del hechizo activo(1-7), 0 si ninguno   
    bool hechizosLuzUsados[8] = { false };//rastrea qué hechizos ha gastado LUZ     
    bool hechizosOscurosUsados[8] = { false }; // rastrea qué hechizos ha gastado OSCURIDAD
    bool bandoLuzUsoMagia; //true si LUZ ya usó la magia este turno         
    bool bandoOscuroUsoMagia;//true si OSCURIDAD ya usó la magia este turno     
    sf::Font fuente; //fuente para el texto de Victoria               
    std::vector<Boton*> botonesHechizos; // 7 botones de hechizos a la derecha del tablero   
    std::vector<Pieza*> piezasMuertasLuz; //Cementerio de LUZ(para hechizo Revive)     
    std::vector<Pieza*> piezasMuertasOscuridad;//Cementerio de OSCURIDAD(para hechizo Revive) 
    sf::Vector2i coordenadasCombate;//posición en el tablero donde ocurre el combate
    sf::Clock relojTablero;//reloj para los temporizadores de la animación de victoria     
    int estadoVictoria = 0;//0=jugando,1=gana LUZ, 2=gana OSCURIDAD,3= empate          
    int motivoVictoria = 0; // 1= nodos,2=aniquilación,3=prisión         
    int faseVictoria = 0; //0=gameplay,1=delay prisión,2=parpadeo verde,3=pantalla negra           
    float temporizadorFase = 0.f;//segundos restantes de la fase actual de victoria   
    sf::Text textoVictoria; //texto del ganador en la pantalla final         
    bool volverAlMenu = false; //true cuando la cinemática de victoria termina
    //Gestores desacoplados, acceden a los privados via friend
    GestorHechizos gestorHechizos;
    GestorVictoria gestorVictoria;
    GestorCombate  gestorCombate;
    GestorTurno    gestorTurno;
    void inicializarBotones();//crea los 7 botones de hechizos
    void gestionarTurno(Casilla* casillaClicada);//delega en GestorTurno
    bool esMovimientoValido(Casilla* origen, Casilla* destino);//valida si el movimiento es legal
    void procesarMagia(Casilla* objetivo);//delega en GestorHechizos
    void finalizarTurno();//delega en GestorTurno
    void registrarMuerte(Pieza* p);//delega en GestorCombate
    void eliminarPiezaDelMapa(Pieza* p);//delega en GestorCombate
    void moverPiezaACasilla(Pieza* p, sf::Vector2i destino);//delega en GestorCombate

public:
    Tablero();//constructor por defecto:95px, sin skin
    Tablero(float tam, std::string skin);
    virtual ~Tablero() override;

    void inicializarTablero(); //coloca las 18 piezas de cada bando en posición inicial
    void procesarEntrada(sf::RenderWindow& ventanaJuego) override;//bucle de eventos+victoria
    void dibujarPantalla(sf::RenderWindow& ventanaJuego) override;//pipeline gráfico del tablero
    //Getters consultados por MotorArchon para coordinar los estados del juego
    bool getHaycombate() const { return hayCombatePendiente; }
    Pieza* getAtacante() const { return atacante; }
    Pieza* getDefensor() const { return defensor; }
    void limpiarBanderaCombate();//resetea hayCombatePendiente tras lanzar la Arena
    void procesarResultadoCombate(Pieza* ganador, Pieza* perdedor, Pieza* atacanteOriginal);
    std::string getSkin() const { return skinActual; }
    sf::Vector2i getCoordenadasCombate() const { return coordenadasCombate; }
    ColorCasilla getColorCasilla(int x, int y);//devuelve el color actual de la casilla para el bono
    bool debeVolverAlMenu() const { return volverAlMenu; }
    int getEstadoVictoria() const { return estadoVictoria; }//1=LUZ,2=OSCURIDAD,3=empate
};