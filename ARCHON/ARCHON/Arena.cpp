#include "Arena.h" 
#include <iostream> 
#include <algorithm> 
#include <cmath> 
#include <typeinfo> 

//constructor de la clase arena. carga todo lo visual, asignar las piezas y preparar el combate
Arena::Arena(Pieza* p1, Pieza* p2, const std::string& skin, Pieza* atacante)
    : spriteFondoArena(nullptr), obstaculos(sf::Vector2f(100.f, 427.f), sf::Vector2f(1000.f, 427.f)), textoVictoria(fuenteVictoria) //inicializa los objetos de la clase y las coordenadas iniciales de los obstaculos
{
    this->atacanteOriginal = atacante; //guarda quien atacó en el tablero 
    this->skinArena = skin; //guarda la skin elegida en el menu
    iniciarBatalla(p1, p2); //coloca a cada uno en su lado
    faseCuentaAtras = 3; //empieza la cuenta atras en 3 segundos

   
    graficos.vincularPiezas(piezaIzquierda, piezaDerecha); //pasa los punteros a la clase de graficos para que lea la vida automaticamente

    std::string rutaFondo; //guarda la ruta de la imagen de fondo
    if (skinArena == "HARRY_POTTER") rutaFondo = "imagenes/HP/suelo_hp_2.png"; //skin de hp 
    else if (skinArena == "STAR_WARS") rutaFondo = "imagenes/SW/suelo_sw_2.png"; //skin de star wars 
    else rutaFondo = "imagenes/CLASSIC/suelo_clss_2.png"; //si no, carga archon clasico por defecto

    if (!texturaFondoArena.loadFromFile(rutaFondo)) { //intenta cargar la textura desde el archivo
        std::cerr << "Error cargando la textura de fondo de la Arena" << std::endl; //si falla escribe un error 
    }

    spriteFondoArena = new sf::Sprite(texturaFondoArena); //crea el sprite con la textura que acaba de cargar
    spriteFondoArena->setScale(sf::Vector2f(1100.f / texturaFondoArena.getSize().x, 855.f / texturaFondoArena.getSize().y)); //hacemos mas grande el fondo para que ocupe toda nuestra ventana de 1100x855

    if (!fuenteVictoria.openFromFile("C:/Windows/Fonts/arial.ttf")) { //la fuente arial
        std::cerr << "Error cargando fuente para victoria" << std::endl; //error por si no la encuentra
    }

    textoVictoria.setCharacterSize(60); //tamaño grande para el texto final
    textoVictoria.setOutlineThickness(4.f); //le ponemos un borde gordito para que se lea bien sobre el fondo
    textoVictoria.setOutlineColor(sf::Color::Black); //color del borde negro
}

//destructor de la arena para evitar fugas de memoria limpiando lo que creamos con new
Arena::~Arena() {
    for (auto p : lista_proyectiles) delete p; //recorre todos los proyectiles vivos y los borramos
    lista_proyectiles.clear(); //vacia la lista de laseres
    lista_ondas.clear(); //vacia la lista de ondas de choque
    delete spriteFondoArena; //borra el sprite del fondo de la memoria dinamica
}

//decide en que lado aparece cada pieza y mira si atacan a distancia o cuerpo a cuerpo
void Arena::iniciarBatalla(Pieza* p1, Pieza* p2) {
    if (p1->getBando() == Bando::LUZ) { //si la pieza 1 es de la luz
        piezaIzquierda = p1; //el bando de la luz siempre la pone a la izquierda
        piezaDerecha = p2; //y el de la oscuridad a la derecha
    }
    else { //si la pieza 1 era de la oscuridad, lo hace al reves
        piezaIzquierda = p2;
        piezaDerecha = p1;
    }

    auto forzarMelee = [](Pieza* p) { 
        std::string nombreClase = typeid(*p).name(); //obtiene el nombre real de la clase de la pieza
        if (nombreClase.find("Caballero") != std::string::npos || //si la pieza es un caballero
            nombreClase.find("Golem") != std::string::npos || //o un golem
            nombreClase.find("Goblin") != std::string::npos || //o un goblin
            nombreClase.find("Troll") != std::string::npos) { //o un troll
            p->setAtaqueCuerpoACuerpo(true); //le activa la bandera de que sus ataques son cuerpo a cuerpo
        }
        };

    forzarMelee(piezaIzquierda); //revisa la pieza de la izquierda
    forzarMelee(piezaDerecha); //y la de la derecha

    posIzquierda = sf::Vector2f(100.f, 427.f); //pone la posicion inicial del jugador 1 centrada a la izquierda
    posDerecha = sf::Vector2f(1000.f, 400.f); //y la del jugador 2 centrada a la derecha
    obstaculos.reiniciar(posIzquierda, posDerecha); //reinicia los obstaculos del mapa para que no salgan encima de los jugadores
}

