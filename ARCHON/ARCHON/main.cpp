#include "MotorArchon.h"

int main() {
	MotorArchon ARCHON; //se crea el motor del juego

	ARCHON.inicializar(); //inicializo los jugadores y el menu (por ahora)

	ARCHON.bucle(); // se inicia el bucle que es lo que va a funcionar mientras se juega, si se sale del bucle se acaba el juego.

	return 0;


}