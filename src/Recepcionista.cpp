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


Recepcionista::Recepcionista(Pipe &clientes, LockFd& lecturaPuerta, Semaforo& escrituraLiving,Pipe &living) :
                            clientes(clientes),living(living), mesaLibre(false),
                                 estado(ESPERANDO), lecturaPuerta(lecturaPuerta), escrituraLiving(escrituraLiving), cantClientesLiving("/bin/bash", 'z'){}

void Recepcionista::run() {

    this->rutinaRecepcionista();

    this->clientes.cerrar();
    this->living.cerrar();
    std::cout << "Termino el proceso recepcionista" << getpid() << std::endl;

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
            case APAGON_RECEPCIONISTA:
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
        case APAGON_RECEPCIONISTA:
            estado = ESPERANDO;
            break;
        default:
            estado = ESPERANDO;
            break;
    }
}

void Recepcionista::esperando() {
    static const int BUFFSIZE = 8;

    // lector
    char buffer[BUFFSIZE];

    std::cout << "Recepcionista("<< getpid() <<"): Esperando clientes..." << std::endl;


    //Leemos con un lock de lectura
    lecturaPuerta.tomarLock();
    ssize_t bytesLeidos = this->clientes.leer(static_cast<void *>(buffer), BUFFSIZE);
    lecturaPuerta.liberarLock();

    if (bytesLeidos <= 0) return;
    std::string mensaje = buffer;
    mensaje.resize(bytesLeidos);

    std::cout << "Recepcionista("<< getpid() <<"): LLego el cliente [" << mensaje << "] (" << bytesLeidos << " bytes) del pipe"
              << std::endl;

    //LLevo clientes al living, o mesa si hay disponible
    this->living.escribir(static_cast<const void *>(mensaje.c_str()), mensaje.size());

    escrituraLiving.p();


    int cantClientes = this->cantClientesLiving.leer();
    std::cout << "Recepcionista("<< getpid() <<"): Cantidad clientes en living [" << cantClientes << "] "<< std::endl;
    cantClientes ++;
    this->cantClientesLiving.escribir(cantClientes);

    escrituraLiving.v();
}

void Recepcionista::ubicandoEnLiving() {

}

void Recepcionista::ubicandoEnMesa() {

}

void Recepcionista::apagon() {

}