//mueve las cosas, lee el teclado y calcula colisiones
void Arena::procesarEntrada(sf::RenderWindow& ventana) {
    if (faseCuentaAtras >= 0) { //si todavia esta en la cuenta atras inicial
        float tiempoTranscurrido = relojCuentaAtras.getElapsedTime().asSeconds(); //mira cuanto tiempo ha pasado
        if (tiempoTranscurrido >= 1.0f) { //si ha pasado un segundo
            faseCuentaAtras--; //baja el numero de la cuenta atras
            relojCuentaAtras.restart(); //reinicia el reloj para el siguiente segundo
        }
        relojArena.restart(); //mantiene a cero el reloj para que nadie se mueva antes de tiempo
        return; //sale del metodo para que no procese el teclado todavia
    }

    float dt = relojArena.restart().asSeconds(); //calcula el delta time para que todo se mueva a la misma velocidad

    if (combateFinalizado) { //si alguien ha muerto
        temporizadorSalida -= dt; //empieza a restar tiempo al cronometro final
        return; //y sale para bloquear los controles
    }

    obstaculos.actualizar(dt, posIzquierda, posDerecha); //actualiza el ciclo de vida de los orbes 
    MotorFisicasArena::propagarOndas(lista_ondas, dt); //hace que las ondas de choque crezcan y se difuminen

    if (tiempoRestanteCooldownIzq > 0.f) tiempoRestanteCooldownIzq -= dt; //si el jugador 1 esta en cooldown, se lo restamos
    if (tiempoRestanteCooldownDer > 0.f) tiempoRestanteCooldownDer -= dt; //lo mismo para el jugador 2

    // --- procesamiento flanco izquierdo ---
    sf::Vector2f dirIzq(0.f, 0.f); //crea un vector a cero para el movimiento del jugador 1
    if (tiempoRestanteCooldownIzq <= 0.f) { //solo se puede mover o atacar si no esta en cooldown de ataque
        IntencionJugador intIzq = ControladorPelea::obtenerIntencionIzquierda(); //leem que teclas esta pulsando
        dirIzq = intIzq.direccionMovimiento; //le pasa la direccion obtenida al vector

        if (intIzq.intentandoAtacar) { //si ha pulsado el boton de disparo
            if (teclaDisparoIzquierdaLibre) { //y ya habia soltado la tecla previamente
                if (piezaIzquierda->esCuerpoACuerpo()) { //si es un personaje cuerpo a cuerpo
                    MotorFisicasArena::generarOndaChoque(posIzquierda, piezaIzquierda->getBando(), lista_ondas); //genera la onda visual
                    if (std::hypot(posIzquierda.x - posDerecha.x, posIzquierda.y - posDerecha.y) <= 95.f) { //calcula la distancia y si esta cerca
                        piezaDerecha->recibirDanyo(piezaIzquierda->getDanio()); //le hace el daño directo
                    }
                }
                else { //si es un personaje de disparo laser
                    sf::Vector2f dirProyectil = MotorFisicasArena::calcularDireccionOrtogonal(posIzquierda, posDerecha); //calcula a donde debe salir la bola
                    lista_proyectiles.push_back(new Proyectiles(posIzquierda.x, posIzquierda.y, piezaIzquierda->getDanio(), 600.0f, dirProyectil, piezaIzquierda->getBando(), skinArena)); //crea el proyectil y lo mete en la lista
                }
                teclaDisparoIzquierdaLibre = false; //bloquea el boton para que tenga que soltarlo
                tiempoRestanteCooldownIzq = 1.4f - (piezaIzquierda->getVelAta() * 0.2f); //le pone el enfriamiento en base a sus atributos
            }
        }
        else { //si no esta intentando atacar
            teclaDisparoIzquierdaLibre = true; //libera la tecla para su proximo disparo
        }
    }
    MotorFisicasArena::aplicarFisicaMovimiento(piezaIzquierda, posIzquierda, dirIzq, posDerecha, dt, obstaculos); //aplica el movimiento con todas las colisiones de muros

    // --- procesamiento flanco derecho ---
    sf::Vector2f dirDer(0.f, 0.f); //vector a cero para el jugador 2
    if (tiempoRestanteCooldownDer <= 0.f) { //si no tiene penalizacion de tiempo
        IntencionJugador intDer = ControladorPelea::obtenerIntencionDerecha(); //lee sus flechitas del teclado
        dirDer = intDer.direccionMovimiento; //las aplica al vector

        if (intDer.intentandoAtacar) { //si quiere atacar
            if (teclaDisparoDerechaLibre) { //y no tiene la tecla atascada
                if (piezaDerecha->esCuerpoACuerpo()) { //si es de cuerpo a cuerpo
                    MotorFisicasArena::generarOndaChoque(posDerecha, piezaDerecha->getBando(), lista_ondas); //onda expansiva visual
                    if (std::hypot(posDerecha.x - posIzquierda.x, posDerecha.y - posIzquierda.y) <= 95.f) { //si el rival esta lo suficientemente pegado
                        piezaIzquierda->recibirDanyo(piezaDerecha->getDanio()); //le hace daño
                    }
                }
                else { //si dispara laseres
                    sf::Vector2f dirProyectil = MotorFisicasArena::calcularDireccionOrtogonal(posDerecha, posIzquierda); //calcula en cruz donde esta el enemigo
                    lista_proyectiles.push_back(new Proyectiles(posDerecha.x, posDerecha.y, piezaDerecha->getDanio(), 600.0f, dirProyectil, piezaDerecha->getBando(), skinArena)); //suelta el proyectil a la lista
                }
                teclaDisparoDerechaLibre = false; //bloquea tecla
                tiempoRestanteCooldownDer = 1.4f - (piezaDerecha->getVelAta() * 0.2f); //cooldown calculado por la velocidad del personaje
            }
        }
        else { //si ya solto la tecla
            teclaDisparoDerechaLibre = true; //le vuelve a dar permiso de disparar
        }
    }
    MotorFisicasArena::aplicarFisicaMovimiento(piezaDerecha, posDerecha, dirDer, posIzquierda, dt, obstaculos); //le pasa sus fisicas y colisiones

    gestionarColisiones(); // llama al metodo de abajo para barrer los laseres que hayan chocado

    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) { //recorre los proyectiles vivos
        (*it)->mover(dt); //los desplaza por la pantalla multiplicando por el tiempo
        if ((*it)->getPosicion().x > 1100 || (*it)->getPosicion().x < 0 || (*it)->getPosicion().y > 855 || (*it)->getPosicion().y < 0) { //si la bala sale fuera de la pantalla
            delete* it; //libera su ram
            it = lista_proyectiles.erase(it); //y la borra de la lista de control
        }
        else { //si sigue dentro de la pantalla
            ++it; //pasa al siguiente proyectil del bucle
        }
    }

    if (!combateFinalizado && (piezaIzquierda->getVidaBase() <= 0 || piezaDerecha->getVidaBase() <= 0)) { //revisa si ha muerto alguien
        combateFinalizado = true; //marca que la pelea termino
        if (piezaIzquierda->getVidaBase() <= 0) { //si murio el jugador de la izquierda
            textoVictoria.setString("GANADOR BANDO DE OSCURIDAD"); //escribe que gano el de la derecha
            textoVictoria.setFillColor(sf::Color::Red); //en color rojo
        }
        else { //si murio el de la derecha
            textoVictoria.setString("GANADOR BANDO DE LUZ"); //pone ganador luz
            textoVictoria.setFillColor(sf::Color::Cyan); //en color cian
        }

        sf::FloatRect limitesTexto = textoVictoria.getLocalBounds(); //mide cuanto ocupa el texto
        textoVictoria.setOrigin({ limitesTexto.size.x / 2.f, limitesTexto.size.y / 2.f }); //le pone el pivote en su centro
        textoVictoria.setPosition({ 550.f, 427.f }); //lo coloca en el medio absoluto de la pantalla
    }
}

