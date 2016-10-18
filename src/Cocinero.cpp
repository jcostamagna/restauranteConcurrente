//
// Created by ale on 17/10/16.
//

#include "Cocinero.h"


Cocinero::~Cocinero() {

}

Cocinero::Cocinero() : vive(true), estado(ESPERANDO_PEDIDO) {}


void Cocinero::run() {
    this->rutinaCocinero();
}

void Cocinero::rutinaCocinero() {
    while (vive) {
        switch (estado) {
            case ESPERANDO_PEDIDO:
                esperandoPedido();
            case ENTREGANDO_PEDIDO:
                entregandoPedido();
            case APAGON_COCINERO:
                apagon();
            default:
                esperandoPedido();
        }

    }
}

void Cocinero::esperandoPedido() {

}

void Cocinero::entregandoPedido() {

}

void Cocinero::apagon() {

}
