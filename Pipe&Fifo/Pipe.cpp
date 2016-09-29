#include "Pipe.h"
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>

Pipe :: Pipe() : lectura(true), escritura(true) {
	if (pipe ( this->descriptores ) == -1){
		std::string mensaje = std::string("Error en pipe() (constructor): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;
		throw mensaje;
	}
	/*fcntl ( this->descriptors[0],F_SETFL,O_NONBLOCK );
	fcntl ( this->descriptors[1],F_SETFL,O_NONBLOCK );*/
}

Pipe::~Pipe() {
}

void Pipe :: setearModo ( const int modo ) {
	if ( modo == LECTURA ) {
		close ( this->descriptores[1] );
		this->escritura = false;

	} else if ( modo == ESCRITURA ) {
		close ( this->descriptores[0] );
		this->lectura = false;
	}
}

ssize_t Pipe :: escribir ( const void* dato,int datoSize ) {
	if ( this->lectura == true ) {
		close ( this->descriptores[0] );
		this->lectura = false;
	}

	return write ( this->descriptores[1],dato,datoSize );
}

ssize_t Pipe :: leer ( void* buffer,const int buffSize ) {
	if ( this->escritura == true ) {
		close ( this->descriptores[1] );
		this->escritura = false;
	}

	return read ( this->descriptores[0],buffer,buffSize );
}

int Pipe :: getFdLectura () const {
	if ( this->lectura == true )
		return this->descriptores[0];
	else
		return -1;
}

int Pipe :: getFdEscritura () const {
	if ( this->escritura == true )
		return this->descriptores[1];
	else
		return -1;
}

void Pipe :: cerrar () {
	if ( this->lectura == true ) {
		close ( this->descriptores[0] );
		this->lectura = false;
	}

	if ( this->escritura == true ) {
		close ( this->descriptores[1] );
		this->escritura = false;
	}
}
