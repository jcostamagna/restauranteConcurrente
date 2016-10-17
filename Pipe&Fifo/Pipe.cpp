#include "Pipe.h"
#include <string>
#include <string.h>
#include <iostream>
#include <errno.h>

Pipe :: Pipe() : lectura(true), escritura(true), semLectores("Pipe.h",1), semEscritores("Pipe.cpp",1) { //TODO: como chequeo que no falle?
	if (pipe ( this->descriptores ) == -1){
		std::string mensaje = std::string("Error en pipe() (constructor): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;
		throw mensaje;
	}
	/*fcntl ( this->descriptors[0],F_SETFL,O_NONBLOCK );
	fcntl ( this->descriptors[1],F_SETFL,O_NONBLOCK );*/
}

Pipe::~Pipe() {
    semEscritores.eliminar();
    semLectores.eliminar();
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
    std::cerr << "PIPE antes del semaforo para escribir, esta en 1 (deberia)" << std::endl;
    semEscritores.p();  // -1  Si hay uno escribiendo nadie mas puede escribir
    std::cerr << "PIPE paso el semaforo para escribir, queda en 0" << std::endl;
    ssize_t written = write ( this->descriptores[1],dato,datoSize );
    semEscritores.v();  // +1
    std::cerr << "PIPE escrito, el semaforo queda en 1" << std::endl;
	return written;
}

ssize_t Pipe :: leer ( void* buffer,const int buffSize ) {
	if ( this->escritura == true ) {
		close ( this->descriptores[1] );
		this->escritura = false;
	}
    semLectores.p();  // -1  Si hay uno leyendo nadie mas puede leer
    std::cerr << "PIPE paso el semaforo para leer, queda en 0" << std::endl;
    ssize_t cantRead = read ( this->descriptores[0],buffer,buffSize );
    semLectores.v();  // +1
    std::cerr << "PIPE leido, el semaforo queda en 1"<< std::endl;
    return cantRead;
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
