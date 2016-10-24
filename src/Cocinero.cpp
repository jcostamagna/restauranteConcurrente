//
// Created by ale on 17/10/16.
//

#include <iostream>
#include <sstream>
#include <iomanip>
#include "Cocinero.h"
#include "Log.h"

Cocinero::Cocinero(Pipe &escrCocinero, Pipe &lectCocinero, std::list<Semaforo *> &semaforos) :
        eCocinero(escrCocinero), lCocinero(lectCocinero), semaforosCocineroMozos(semaforos), vive(true),
        estado(ESPERANDO_PEDIDO) {

}


void Cocinero::run() {
    this->rutinaCocinero();

    std::cout << "Termino el proceso cocinero" << getpid() << std::endl;

    this->eCocinero.cerrar();
    this->lCocinero.cerrar();
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
    //static const int BUFFSIZE = 12;
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
    unsigned int idMozo;
    std::string pedido;
    for (i = 0; i < PID_LENGHT; ++i) {
        ss1 << mensaje.at(i);
    }

    for (; i < mensaje.length(); ++i) {
        ss2 << mensaje.at(i);
    }

    ss1 >> idMozo;
    ss2 >> pedido;

    ss.str("");
    ss << "Cocinero: Leo al mozo ->" << idMozo << "<-"
       << "Cocinando " << pedido << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Cocinero: Leo al mozo ->" << idMozo << "<-"
              << "Cocinando " << pedido << std::endl;

    /*int N;
    try {
        N = std::stoi(mensaje);
    } catch (...) {
        std::cout << "Problema parseando id mozo" << std::endl;
    }*/

    ss.str("");
    ss << "Cocinero: Pongo en verde el semaforoConCocinero ->" << idMozo << "<-" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Cocinero: Pongo en verde el semaforoConCocinero ->" << idMozo << "<-" << std::endl;
    if (semaforosCocineroMozos.size() > idMozo) {
        std::list<Semaforo *>::iterator it = semaforosCocineroMozos.begin();
        std::advance(it, idMozo);
        (*it)->v();
    }
    avanzarEstado();
}

void Cocinero::entregandoPedido() {
    std::ostringstream datoStream;
    datoStream << std::setfill('0') << std::setw(BUFFSIZE) << "Pedido listo";
    std::string dato = datoStream.str();

    std::stringstream ss;
    ss << "Cocinero: Escribo en mozo: [" << dato << "]" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Cocinero: Escribo en mozo: [" << dato << "]" << std::endl;

    lCocinero.escribir(static_cast<const void *>(dato.c_str()), dato.size());
    avanzarEstado();
}

void Cocinero::apagon() {

}

Cocinero::~Cocinero() {

}