//para ver si los proyectiles chocan con los personajes o con los obstaculos
void Arena::gestionarColisiones() {
    float radioHitbox = 35.f; //tamaño del radio invisible de las piezas para recibir golpes
    for (auto it = lista_proyectiles.begin(); it != lista_proyectiles.end();) { //revisa cada disparo
        bool proyectilDestruido = false; //flag para saber si este laser debe morir en este frame
        sf::Vector2f posP = (*it)->getPosicion(); //saca sus coordenadas

        if (obstaculos.hayColisionCircular(posP, 5.f)) proyectilDestruido = true; //si toca los muros del centro, se marca para destruir

        if (!proyectilDestruido && piezaDerecha && (*it)->getBando() != piezaDerecha->getBando()) { //si el laser no es amigo del jugador derecho
            if (std::hypot(posP.x - posDerecha.x, posP.y - posDerecha.y) < radioHitbox) { //calcula distancia y vemos si le da
                piezaDerecha->recibirDanyo((*it)->getDanyo()); //le aplica los puntos de daño
                proyectilDestruido = true; //marca el laser como chocado
            }
        }

        if (!proyectilDestruido && piezaIzquierda && (*it)->getBando() != piezaIzquierda->getBando()) { //lo mismo pero contra el jugador izquierdo
            if (std::hypot(posP.x - posIzquierda.x, posP.y - posIzquierda.y) < radioHitbox) { //mide
                piezaIzquierda->recibirDanyo((*it)->getDanyo()); //le hace daño
                proyectilDestruido = true; //marca el laser para borrarlo
            }
        }

        if (proyectilDestruido) { //si el laser choco con un personaje o un obstaculo
            delete* it; //borra el objeto dinamico
            it = lista_proyectiles.erase(it); //y lo saca de la lista
        }
        else ++it; //si el laser no dio a nada, mira la siguiente de la lista
    }
}

