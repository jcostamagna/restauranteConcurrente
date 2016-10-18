//
// Created by juan on 15/10/16.
//
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <SIGINT_Handler.h>
#include <SignalHandler.h>
#include "Recepcionista.h"


Recepcionista::Recepcionista(Pipe &clientes, LockFd& lecturaPuerta) : clientes(clientes), vive(true), estado(ESPERANDO), lecturaPuerta(lecturaPuerta) {}

void Recepcionista::run() {

    // se registra el event handler declarado antes
    SignalHandler::getInstance()->registrarHandler(SIGINT, &sigint_handler);

    this->rutinaRecepcionista();

    // se recibio la senial SIGINT, se sale del while y sigue aca,  el proceso termina
    SignalHandler::destruir();
    std::cout << "Termino el proceso " << getpid() << std::endl;

    this->clientes.cerrar();
    exit(0);
}

void Recepcionista::rutinaRecepcionista() {
    while (sigint_handler.getGracefulQuit() == 0) {
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
    static const int BUFFSIZE = 26;
    // lector
    char buffer[BUFFSIZE];

    std::cout << "Lector: esperando para leer..." << std::endl;
    lecturaPuerta.tomarLock();
    ssize_t bytesLeidos = this->clientes.leer(static_cast<void *>(buffer), BUFFSIZE);
    lecturaPuerta.liberarLock();

    if (bytesLeidos <= 0) return;
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);

    std::cout << "Lector: lei el dato [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe y soy el hijo "
              << getpid() << std::endl;
    //std::cout << "Lector: fin del proceso" << std::endl;
    //vive = false;

}

void Recepcionista::ubicandoEnLiving() {

}

void Recepcionista::ubicandoEnMesa() {

}

void Recepcionista::apagon() {

}
