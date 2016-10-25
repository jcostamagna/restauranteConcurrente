//
// Created by ale on 16/10/16.
//

#include <iostream>
#include "Mozo.h"
#include "Log.h"
#include <sstream>
#include <iomanip>

Mozo::Mozo(int id, Pipe &pedidos, Pipe &escrCocinero, Semaforo &semaforo,
           const std::map<int, Semaforo *> &semaforosMesas)
        : id(id), pedidos(pedidos), eCocinero(escrCocinero), vive(true),
          estado(RECIBIENDO_ORDEN), semaforoConCocinero(semaforo), semaforosMesas(semaforosMesas),
          cuenta("/bin/bash", 'j'), idMesa(-1) {}

Mozo::~Mozo() {

}

void Mozo::run() {
    this->rutinaMozo();

    this->pedidos.cerrar();
    std::cout << "Termino el proceso mozo " << getpid() << std::endl;
}


void Mozo::rutinaMozo() {
    while (sigint_handler.getGracefulQuit() == 0) {
        switch (estado) {
            case RECIBIENDO_ORDEN:
                recibiendoOrden();
                break;
            case ESPERANDO_COMIDA:
                pedirComida();
                break;
            case ENTREGANDO_COMIDA:
                entregandoComida();
                break;
            case ENTREGANDO_CUENTA:
                entregandoCuenta();
                break;
            case APAGON_MOZO:
                apagon();  // kill me
                break;
            default:
                estado = RECIBIENDO_ORDEN;
                break;
        }
    }

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
        case APAGON_MOZO:
            estado = RECIBIENDO_ORDEN;  // kill me
            break;
        default:
            estado = RECIBIENDO_ORDEN;
            break;
    }
}

void Mozo::recibiendoOrden() {
    char buffer[BUFFSIZE];

    std::stringstream ss;
    ss << "Mozo[" << id << "]: esperando para leer pedido..." << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mozo[" << id << "]: esperando para leer pedido..." << std::endl;
    ss.str("");
    ss.flush();
    ss.clear();

    ssize_t bytesLeidos = this->pedidos.leer(static_cast<void *>(buffer), BUFFSIZE);  // Leo pedidos de cualquier mesa
    if (bytesLeidos <= 0) return;
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos); //Si todos escribimos BUFFSIZE no haria falta...

    ss.str("");
    ss << "Mozo[" << id << "]: Leí el PEDIDO [" << mensaje << "]" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mozo[" << id << "]: Leí el PEDIDO [" << mensaje << "]" << std::endl;

    std::stringstream ss1, ss2;
    unsigned i;
    std::string pedido;
    for (i = 0; i < PID_LENGHT; ++i) {
        ss1 << mensaje.at(i);
    }
    bool pedirCuenta = true;
    for (; i < mensaje.length(); ++i) {
        ss2 << mensaje.at(i);
        pedirCuenta &= mensaje.at(i) == '0';
    }

    ss1 >> this->idMesa;
    ss2 >> pedido;

    ss.str("");
    ss << "Mozo[" << id << "]: Leí el PEDIDO del cliente con PID " << idMesa << " y pidio de comer: ";
    ss << pedido << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mozo[" << id << "]: Leí el PEDIDO del cliente con PID " << idMesa << " y pidio de comer: " << pedido << std::endl;

    this-> pedido = pedido;

    if (!pedirCuenta) {
        estado = ESPERANDO_COMIDA;  // Si hay pedido va a pedir la comida al cocinero
    } else {
        estado = ENTREGANDO_CUENTA;  // Si el pedido es 00000 significa que quiere la cuenta
    }
}


void Mozo::pedirComida() {
    std::ostringstream datoStream;

    datoStream << std::setfill('0') << std::setw(PID_LENGHT) << id;
    datoStream << std::setfill('0') << std::setw(BUFFSIZE-PID_LENGHT) << pedido;

    std::string dato = datoStream.str();

    this->eCocinero.escribir(static_cast<const void *>(dato.c_str()), BUFFSIZE);  // Pido la comida

    std::stringstream ss;
    ss << "Mozo[" << id << "]: Intentando acceder al cocinero" << std::endl;
    Log::getInstance()->log(ss.str());
    ss.str("");
    std::cout << "Mozo[" << id << "]: Intentando acceder al cocinero" << std::endl;

    ss << "Mozo[" << id << "]: Escribo mi ID con el pedido en cocinero y me pongo rojo. "
       << "Pedido: " << dato << std::endl;
    Log::getInstance()->log(ss.str());
    ss.str("");

    std::cout << "Mozo[" << id << "]: Escribo mi ID con el pedido en cocinero y me pongo rojo. "
              << "Pedido: " << dato << std::endl;

    semaforoConCocinero.p();  // Me bloqueo mientras el cocinero cocina

    esperarComida();
}


void Mozo::esperarComida() {
    std::stringstream ss;
    ss << "Mozo[" << id << "]: SEMAFORO VERDE, puedo buscar la comida cocinada" << std::endl;
    Log::getInstance()->log(ss.str());
    ss.str("");
    std::cout << "Mozo[" << id << "]: SEMAFORO VERDE, puedo buscar la comida cocinada" << std::endl;

    avanzarEstado();
}

void Mozo::entregandoComida() {
    std::stringstream ss;
    ss << "Mozo[" << id << "] entrega COMIDA y vuelve a recibir ordenes" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mozo[" << id << "] entrega COMIDA y vuelve a recibir ordenes" << std::endl;
    //Desbloqueo mesa
    semaforosMesas.at(idMesa)->v();
    avanzarEstado();
}

void Mozo::entregandoCuenta() {
    std::stringstream ss;
    ss << "Mozo[" << id << "] entrega cuenta" << std::endl;
    Log::getInstance()->log(ss.str());
    std::cout << "Mozo[" << id << "] entrega cuenta" << std::endl;

    semaforosMesas.at(idMesa)->v();
    avanzarEstado();
}

void Mozo::apagon() {

}
