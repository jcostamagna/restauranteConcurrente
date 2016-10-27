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
        estado(ESPERANDO_PEDIDO), idMozoCocinarle(999), pedido("") {

}


void Cocinero::run() {
    this->rutinaCocinero();

    std::cout << "Termino el proceso cocinero " << getpid() << std::endl;

    this->eCocinero.cerrar();
}

void Cocinero::rutinaCocinero() {
    while (sigint_handler.getGracefulQuit() == 0) {
        if (apagon_handler_procesos.getApagon() == 1) {
            estado = APAGON_COCINERO;
            apagon_handler_procesos.stopApagon();
        }
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
            estado = ESPERANDO_PEDIDO;
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
    for (i = 0; i < PID_LENGHT; ++i) {
        ss1 << mensaje.at(i);
    }

    for (; i < mensaje.length(); ++i) {
        ss2 << mensaje.at(i);
    }

    ss1 >> this->idMozoCocinarle;
    ss2 >> this->pedido;

    if (apagon_handler_procesos.getApagon() == 1)
        return;

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
    if (apagon_handler_procesos.getApagon() == 1) {
        estado = APAGON_COCINERO;
        apagon_handler_procesos.stopApagon();

        std::stringstream ss;
        ss.str("");
        ss << "APAGON: Cocinero[" << getpid() << "] No entrego el pedido por APAGON!" << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "APAGON: Cocinero[" << getpid() << "] No entrego el pedido por apagon!" << std::endl;
    } else {
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
        idMozoCocinarle = 999; // para que no entre en este if?
        avanzarEstado();
    }
}

void Cocinero::apagon() {
    std::stringstream ss;
    ss << "APAGON: Cocinero[" << getpid() << "] MODO APAGON" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "APAGON: Cocinero[" << getpid() << "] MODO APAGON" << std::endl;

    limpiarPipePedidos();

    sleep(TIEMPO_APAGON);
    avanzarEstado();
}

Cocinero::~Cocinero() {

}

void Cocinero::cocinar(std::string pedido) {
    sleep(1);  // dormir la cantidad de comidas * 0.5
}

void Cocinero::limpiarPipePedidos() {
    bool terminaLimpieza = false;

    //Leemos con un lock de lectura

    while (!terminaLimpieza) {
        std::stringstream ss2;
        unsigned i;
        terminaLimpieza = true;
        for (i = PID_LENGHT; i < pedido.length(); ++i) {
            ss2 << pedido.at(i);
            terminaLimpieza &= pedido.at(i) == LIMPIAR_PEDIDOS;
        }
        //Si todos los chars son X, se termino la limpieza
        if (terminaLimpieza)
            return;

        char buffer[BUFFSIZE];

        std::stringstream ss;
        ss << "COCINERO: esperando para limpiar pedido..." << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "COCINERO: esperando para limpiar pedido..." << std::endl;
        ss.str("");
        ss.flush();
        ss.clear();

        ssize_t bytesLeidos = this->eCocinero.leer(static_cast<void *>(buffer), BUFFSIZE);  // Leo pedidos de cualquier mozo

        if (bytesLeidos <= 0) {
            return;
        }
        std::string mensaje = buffer;
        mensaje.resize(bytesLeidos); //Si todos escribimos BUFFSIZE no haria falta...

        pedido = mensaje;

        ss << "COCINERO: tirando el pedido..." << pedido << std::endl;
        Log::getInstance()->log(ss.str());
        std::cout << "COCINERO: tirando el pedido..." << pedido << std::endl;
    }
}
