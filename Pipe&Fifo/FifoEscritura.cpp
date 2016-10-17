#include <cstring>
#include <iostream>
#include "FifoEscritura.h"

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre), semEscritores("FifoEscritura.cpp", 1) {
}

FifoEscritura::~FifoEscritura() {
}

void FifoEscritura::abrir() {
    fd = open(nombre.c_str(), O_WRONLY);

    if (fd == -1) {
        std::string mensaje = std::string("Error en open() (constructor): ") + std::string(strerror(errno));
        std::cerr << mensaje << std::endl;
        throw mensaje;
    }
}

ssize_t FifoEscritura::escribir(const void *buffer, const ssize_t buffsize) const {
    semEscritores.p();  // -1  Si hay uno escribiendo nadie mas puede escribir
    ssize_t written = write(fd, buffer, buffsize);
    semEscritores.v();  // +1
    return written;
}
