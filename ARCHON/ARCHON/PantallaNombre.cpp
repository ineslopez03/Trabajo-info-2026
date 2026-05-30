#include "PantallaNombre.h"
#include "MotorArchon.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

struct RegistroRanking {
    std::string nombre;
    int victorias;
};

PantallaNombre::PantallaNombre(Bando ganador)
    : textoTitulo(fuente),
    SpriteFondo(Fondo),
    textoInstrucciones(fuente),
    textoNombre(fuente),
    textoBotonEnter(fuente)
{
    bandoGanador = ganador;//se asigna el bando ganador al atributo de la clase para configurar el mensaje de victoria
    nombreIngresado = "";//inicialmente el nombre ingresado esta vacio

    if (!Fondo.loadFromFile("../ARCHON/imagenes/Fondo Menu 800x800.png")) std::cout << "Error fondo menu\n";
    SpriteFondo.setTexture(Fondo, true);//le asignamos la textura del fondo al sprite
    float EscalaX = 1100.0f / Fondo.getSize().x;//se le da un tamaño de 1100x855 para que se adapte al ancho de la ventana
    float EscalaY = 855.0f / Fondo.getSize().y;//se le da un tamaño de 1100x855 para que se adapte al alto de la ventana
    SpriteFondo.setScale(sf::Vector2f(EscalaX, EscalaY));//se le asignan los tamaños al sprite

    if (!fuente.openFromFile("../ARCHON/fuentes/Rush Zone.otf")) {//carga la fuente, si no la encuentra lanza error
        std::cerr << "Error cargando fuente en PantallaNombre" << std::endl;
    }

    if (bandoGanador == Bando::LUZ) {//si gana la luz muestra el mensaje en azul cyan
        textoTitulo.setString("VICTORIA DEL BANDO DE LA LUZ");
        textoTitulo.setFillColor(sf::Color::Cyan);
    }
    else {//si gana la oscuridad muestra el mensaje en rojo
        textoTitulo.setString("VICTORIA DEL BANDO DE LA OSCURIDAD");
        textoTitulo.setFillColor(sf::Color::Red);
    }

    textoTitulo.setCharacterSize(40);//le asignamos un tamaño de letra grande para el titulo
    textoTitulo.setPosition(sf::Vector2f(150.f, 150.f)); //y se posiciona

    textoInstrucciones.setString("Introduce tu nombre para el Ranking:");//mensaje de instrucciones para el usuario
    textoInstrucciones.setFillColor(sf::Color::White);//se configura color tamaño posicion
    textoInstrucciones.setCharacterSize(25);
    textoInstrucciones.setPosition(sf::Vector2f(150.f, 250.f));

    textoNombre.setString("_");//el texto del nombre empieza con un guion bajo para indicar que se puede empezar a escribir
    textoNombre.setFillColor(sf::Color::Yellow);//se configura color tamaño posicion
    textoNombre.setCharacterSize(35);
    textoNombre.setPosition(sf::Vector2f(150.f, 320.f));

    textoBotonEnter.setString("[ Presiona ENTER para guardar y continuar ]");//mensaje para indicar al usuario
    textoBotonEnter.setFillColor(sf::Color(255, 0, 0, 150));//configuracion
    textoBotonEnter.setCharacterSize(20);
    textoBotonEnter.setPosition(sf::Vector2f(150.f, 450.f));
}//estos datos quedan configurados en el constructor para que se muestren al dibujar la pantalla

