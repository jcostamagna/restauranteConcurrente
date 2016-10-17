//
// Created by juan on 15/10/16.
//
#include <sys/wait.h>
#include <iostream>
#include "Recepcionista.h"


Recepcionista::Recepcionista(Pipe& clientes) : clientes(clientes),vive(true), estado(ESPERANDO) {}

void Recepcionista::run() {
    this->rutinaRecepcionista();
}

void Recepcionista::rutinaRecepcionista(){
    while (vive) {
        switch (estado) {
            case ESPERANDO:
                esperando();
                break;
            case UBICANDO_EN_LIVING:
                ubicandoEnLiving();
                break;
            case UBICANDO_EN_MESA:
                ubicandoEnMesa();
                break;
            case APAGON:
                apagon();
                break;
            default:
                esperando();
                break;
        }
    }
    this->clientes.cerrar ();
    exit ( 0 );
}

void Recepcionista::avanzarEstado() {
    switch (estado) {
        //case ESPERANDO; si estaba ESPERANDO lo define la rutina si va a UBICANDO EN EL LIVING O A UBICANDO EN MESA
        case UBICANDO_EN_LIVING:
            estado = ESPERANDO;
            break;
        case UBICANDO_EN_MESA:
            estado = ESPERANDO;
            break;
        case APAGON:
            estado = ESPERANDO;
            break;
        default:
            estado = ESPERANDO;
            break;
    }
}

void Recepcionista::esperando() {
    static const int BUFFSIZE = 100;
    // lector
    char buffer[BUFFSIZE];

    std::cout << "Lector: esperando para leer..." << std::endl;
    ssize_t bytesLeidos = this->clientes.leer ( static_cast<void*>(buffer),BUFFSIZE );
    std::string mensaje = buffer;
    mensaje.resize ( bytesLeidos );

    std::cout << "Lector: lei el dato [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe" << std::endl;
    std::cout << "Lector: fin del proceso" << std::endl;

    vive = false;
}

void Recepcionista::ubicandoEnLiving() {

}

void Recepcionista::ubicandoEnMesa() {

}

void Recepcionista::apagon() {

}
