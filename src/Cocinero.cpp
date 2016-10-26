//
// Created by ale on 17/10/16.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Cocinero.h"
#include "Log.h"

Cocinero::Cocinero(Pipe &escrCocinero, std::list<Semaforo *> &semaforos) :
        eCocinero(escrCocinero), semaforosCocineroMozos(semaforos), vive(true),
        estado(ESPERANDO_PEDIDO), idMozoCocinarle(999) {

}


void Cocinero::run() {
    this->rutinaCocinero();

    std::cout << "Termino el proceso cocinero " << getpid() << std::endl;

    this->eCocinero.cerrar();
}

void Cocinero::rutinaCocinero() {
    while (sigint_handler.getGracefulQuit() == 0) {
        switch (estado) {
            case ESPERANDO_PEDIDO:
                esperandoPedido();
                break;
            case ENTREGANDO_PEDIDO:
                entregandoPedido();
                break;
            case APAGON_COCINERO:
                apagon();
                break;
            default:
                esperandoPedido();
                break;
        }

    }
}

void Cocinero::avanzarEstado() {
    switch (estado) {
        case ESPERANDO_PEDIDO:
            estado = ENTREGANDO_PEDIDO;
            break;
        case ENTREGANDO_PEDIDO:
            estado = ESPERANDO_PEDIDO;
            break;
        case APAGON_COCINERO:
            estado = APAGON_COCINERO;
            break;
        default:
            estado = ESPERANDO_PEDIDO;
            break;
    }
}

void Cocinero::esperandoPedido() {
    char buffer[BUFFSIZE];

    std::stringstream ss;
    ss << "Cocinero: esperando pedido..." << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Cocinero: esperando pedido..." << std::endl;


    //como es un solo lector, no necesito lock
    ssize_t bytesLeidos = eCocinero.leer(static_cast<void *>(buffer), BUFFSIZE);
    if (bytesLeidos <= 0) return;
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);

    std::stringstream ss1, ss2;
    unsigned i;
    std::string pedido;
    for (i = 0; i < PID_LENGHT; ++i) {
        ss1 << mensaje.at(i);
    }

    for (; i < mensaje.length(); ++i) {
        ss2 << mensaje.at(i);
    }

    ss1 >> this->idMozoCocinarle;
    ss2 >> pedido;

    ss.str("");
    ss << "Cocinero: Leo al mozo ->" << idMozoCocinarle << "<-"
       << "Cocinando [" << pedido << "]" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Cocinero: Leo al mozo ->" << idMozoCocinarle << "<-"
              << "Cocinando [" << pedido << "]" << std::endl;


    cocinar(pedido);


    avanzarEstado();
}

void Cocinero::entregandoPedido() {
    std::stringstream ss;
    ss << "Cocinero: Entrego pedido a mozo" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Cocinero: Entrego pedido a mozo" << std::endl;

    ss.str("");
    ss << "Cocinero: Pongo en verde el semaforo con el mozo ->" << idMozoCocinarle << "<-" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Cocinero: Pongo en verde el semaforo con el mozo ->" << idMozoCocinarle << "<-" << std::endl;
    if (semaforosCocineroMozos.size() > idMozoCocinarle) {
        std::list<Semaforo *>::iterator it = semaforosCocineroMozos.begin();
        std::advance(it, idMozoCocinarle);
        (*it)->v();
    }
    idMozoCocinarle = 999; //para que no entre en este if
    avanzarEstado();
}

void Cocinero::apagon() {

}

Cocinero::~Cocinero() {

}

void Cocinero::cocinar(std::string pedido) {
    sleep(1);  // dormir la cantidad de comidas * 0.5
}
