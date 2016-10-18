//
// Created by ale on 16/10/16.
//

#include "Mozo.h"

Mozo::Mozo(): vive(true), estado (RECIBIENDO_ORDEN) {}

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
            case ESPERANDO_COMIDA:
                esperandoComida();
            case ENTREGANDO_COMIDA:
                entregandoComida();
            case ENTREGANDO_CUENTA:
                entregandoCuenta();
            case APAGON_MOZO:
                apagon();  // kill me
            default:
                estado = RECIBIENDO_ORDEN;
        }
    }
}

void Mozo::avanzarEstado() {
    switch (estado) {
        case RECIBIENDO_ORDEN:
            estado = ESPERANDO_COMIDA;
        case ESPERANDO_COMIDA:
            estado = ENTREGANDO_COMIDA;
        case ENTREGANDO_COMIDA:
            estado = RECIBIENDO_ORDEN;
        case ENTREGANDO_CUENTA:
            estado = RECIBIENDO_ORDEN;
        case APAGON_MOZO:
            estado = RECIBIENDO_ORDEN;  // kill me
        default:
            estado = RECIBIENDO_ORDEN;
    }
}

void Mozo::esperandoComida() {

}

void Mozo::recibiendoOrden() {

}

void Mozo::entregandoComida() {

}

void Mozo::entregandoCuenta() {

}

void Mozo::apagon() {

}