void PantallaNombre::procesarEntrada(sf::RenderWindow& ventana, MotorArchon* motor) {//metodo de procesar entradas
    while (const auto evento = ventana.pollEvent()) {//extraemos el evento
        if (evento->is<sf::Event::Closed>()) {//si es cerrar ventana
            ventana.close();//la cierra
        }
        if (const auto* textEntered = evento->getIf<sf::Event::TextEntered>()) {//si es un evento de texto metido

            if (textEntered->unicode == 8) {//si es un backspace, borra el ultimo caracter del nombre ingresado
                if (!nombreIngresado.empty()) {//si el nombre no esta vacio
                    nombreIngresado.pop_back();//borra el ultimo caracter de la cadena
                }
            }
            else if (textEntered->unicode == 13) {//si se presiona enter
                if (!nombreIngresado.empty()) {//y el nombre no esta vacio
                    guardarEnRanking(); //Lo guarda en el ranking
                    motor->encolarCambioEstado(EstadoJuego::MENU);//transita directamente al menu a través del motor
                }
            }
            else if (textEntered->unicode >= 32 && textEntered->unicode < 127) {//si se ingresa un caracter
                if (textEntered->unicode != ' ' && nombreIngresado.size() < 12) {//y no es un espacio y el nombre no supera los 12 caracteres
                    nombreIngresado += static_cast<char>(textEntered->unicode);//se agrega el caracter a la cadena del nombre ingresado
                }
            }

            if (nombreIngresado.empty()) {//si el nombre ingresado esta vacio
                textoNombre.setString("_");//el texto del nombre se muestra solo con el guion bajo
            }
            else {
                textoNombre.setString(nombreIngresado + "_");//si no, se muestra el nombre ingresado seguido de un guion
            }
        }
    }
}

void PantallaNombre::guardarEnRanking() {//para guardar el nombre ingresado en el ranking(al final de todo)
    std::vector<RegistroRanking> listaRanking;//vector para almacenar los registros del ranking
    //del archivo y lo que hemos metido
    bool encontrado = false;//si encontramos el nombre en el ranking activamos el flag para cambiar su numero de victorias
    std::ifstream archivoLectura("ranking.txt");//abrimos el archivo de texto del ranking

    if (archivoLectura.is_open()) {//si se ha podido abrir el archivo
        RegistroRanking reg;//se crea unregistro para almacenar cada linea del archivo
        while (archivoLectura >> reg.nombre >> reg.victorias) {//mientras podamos extraer un nombre y un numero de victorias del archivo
            if (reg.nombre == nombreIngresado) {//si el nombre extraido del archivo coincide con el nombre ingresado
                reg.victorias += 1;//le sumamos una victoria
                encontrado = true;//activamos el flag
            }
            listaRanking.push_back(reg);//se agrega el registro a la lista del ranking,
            //tanto si se ha encontrado el nombre como si no,
            //para luego reescribir el archivo con los datos actualizados
        }
        archivoLectura.close();//una vez leido todo lo cerramos
    }

    if (!encontrado) {//si no se detecta el nombre
        RegistroRanking nuevoReg;//creamos un nuevo registro para el nuevo nombre ingresado
        nuevoReg.nombre = nombreIngresado;//metemos el nombre nuevo
        nuevoReg.victorias = 1;//le sumamos una victoria
        listaRanking.push_back(nuevoReg);//y lo ponemos al final del todo
    }

    std::sort(listaRanking.begin(), listaRanking.end(), [](const RegistroRanking& a, const RegistroRanking& b) {
        return a.victorias > b.victorias;
        });//usamos la funcion sort con una lambda para ordenar el ranking de mayor a menor numero de victorias

    std::ofstream archivoEscritura("ranking.txt");//abrimos el archivo de texto del ranking
    if (archivoEscritura.is_open()) {//si se ha abierto
        for (const auto& jugador : listaRanking) {//recorremos la lista del ranking y escribimos cada registro en el archivo
            archivoEscritura << jugador.nombre << " " << jugador.victorias << "\n";//formato nombre victorias
        }
        archivoEscritura.close();//se cierra el archivo
    }
}

void PantallaNombre::dibujarPantalla(sf::RenderWindow& ventana) {
    sf::RectangleShape fondoMorado(sf::Vector2f(static_cast<float>(ventana.getSize().x), static_cast<float>(ventana.getSize().y)));
    //se hace un rectangulo morado semitransparente para que se vea el fondo.
    fondoMorado.setFillColor(sf::Color(20, 10, 30, 100));

    ventana.draw(SpriteFondo);//pintamos el fondo del menú
    ventana.draw(fondoMorado);//luego pinto el rectangulo ....
    ventana.draw(textoTitulo);
    ventana.draw(textoInstrucciones);
    ventana.draw(textoNombre);//pintamos todo por orden, de atras hacia adelante

    if (!nombreIngresado.empty()) {//si el nombre no esta vacio
        ventana.draw(textoBotonEnter);//puede pintar el mensaje de presionar enter para guardar y continuar
    }
}