//
// Created by juan on 15/10/16.
//
#include <sys/wait.h>
#include <iostream>
#include "Recepcionista.h"

void Recepcionista::start() {
    this->pid = fork ();
    if (this->pid == 0){
        rutinaRecepcionista();
    }
}

void Recepcionista::stop() {
    waitpid ( this->pid , NULL, 0);
}

void Recepcionista::rutinaRecepcionista(){
    static const int BUFFSIZE = 100;
    // lector
    char buffer[BUFFSIZE];

    std::cout << "Lector: esperando para leer..." << std::endl;
    ssize_t bytesLeidos = this->clientes.leer ( static_cast<void*>(buffer),BUFFSIZE );
    std::string mensaje = buffer;
    mensaje.resize ( bytesLeidos );

    std::cout << "Lector: lei el dato [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe" << std::endl;
    std::cout << "Lector: fin del proceso" << std::endl;

    this->clientes.cerrar ();
    exit ( 0 );
}

int Recepcionista::getPid() {
    return this->pid;
}
