#include <cstring>
#include <iostream>
#include "FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre), semLectores("FifoLectura.cpp", 1) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
    fd = open(nombre.c_str(), O_RDONLY);

    if (fd == -1) {
        std::string mensaje = std::string("Error en open() (constructor): ") + std::string(strerror(errno));
        std::cerr << mensaje << std::endl;
        throw mensaje;
    }
}

ssize_t FifoLectura::leer(void *buffer, const ssize_t buffsize) const {
    semLectores.p();  // -1  Si hay uno leyendo nadie mas puede leer
    ssize_t cantRead = read(fd, buffer, buffsize);
    semLectores.v();  // +1
    return cantRead;
}
