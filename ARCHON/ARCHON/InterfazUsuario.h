#pragma once
class InterfazUsuario
{
public:
	virtual  ~InterfazUsuario() {} //Hacemos un destructor virtual para que borre todo del hijo

	virtual void procesarEntrada() = 0; //cada clase hija deberá implementar esta clase a su manera

	virtual void dibujarPantalla() = 0; //cada clase hija deberá implementar esta clase a su manera
};