//para dubujar las cosas en la pantalla en el orden correcto
void Arena::dibujarPantalla(sf::RenderWindow& ventana) {
    ventana.setView(ventana.getDefaultView()); 
    ventana.draw(*spriteFondoArena); //dibuja el cesped primero
    obstaculos.dibujar(ventana); //encima del cesped pone los obstaculos

    for (const auto& onda : lista_ondas) ventana.draw(onda.forma); //dibuja las ondas de golpes cuerpo a cuerpo activas

    auto dibujarAuraDanyo = [&](Pieza* p, sf::Vector2f pos) { 
        if (p && p->estaParpadeandoDanyo()) { //si la pieza esta sufriendo daño ahora mismo
            sf::CircleShape auraImpacto(45.f); //crea un circulo semitransparente rojo
            auraImpacto.setOrigin({ 45.f, 45.f }); //lo centra y
            auraImpacto.setPosition(pos); //lo pone debajo de la pieza
            auraImpacto.setFillColor(sf::Color(255, 0, 0, 130)); //rojo suave
            ventana.draw(auraImpacto); //lo dibuja
        }
        };

    if (piezaIzquierda) { //si existe el jugador 1
        dibujarAuraDanyo(piezaIzquierda, posIzquierda); //dibuja su circulo rojo si hace falta
        piezaIzquierda->procesarEfectoVisual(); //actualiza sus colores de daño
        piezaIzquierda->dibujarEnArena(ventana, posIzquierda, true, skinArena); //dibuja su sprite principal mirando a la derecha
    }
    if (piezaDerecha) { //si existe el jugador 2
        dibujarAuraDanyo(piezaDerecha, posDerecha); //aura si le acaban de pegar
        piezaDerecha->procesarEfectoVisual(); //color visual
        piezaDerecha->dibujarEnArena(ventana, posDerecha, false, skinArena); //le manda dibujarse pero invertido para mirar a la izquierda
    }

    for (auto p : lista_proyectiles) p->dibujar(ventana); //dibuja los fueguitos y lasers volando por la pantalla

    
    graficos.actualizar(faseCuentaAtras); //le dice a la interfaz que actualice sus datos
    graficos.dibujar(ventana, faseCuentaAtras >= 0); //dibuja la barra de vida por encima de todos los sprites anteriores

    if (combateFinalizado) ventana.draw(textoVictoria); //si acabo la pelea pinta las letronas del ganador a capa maxima
}