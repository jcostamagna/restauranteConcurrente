//
// Created by ale on 16/10/16.
//

#include <iostream>
#include "Mozo.h"

Mozo::Mozo(int id, Pipe& pedidos, Pipe& escrCocinero, Pipe& lectCocinero, Semaforo& semaforo)
            : id(id), pedidos(pedidos), eCocinero(escrCocinero), lCocinero(lectCocinero),vive(true),
              estado (RECIBIENDO_ORDEN), semaforo(semaforo) {}

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
    static const int BUFFSIZE = 12;
    std::cout << "Mozo[" << id << "]: Intentando acceder al cocinero" << std::endl;

    std::string dato = std::to_string(id) + "           ";
    std::cout << "Mozo[" << id << "]: Escribo mi ID en cocinero y me pongo rojo" << std::endl;
    this->eCocinero.escribir(static_cast<const void *>(dato.c_str()), BUFFSIZE);
    semaforo.p();
    std::cout << "Mozo[" << id << "]: SEMAFORO VERDE" << std::endl;
    char buffer[BUFFSIZE];
    ssize_t bytesLeidos = this->lCocinero.leer(static_cast<void*>(buffer),BUFFSIZE );
    std::string mensaje = buffer;
    mensaje.resize ( bytesLeidos );
    std::cout << "Mozo[" << id << "] recibio de cocinero: " << mensaje << std::endl;

    avanzarEstado();
}

void Mozo::recibiendoOrden() {
    static const int BUFFSIZE = 12;
    char buffer[BUFFSIZE];

    std::cout << "Mozo[" << id << "]: esperando para leer..." << std::endl;
    ssize_t bytesLeidos = this->pedidos.leer ( static_cast<void*>(buffer),BUFFSIZE );
    std::string mensaje = buffer;
    mensaje.resize ( bytesLeidos );
    std::cout << "PEDIDO: " << mensaje << std::endl;
    if(mensaje == "comida      ") {
        estado = ESPERANDO_COMIDA;
    } else if (mensaje == "cuenta      ") {
        estado = ENTREGANDO_CUENTA;
    }
}

void Mozo::entregandoComida() {
    std::cout << "Mozo[" << id << "] entrega COMIDA y vuelve a recibir Ordenes" << std::endl;
    avanzarEstado();
}

void Mozo::entregandoCuenta() {
    std::cout << "Mozo[" << id << "] entrega cuenta" << std::endl;
    vive = false;
}

void Mozo::apagon() {

}
