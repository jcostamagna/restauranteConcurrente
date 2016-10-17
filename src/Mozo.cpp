//
// Created by ale on 16/10/16.
//

#include <iostream>
#include "Mozo.h"

Mozo::Mozo(Pipe& pedidos): pedidos(pedidos), vive(true), estado (RECIBIENDO_ORDEN) {}

Mozo::~Mozo() {
}

void Mozo::run() {
    this->rutinaMozo();
}


void Mozo::rutinaMozo() {
    while (vive) {
        switch (estado) {
            case RECIBIENDO_ORDEN:
                recibiendoOrden();
                break;
            case ESPERANDO_COMIDA:
                esperandoComida();
                break;
            case ENTREGANDO_COMIDA:
                entregandoComida();
                break;
            case ENTREGANDO_CUENTA:
                entregandoCuenta();
                break;
            case APAGON:
                apagon();  // kill me
                break;
            default:
                estado = RECIBIENDO_ORDEN;
                break;
        }
    }
    this->pedidos.cerrar ();
    exit ( 0 );
}

void Mozo::avanzarEstado() {
    switch (estado) {
        case RECIBIENDO_ORDEN:
            estado = ESPERANDO_COMIDA;
            break;
        case ESPERANDO_COMIDA:
            estado = ENTREGANDO_COMIDA;
            break;
        case ENTREGANDO_COMIDA:
            estado = RECIBIENDO_ORDEN;
            break;
        case ENTREGANDO_CUENTA:
            estado = RECIBIENDO_ORDEN;
            break;
        case APAGON:
            estado = RECIBIENDO_ORDEN;  // kill me
            break;
        default:
            estado = RECIBIENDO_ORDEN;
            break;
    }
}

void Mozo::esperandoComida() {

}

void Mozo::recibiendoOrden() {
    static const int BUFFSIZE = 100;
    // lector
    char buffer[BUFFSIZE];

    std::cout << "Lector: esperando para leer..." << std::endl;
    ssize_t bytesLeidos = this->pedidos.leer ( static_cast<void*>(buffer),BUFFSIZE );
    std::string mensaje = buffer;
    mensaje.resize ( bytesLeidos );
    if(mensaje == "comida") {
        std::cout << "ES IGUAL A COMIDA" << std::endl;
        estado = ENTREGANDO_COMIDA;
    } else if (mensaje == "cuenta") {
        estado = ENTREGANDO_CUENTA;
    }

    std::cout << "Mozo: lei el dato [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe" << std::endl;
    std::cout << "Mozo: fin del proceso" << std::endl;
}

void Mozo::entregandoComida() {
    std::cout << "Mozo entrega COMIDA" << std::endl;
    vive = false;
}

void Mozo::entregandoCuenta() {
    std::cout << "Mozo entrega cuenta" << std::endl;
    vive = false;
}

void Mozo::apagon() {

}
