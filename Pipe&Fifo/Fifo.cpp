#include <cstring>
#include <iostream>
#include "Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {
	int result = mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 );
	if (result == -1){
		std::string mensaje = std::string("Error en mknod() (constructor): ") + std::string(strerror(errno));
		std::cerr<<mensaje<<std::endl;
		throw mensaje;
	}
}

Fifo::~Fifo() {
}

void Fifo::cerrar() {
	close ( fd );
	fd = -1;
}

void Fifo::eliminar() const {
	unlink ( nombre.c_str() );
}
