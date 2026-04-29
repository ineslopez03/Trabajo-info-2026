#include "MotorArchon.h"




MotorArchon::MotorArchon(){
	estadoActual = EstadoJuego::MENU; //Empezamos en el menú
	//dejamos vacios los punteros al inicio y ya se les asignará valores cuando toque, evitamos accesos no habilitados
	pantallaActiva = nullptr; 
	jugador1 = nullptr;
	jugador2 = nullptr; 
	ejecutando = true; //ponemos a 1 nada mas iniciar para que el bucle se pueda ejecutar
}

MotorArchon::~MotorArchon(){
	//como los voy a crear dinamicamente usando new, debo hacer el delete
	if (pantallaActiva=!nullptr) delete pantallaActiva;
	if (jugador1 = !nullptr) delete jugador1;
	if (jugador2 = !nullptr) delete jugador2;
}
