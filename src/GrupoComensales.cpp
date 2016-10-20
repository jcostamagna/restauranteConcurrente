//
// Created by ale on 17/10/16.
//

#include <cstdlib>
#include <ctime>
#include "GrupoComensales.h"

GrupoComensales::GrupoComensales(): vive(true), estado (ESPERANDO_EN_PUERTA), cantidad(0) {}

GrupoComensales::~GrupoComensales() {
}

void GrupoComensales::run() {
    this->rutinaComensal();
}


void GrupoComensales::sumarCantidad(){
    this->cantidad++;
}
int GrupoComensales::getCantidad(){
    return this->cantidad;
}


void GrupoComensales::rutinaComensal()  {
    while (vive) {
        switch (estado) {
            case ESPERANDO_EN_PUERTA:
                esperandoEnPuerta();
            case ESPERANDO_EN_LIVING:
                esperandoEnLiving();
            case ESPERANDO_ORDENAR:
                esperandoOrdenar();
            case ESPERANDO_MOZO_COMIDA:
                esperandoComida();
            case COMIENDO:
                comiendo();
            case ESPERANDO_PAGAR:
                esperandoPagar();
            case PAGAR_RETIRARSE:
                pagarRetirarse();
            case APAGON:
                apagon();
        }
    }
}

void GrupoComensales::avanzarEstado() {
    switch (estado) {
        case ESPERANDO_EN_PUERTA:
            estado = ESPERANDO_EN_LIVING;
        case ESPERANDO_EN_LIVING:
            estado = ESPERANDO_ORDENAR;
        case ESPERANDO_ORDENAR:
            estado = ESPERANDO_MOZO_COMIDA;
        case ESPERANDO_MOZO_COMIDA:
            estado = COMIENDO;
        case COMIENDO:
            if (calcularRandom(10) < 3)
                estado = ESPERANDO_ORDENAR; // vuelven a pedir comida con una probabilidad de 0.3
            else
                estado = ESPERANDO_PAGAR;  // sino, pagan y se van
        case ESPERANDO_PAGAR:
            estado = PAGAR_RETIRARSE;
        case PAGAR_RETIRARSE:
            vive = false;  // TODO: chequear esto
        case APAGON:
            vive = false;
    }
}

void GrupoComensales::esperandoEnPuerta() {

}

void GrupoComensales::esperandoEnLiving() {

}

void GrupoComensales::esperandoOrdenar() {

}

void GrupoComensales::esperandoComida() {

}

void GrupoComensales::comiendo() {

}

void GrupoComensales::esperandoPagar() {

}

void GrupoComensales::pagarRetirarse() {

}

void GrupoComensales::apagon() {

}


int GrupoComensales::calcularRandom (int tope) {
    std::srand ( std::time(NULL) );
    int resultado = rand() % tope;
    return resultado;
}
